/*
 * Copyright (C) 2018 ETH Zurich and University of Bologna
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
 * Description:
 */

#include "pulp.h"
#include <stdio.h>

/* TODO: fix this, used by rt code */
/* #define ARCHI_NB_PERIPH 8 */

/* TODO: put them in the right place */
#define ARCHI_UDMA_TRACER_ID(id) 7 /* TODO: collides with TGEN id*/
#define UDMA_TRACER_OFFSET(id) UDMA_PERIPH_OFFSET(ARCHI_UDMA_TRACER_ID(id))
#define UDMA_TRACER_RX_ADDR(id)                                                \
    (ARCHI_UDMA_ADDR + UDMA_TRACER_OFFSET(id) + UDMA_CHANNEL_RX_OFFSET)
#define UDMA_TRACER_OFFSET(id) UDMA_PERIPH_OFFSET(ARCHI_UDMA_TRACER_ID(id))
#define ARCHI_SOC_EVENT_TRACER_RX(id)                                          \
    ARCHI_SOC_EVENT_UDMA_RX(ARCHI_UDMA_TRACER_ID(id))

unsigned int write_and_read_l2(unsigned int *, unsigned int);

int main()
{
    printf("Hello World!\n");
    printf("Read and write to base address: %x\n",
	   write_and_read_l2(rt_l2_base(), 0xdeadfeeb));
    printf("Memory base address: %x\n", (unsigned int)rt_l2_base());
    printf("Memory CAMERA RX: %x\n", UDMA_CAM_RX_ADDR(0));
    printf("Memory TRACER RX: %x\n", UDMA_TRACER_RX_ADDR(0));
    printf("Memory base address: %x\n", (unsigned int)rt_l2_base());
    printf("Identity test: %d\n", (is_fc()));

    printf("Enable uDMA channel of tracer\n"); /* activate clk */
    plp_udma_cg_set(plp_udma_cg_get() | (1 << ARCHI_UDMA_TRACER_ID(0)));

    printf("Enable event\n");
    soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_TRACER_RX(0));

    printf("uDMA request @%x:\n", UDMA_TRACER_RX_ADDR(0));
    plp_udma_enqueue(UDMA_TRACER_RX_ADDR(0), (unsigned int)rt_l2_base(), 128,
		     UDMA_CHANNEL_CFG_SIZE_32);
    printf("Wait for interrupt\n");
    wait_soc_event(ARCHI_SOC_EVENT_TRACER_RX(0));
    clear_soc_event(ARCHI_SOC_EVENT_TRACER_RX(0));

    return 0;
}

unsigned int write_and_read_l2(unsigned int *addr, unsigned int value)
{
    volatile unsigned int *uint_ptr = addr;
    *(uint_ptr) = value;
    return *(uint_ptr);
}
