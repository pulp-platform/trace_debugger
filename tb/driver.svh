// Copyright 2018 Robert Balas
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
//
// Author: Robert Balas (balasr@student.ethz.ch)
// Description: Push stimuli to duv

import trdb_pkg::*;
import trdb_tb_pkg::*;

class Driver;

    virtual trace_debugger_if duv_if;
    mailbox #(Stimuli) mail;
    event   tb_eos;

    function new(virtual trace_debugger_if duv_if, mailbox #(Stimuli) mail,
                event tb_eos);
        this.duv_if = duv_if;
        this.mail   = mail;
        this.tb_eos = tb_eos;
    endfunction


    // parse stimuli file
    function int parse_stimuli(string path, Stimuli stimuli);
        string line;
        int    fp, c;
        string err_str;
        int    linecnt = 0;

        logic                ivalid;
        logic                iexception;
        logic                interrupt;
        logic [CAUSELEN-1:0] cause;
        logic [XLEN-1:0]     tval;
        logic [PRIVLEN-1:0]  priv;
        logic [XLEN-1:0]     iaddr;
        logic [ILEN-1:0]     instr;
        logic                compressed;

        if($test$plusargs("debug"))
            $display("[STIMULI]@%t: Opening file %s.", $time, path);
        fp = $fopen(path, "r");

        if($ferror(fp, err_str)) begin
            $error(err_str);
            return -1;
        end


        while ($fgets(line, fp) != 0) begin
            c = $sscanf(line,"valid=%h exception=%h interrupt=%h cause=%h \
                    tval=%h priv=%h compressed=%h addr=%h instr=%h",
                    ivalid, iexception, interrupt, cause,
                    tval, priv, compressed, iaddr, instr);
            if (c <= 0) begin
                $error("$sscanf error");
                return -1;
            end
            linecnt++;
            stimuli.ivalid.push_front(ivalid);
            stimuli.iexception.push_front(iexception);
            stimuli.interrupt.push_front(interrupt);
            stimuli.cause.push_front(cause);
            stimuli.tval.push_front(tval);
            stimuli.priv.push_front(priv);
            stimuli.iaddr.push_front(iaddr);
            stimuli.instr.push_front(instr);
            stimuli.compressed.push_front(compressed);
        end

        if($test$plusargs("debug")) begin
            $display("[STIMULI]@%t: Read %d lines.", $time, linecnt);
        end

        if ($ferror(fp, err_str)) begin
            $error("[ERROR]: %s", err_str);
            return -1;
        end else if ($feof(fp)) begin
            if($test$plusargs("debug"))
                $display("[STIMULI]@%t: Finished parsing %s.", $time, path);
        end

        if(!fp)
            $fclose(fp);
        return 0;
    endfunction

    // parse stimuli file in cvs format
    function int parse_cvs_stimuli(string path, Stimuli stimuli);
        string line;
        int    fp, c;
        string err_str;
        int    linecnt = 0;

        logic                ivalid;
        logic                iexception;
        logic                interrupt;
        logic [CAUSELEN-1:0] cause;
        logic [XLEN-1:0]     tval;
        logic [PRIVLEN-1:0]  priv;
        logic [XLEN-1:0]     iaddr;
        logic [ILEN-1:0]     instr;
        logic                compressed;

        if($test$plusargs("debug"))
            $display("[STIMULI]@%t: Opening file %s.", $time, path);
        fp = $fopen(path, "r");

        if($ferror(fp, err_str)) begin
            $error(err_str);
            return -1;
        end

        c = $fgets(line, fp);
        if (c <= 0) begin
            $error("$sscanf error");
            return -1;
        end


        while ($fgets(line, fp) != 0) begin
            c = $sscanf(line,"%h,%h,%h,%h,%h,%h,%h,%h",
                        ivalid, iaddr, instr, priv, iexception,
                        cause, tval, interrupt);
            if (c <= 0) begin
                $error("$sscanf error");
                return -1;
            end
            compressed = ((instr[1:0] & 2'h3) != 2'h3);
            linecnt++;
            stimuli.ivalid.push_front(ivalid);
            stimuli.iexception.push_front(iexception);
            stimuli.interrupt.push_front(interrupt);
            stimuli.cause.push_front(cause);
            stimuli.tval.push_front(tval);
            stimuli.priv.push_front(priv);
            stimuli.iaddr.push_front(iaddr);
            stimuli.instr.push_front(instr);
            stimuli.compressed.push_front(compressed);
        end

        if($test$plusargs("debug")) begin
            $display("[STIMULI]@%t: Read %d lines.", $time, linecnt);
        end

        if ($ferror(fp, err_str)) begin
            $error("[ERROR]: %s", err_str);
            return -1;
        end else if ($feof(fp)) begin
            if($test$plusargs("debug"))
                $display("[STIMULI]@%t: Finished parsing %s.", $time, path);
        end

        if(!fp)
            $fclose(fp);
        return 0;
    endfunction


    function void apply_zero();
        if($test$plusargs("debug"))
            $display("[DRIVER] @%t: Applying zero stimuli.", $time);
        this.duv_if.cb.ivalid     <= 1'b0;
        this.duv_if.cb.iexception <= 1'b0;
        this.duv_if.cb.interrupt  <= 1'b0;
        this.duv_if.cb.cause      <= 5'b0;
        this.duv_if.cb.tval       <= '0;
        this.duv_if.cb.priv       <= 3'h7; // PRIVLEN'h7;
        this.duv_if.cb.iaddr      <= '0;
        this.duv_if.cb.instr      <= '0;
        this.duv_if.cb.compressed <= 1'b0;
    endfunction


    task write_apb(input logic [3:0] waddr, input logic [31:0] wdata);
        // write request
        @(this.duv_if.cb);
        this.duv_if.cb.paddr  <= waddr;
        this.duv_if.cb.pwrite <= 1'b1;
        this.duv_if.cb.psel   <= 1'b1;
        this.duv_if.cb.pwdata <= wdata;

        @(this.duv_if.cb);
        // we are now the access state of the apb
        this.duv_if.cb.penable     <= 1'b1;

        // wait until request consumed
        @(this.duv_if.cb);
        wait(this.duv_if.cb.pready == 1);
        this.duv_if.cb.penable     <= 1'b0;
        this.duv_if.cb.psel   <= 1'b0;
    endtask


    task test_sw_dump();
        write_apb(REG_TRDB_DUMP, 32'ha0a0a0a0);
        write_apb(REG_TRDB_DUMP, 32'hb0b0b0b0);
        write_apb(REG_TRDB_DUMP, 32'hc0c0c0c0);
        write_apb(REG_TRDB_DUMP, 32'hd0d0d0d0);
        write_apb(REG_TRDB_DUMP, 32'hf0f0f0f0);
    endtask


    task run();
        Stimuli stimuli;
        int full_address, implicit_ret;

        full_address = ($test$plusargs("fulladdr") != 0);
        implicit_ret = ($test$plusargs("implicitret") != 0);

        if($test$plusargs("debug"))
            $display("[DRIVER] @%t: Entering run() task.", $time);

        wait(this.duv_if.rst_ni == 1);
            if($test$plusargs("debug"))
                $display("[DRIVER] @%t: Reset low.", $time);


        // parse stimuli file
        stimuli = new();

        // assign default test
        if(!$value$plusargs("testname=%s", testname))
            testname = default_stimuli_path;

        if($test$plusargs("cvs")) begin
            if(parse_cvs_stimuli(testname, stimuli))
                $fatal("parse_cvs_stimuli() failed");
        end else begin
            if(parse_stimuli(testname, stimuli))
                $fatal("parse_stimuli() failed");
        end

        // send to scoreboard
        mail.put(stimuli);

        //TODO: fix this hack
        apply_zero();

        @(this.duv_if.cb);
        apply_zero();

        // enable trace debugger
        write_apb(REG_TRDB_CTRL, (1 << TRDB_ENABLE)
                  | (1 << TRDB_TRACE_ACTIVATED)
                  | (full_address << TRDB_FULL_ADDR)
                  | (implicit_ret << TRDB_IMPLICIT_RET));

        // apply stimuli according to Top-Down Digital VLSI Design (Kaeslin)
        for(int i = stimuli.ivalid.size() - 1; i >= 0; i--) begin

            @(this.duv_if.cb);

            // #STIM_APPLICATION_DEL;

            this.duv_if.cb.ivalid     <= stimuli.ivalid[i];
            this.duv_if.cb.iexception <= stimuli.iexception[i];
            this.duv_if.cb.interrupt  <= stimuli.interrupt[i];
            this.duv_if.cb.cause      <= stimuli.cause[i];
            this.duv_if.cb.tval       <= stimuli.tval[i];
            this.duv_if.cb.priv       <= stimuli.priv[i];
            this.duv_if.cb.priv[2]    <= 1'b1; // we force debug mode, stimuli from
                                               // spike traces don't know that bit
            this.duv_if.cb.iaddr      <= stimuli.iaddr[i];
            this.duv_if.cb.instr      <= stimuli.instr[i];
            this.duv_if.cb.compressed <= stimuli.compressed[i];

            // #(RESP_ACQUISITION_DEL - STIM_APPLICATION_DEL);
            // take response in monitor.svh
        end

        // wait for the tracer fifo to empty (TODO: poll fifo or something)
        repeat (30) @(this.duv_if.cb);

        $display("[DRIVER] @%t: Testing software writes", $time);
        test_sw_dump();

        // wait for the tracer fifo to empty (TODO: poll fifo or something)
        repeat (30) @(this.duv_if.cb);

        $display("[DRIVER] @%t: Flushing buffers.", $time);
        // write flush command to register
        write_apb(REG_TRDB_CTRL, (1 << TRDB_FLUSH_STREAM | 3));


        $display("[DRIVER] @%t: Driver finished.", $time);

        repeat (100) begin
            @(this.duv_if.cb);
            apply_zero();
        end

        -> tb_eos;

    endtask;


endclass // Driver
