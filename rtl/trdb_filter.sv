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
// Description: Filtering logic to reduce number of packets generated. There are
// several settings which influence when packets are generated. They all boil
// down to whether qualified is high (generated packets) or not.


import trdb_pkg::*;


module trdb_filter
     (
     // this is manually controllable by the user
     input logic            trace_activated_i,
     output logic           trace_req_deactivate_o,

     // whether to consider filters at all
     input logic            apply_filters_i,

     // trace a specific privilege mode (disabled on RISCY)
     input logic            trace_selected_priv_i,
     input logic [1:0]      which_priv_i,
     input logic [1:0]      priv_i,

     // trace specific address ranges
     input logic            trace_range_event_i,
     input logic            trace_stop_event_i,
     input logic [XLEN-1:0] trace_lower_addr_i,
     input logic [XLEN-1:0] trace_higher_addr_i,
     input logic [XLEN-1:0] iaddr_i,

     output logic           trace_range_match_o,
     output logic           trace_priv_match_o,
     output logic           trace_qualified_o);

    logic                   ip_in_range;
    logic                   priv_matching;

    // ip in specified range
    always_comb begin: ip_range_check
        if(trace_lower_addr_i <= iaddr_i && iaddr_i < trace_higher_addr_i)
            ip_in_range = '1;
        else
            ip_in_range = '0;
    end

    // deactivate when ip lands in specified range
    always_comb begin: stop_check
        trace_req_deactivate_o = apply_filters_i && ip_in_range &&
                                 trace_stop_event_i;
    end

    // privilege matching
    always_comb begin: priv_check
        // priv_matching = '0;
        // if(trace_priv_i == which_priv_i)
        //     priv_matching = '1;
        // else
        //     priv_matching = '0;
        // // consider the case when we want to trace all privilege modes
        // priv_matching     = priv_matching | ~trace_selected_priv_i;
        priv_matching = '1;
    end

    assign trace_range_match_o = ip_in_range;
    assign trace_priv_match_o = priv_matching;

    always_comb begin
        trace_qualified_o = '0;
        // if we are not interested in filters then the users controls the
        // tracing
        if(!apply_filters_i)
            trace_qualified_o = trace_activated_i;
        else
            trace_qualified_o = trace_activated_i &&
                                (trace_range_event_i && ip_in_range) &&
                                priv_matching;
    end

endmodule
