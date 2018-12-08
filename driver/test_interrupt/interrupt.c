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
 * Description: Test for a straightforward udma interrupt situation
 */

#include "pulp.h"
#include <stdio.h>


int main()
{
    plp_udma_cg_set(plp_udma_cg_get() | (1 << ARCHI_UDMA_TRACER_ID(0)));
    soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_TRACER_RX(0));
    plp_udma_enqueue(UDMA_TRACER_RX_ADDR(0), (unsigned int)rt_l2_base(), 12,
		     UDMA_CHANNEL_CFG_SIZE_32);
    wait_soc_event(ARCHI_SOC_EVENT_TRACER_RX(0));
    clear_soc_event(ARCHI_SOC_EVENT_TRACER_RX(0));
    printf("Got interrupt\n");
    return 0;
}

