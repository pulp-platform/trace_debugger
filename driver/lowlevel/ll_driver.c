/*
 * Copyright (C) 2018 Robert Balas
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Robert Balas (balasr@student.ethz.ch)
 * Description: Test the APB interface and UDMA functionality of the trace
 * debugger
 */

#include "pulp.h"
#include <stdio.h>

union uint2ptr{
    unsigned int i;
    unsigned int* p;
} conv;

void write_l2(unsigned int *addr, unsigned int value);

unsigned int read_l2(unsigned int *addr);

unsigned int write_and_read_l2(unsigned int *, unsigned int);

int main()
{
    /* activate clk to tracer interface to UDMA*/
    plp_udma_cg_set(plp_udma_cg_get() | (1 << ARCHI_UDMA_TRACER_ID(0)));

    /* the trace debugger registers are mapped to
     * `define TRDB_START_ADDR          32'h1A12_0000
     * `define TRDB_END_ADDR            32'h1A12_0FFF
     * see in periph_bus_defines.sv
     */

    /* allow interrupt by tracer interface to udma */
    soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_TRACER_RX(0));

    /* quickly queue some udma transfer */
    unsigned int size = 32;
    unsigned int base = (unsigned int)rt_l2_base();
    plp_udma_enqueue(UDMA_TRACER_RX_ADDR(0), base, size,
		     UDMA_CHANNEL_CFG_SIZE_32);

    /* This should make the trace debugger start generating packets */
    conv.i = 0x1a120000;
    write_l2(conv.p, 1);

    printf("uDMA request @%x:\n", UDMA_TRACER_RX_ADDR(0));


    /* Let's call some functions to generated some code */
    printf("Hello World!\n");
    printf("Memory base address: %x\n", (unsigned int)rt_l2_base());
    printf("Memory CAMERA RX: %x\n", UDMA_CAM_RX_ADDR(0));
    printf("Memory TRACER RX: %x\n", UDMA_TRACER_RX_ADDR(0));
    printf("Whoami: %d\n", (is_fc()));

    printf("Wait for interrupt\n");
    wait_soc_event(ARCHI_SOC_EVENT_TRACER_RX(0));
    clear_soc_event(ARCHI_SOC_EVENT_TRACER_RX(0));

    /* we now dump the memory to check what happened */
    for (unsigned int i = base; i < size + base; i+=4){
	conv.i = i;
	printf("mem:%08x\n", read_l2(conv.p));
    }
    return 0;
}

/* Let's just pray that this generated the right code */
void write_l2(unsigned int *addr, unsigned int value)
{
    volatile unsigned int *uint_ptr = addr;
    *(uint_ptr) = value;
}

unsigned int read_l2(unsigned int *addr)
{
    volatile unsigned int *uint_ptr = addr;
    return *(uint_ptr);
}

unsigned int write_and_read_l2(unsigned int *addr, unsigned int value)
{
    volatile unsigned int *uint_ptr = addr;
    *(uint_ptr) = value;
    return *(uint_ptr);
}
