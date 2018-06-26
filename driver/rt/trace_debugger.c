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
 * Authors: Robert Balas (balasr@student.ethz.ch)
 * Description: Driver for the trace debugger unit
 */


#include "rt/rt_api.h"
/* TODO: Upstream this*/
#include "rt_trace_debugger.h"

/* TODO: should this be allocated in the handler?*/
/* TODO: is this ok if I allocate resources just like this?*/
uint8_t *trace_buffs[2];
unsigned int buffer_size;
unsigned int dummy;
rt_event_sched_t *sched_i;

/* Initialize rt_trace_dbg_conf_t with default values */
void rt_trace_debugger_conf_init(rt_trace_dbg_conf_t *conf)
{
    conf->buffer_size = 128; /* TODO: turn that up */
    conf->dummy = 1;
    return;
}

/* Allocate trace debugger device */
rt_trace_dbg_t *rt_trace_debugger_open(char *dev_name,
				       rt_trace_dbg_conf_t *conf,
				       rt_event_sched_t *sched,
				       rt_event_t *event)
{
    /* TODO: open only once */
    if (!conf || !dev_name)
	return NULL;

    /* TODO: do I need to disable irq, what could potentially go wrong? */
    int irq = rt_irq_disable();
    rt_trace(RT_TRACE_DEV_CTRL,
	     "[TRACE_DBG] Opening trace debugger device (name: %s)\n",
	     dev_name);

    /* TODO: let's skip that for now */
    /* rt_dev_t *dev = rt_dev_get(dev_name); */

    buffer_size = conf->buffer_size;
    dummy = conf->dummy;
    /* TODO: is it ok to hold onto this pointer?*/
    sched_i = sched;

    if (rt_event_alloc(sched, 2))
	goto fail_event;

    rt_trace_dbg_t *tracer = rt_alloc(RT_ALLOC_FC_DATA, sizeof(rt_trace_dbg_t));
    if (!tracer)
	goto fail_alloc_fc;
    tracer->channel = 2 * ARCHI_UDMA_TRACER_ID(0);
    tracer->buffer_size = conf->buffer_size;

    trace_buffs[0] = rt_alloc(RT_ALLOC_PERIPH, buffer_size);
    trace_buffs[1] = rt_alloc(RT_ALLOC_PERIPH, buffer_size);
    if (!trace_buffs[0] || !trace_buffs[1])
	goto fail_alloc_periph;


    plp_udma_cg_set(plp_udma_cg_get() | (1 << ARCHI_UDMA_TRACER_ID(0)));
    soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_TRACER_RX(0));

    /* TODO: don't know if need to propagate rt_periph_copy_t*/
    rt_periph_copy(NULL, 2 * ARCHI_UDMA_TRACER_ID(0),
		   (unsigned int)trace_buffs[0], buffer_size, 0,
		   rt_event_get(sched_i, __rt_trace_debugger_eot, (void *)0));
    rt_periph_copy(NULL, 2 * ARCHI_UDMA_TRACER_ID(0),
		   (unsigned int)trace_buffs[1], buffer_size, 0,
		   rt_event_get(sched_i, __rt_trace_debugger_eot, (void *)1));

    rt_trace(
	RT_TRACE_DEV_CTRL,
	"[TRACE_DBG] Succesfully opened trace debugger device (name: %s)\n",
	dev_name);

    return tracer;

fail_alloc_periph:
    __rt_trace_debugger_cleanup(); /* Maybe one of both succesfully allocated*/
    rt_free(RT_ALLOC_FC_DATA, tracer, sizeof(rt_trace_dbg_t));
fail_alloc_fc:
    soc_eu_fcEventMask_clearEvent(ARCHI_SOC_EVENT_TRACER_RX(0));
    plp_udma_cg_set(plp_udma_cg_get() & ~(1 << ARCHI_UDMA_TRACER_ID(0)));
fail_event:
    rt_irq_restore(irq);
    return NULL;
}

void rt_trace_debugger_control(rt_trace_dbg_t *handle)
{
}

void rt_trace_debugger_close(rt_trace_dbg_t *handle, rt_event_t *event)
{
    /* TODO: don't know what to do with event */
    if (!handle)
	rt_free(RT_ALLOC_FC_DATA, handle, sizeof(rt_trace_dbg_t));
    __rt_trace_debugger_cleanup();
    soc_eu_fcEventMask_clearEvent(ARCHI_SOC_EVENT_TRACER_RX(0));
    plp_udma_cg_set(plp_udma_cg_get() & ~(1 << ARCHI_UDMA_TRACER_ID(0)));
    rt_trace(RT_TRACE_DEV_CTRL, "[TRACE_DBG] Closed trace debugger device.\n");
    return;
}

/* ensure continous transfer */
void __rt_trace_debugger_eot(void *arg)
{
    rt_trace(RT_TRACE_DEV_CTRL, "[TRACE_DBG] End of transfer, re-enqueue...\n");
    int index = (int)arg;

    /* TODO: remove this quick and dirty debugging stuff*/
    for (unsigned int i = 0; i < buffer_size; i++) {
	rt_trace(RT_TRACE_DEV_CTRL, "Mem@%p: 0x%x\n", (trace_buffs[index] + i),
		 *(trace_buffs[index] + i));
    }

    rt_periph_copy(
	NULL, 2 * ARCHI_UDMA_TRACER_ID(0), (unsigned int)trace_buffs[index],
	buffer_size, 0,
	rt_event_get(sched_i, __rt_trace_debugger_eot, (void *)index));
}

/* free all driver resources */
void __rt_trace_debugger_cleanup()
{
    if (!trace_buffs[0])
	rt_free(RT_ALLOC_PERIPH, trace_buffs[0], buffer_size);
    if (!trace_buffs[1])
	rt_free(RT_ALLOC_PERIPH, trace_buffs[1], buffer_size);
    return;
}
