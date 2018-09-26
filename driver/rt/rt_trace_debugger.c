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
rt_spim_t *trdb_spi;

/* for type conversions */
union uint2ptr {
    unsigned int i;
    unsigned int *p;
} conv;

/* just comment this since if its too hard to enable the rt trace */
#define rt_trace(trace, x...)                                                  \
    do {                                                                       \
	rt_msg(x);                                                             \
    } while (0)

/* we use this for register writes */
static void write_reg_l2(unsigned int addr, unsigned int value)
{
    conv.i = addr;
    volatile unsigned int *uint_ptr = conv.p;
    *(uint_ptr) = value;
}


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
				       rt_spim_t *spi, rt_event_sched_t *sched,
				       rt_event_t *event)
{
    /* TODO: open only once */
    if (!conf || !dev_name)
	return NULL;

    if (!spi)
	return NULL;
    trdb_spi = spi;

    /* TODO: do I need to disable irq, what could potentially go wrong? */
    int irq = rt_irq_disable();
    rt_trace(RT_TRACE_DEV_CTRL,
	     "[TRDB] opening trace debugger device (name: %s)\n", dev_name);

    /* TODO: let's skip that for now */
    /* rt_dev_t *dev = rt_dev_get(dev_name); */

    buffer_size = conf->buffer_size;
    dummy = conf->dummy;
    /* TODO: is it ok to hold onto this pointer?*/
    sched_i = sched;

    if (rt_event_alloc(sched, 2))
	goto fail_event;

    rt_trace_dbg_t *tracer = rt_alloc(RT_ALLOC_FC_DATA, sizeof(rt_trace_dbg_t));
    if (!tracer) {
	rt_error("[TRDB] failed to allocate rt_trace_dbg_t struct memory\n");
	goto fail_alloc_fc;
    }

    tracer->channel = 2 * ARCHI_UDMA_TRACER_ID(0);
    tracer->buffer_size = conf->buffer_size;

    trace_buffs[0] = rt_alloc(RT_ALLOC_PERIPH, buffer_size);
    trace_buffs[1] = rt_alloc(RT_ALLOC_PERIPH, buffer_size);
    if (!trace_buffs[0] || !trace_buffs[1]) {
	rt_error("[TRDB] failed to allocate memory for double buffers\n");
	goto fail_alloc_periph;
    }


    /* This handles the udma side, enables the clock for the peripheral and
     * enables the event in the event unit
     */
    plp_udma_cg_set(plp_udma_cg_get() | (1 << ARCHI_UDMA_TRACER_ID(0)));
    soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_TRACER_RX(0));

    /* TODO: don't know if need to propagate rt_periph_copy_t*/
    rt_periph_copy(NULL, 2 * ARCHI_UDMA_TRACER_ID(0),
		   (unsigned int)trace_buffs[0], buffer_size, 0,
		   rt_event_get(sched_i, __rt_trace_debugger_eot, (void *)0));
    rt_periph_copy(NULL, 2 * ARCHI_UDMA_TRACER_ID(0),
		   (unsigned int)trace_buffs[1], buffer_size, 0,
		   rt_event_get(sched_i, __rt_trace_debugger_eot, (void *)1));

    /* This enables the trace debugger side (which lies close the fc) */
    write_reg_l2(TRDB_REG_CFG, TRDB_FLAG_ENABLE);

    rt_trace(RT_TRACE_DEV_CTRL,
	     "[TRDB] succesfully opened trace debugger device (name: %s)\n",
	     dev_name);

    return tracer;

fail_alloc_periph:
    if (!trace_buffs[0])
	rt_free(RT_ALLOC_PERIPH, trace_buffs[0], buffer_size);
    if (!trace_buffs[1])
	rt_free(RT_ALLOC_PERIPH, trace_buffs[1], buffer_size);
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
    write_reg_l2(TRDB_REG_CFG, TRDB_FLAG_DISABLE);
    /* TODO: don't know what to do with event */
    if (!handle)
	rt_free(RT_ALLOC_FC_DATA, handle, sizeof(rt_trace_dbg_t));
    if (!trace_buffs[0])
	rt_free(RT_ALLOC_PERIPH, trace_buffs[0], buffer_size);
    if (!trace_buffs[1])
	rt_free(RT_ALLOC_PERIPH, trace_buffs[1], buffer_size);

    soc_eu_fcEventMask_clearEvent(ARCHI_SOC_EVENT_TRACER_RX(0));
    /* TODO: probably shouldnt kill the transfer like that */
    plp_udma_cg_set(plp_udma_cg_get() & ~(1 << ARCHI_UDMA_TRACER_ID(0)));
    rt_trace(RT_TRACE_DEV_CTRL, "[TRDB] closed trace debugger device.\n");
    return;
}

/* ensure continous transfer */
void __rt_trace_debugger_eot(void *arg)
{
    int index = (int)arg;

    rt_trace(RT_TRACE_DEV_CTRL,
	     "[TRDB] end of transfer, re-enqueue buffer: %d\n", index);

    /* /\* TODO: remove this quick and dirty debugging stuff*\/ */
    /* for (unsigned int i = 0; i < buffer_size; i++) { */
    /* 	rt_trace(RT_TRACE_DEV_CTRL, "Mem@%p: 0x%x\n", (trace_buffs[index] + i),
     */
    /* 		 *(trace_buffs[index] + i)); */
    /* } */

    rt_spim_send_qspi(trdb_spi, trace_buffs[(int)arg], buffer_size * 8,
		      RT_SPIM_CS_AUTO,
		      rt_event_get(sched_i, __rt_spim_eot, (void *)index));
    /* rt_periph_copy( */
    /* 	NULL, 2 * ARCHI_UDMA_TRACER_ID(0), (unsigned int)trace_buffs[index], */
    /* 	buffer_size, 0, */
    /* 	rt_event_get(sched_i, __rt_trace_debugger_eot, (void *)index)); */
}

void __rt_spim_eot(void *arg)
{
    rt_trace(RT_TRACE_DEV_CTRl, "[TRDB] spi end of transfer of buffer: %d\n",
	     (int)arg);
}
