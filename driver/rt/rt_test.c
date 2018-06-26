#include <stdio.h>
#include "rt/rt_api.h"

//#define BUFF_SIZE 8192
#define BUFF_SIZE 128

static uint8_t *buffers[2];

static void end_of_transfer(void *arg)
{
    rt_trace(RT_TRACE_DEV_CTRL, "[Trace_DBG] end of transfer, re-enqueue...\n");
    int index = (int)arg;

    for (unsigned int i = 0; i < BUFF_SIZE; i++) {
	rt_trace(RT_TRACE_DEV_CTRL, "Mem@%p: 0x%x\n", (buffers[index] + i),
		 *(buffers[index] + i));
    }

    rt_periph_copy(NULL, 2 * ARCHI_UDMA_TRACER_ID(0),
		   (unsigned int)buffers[index], BUFF_SIZE, 0,
		   rt_event_get(NULL, end_of_transfer, (void *)index));
}

int main()
{

#ifdef __RT_USE_IO
    printf("__RT_USE_IO is set\n");
#else
    printf("__RT_USE_IO is NOT set\n");
#endif

#ifdef __RT_USE_TRACE
    printf("__RT_USE_TRACE is set\n");
#else
    printf("__RT_USE_TRACE is NOT set\n");
#endif

    /* TODO: fix MACRO */
    rt_trace(RT_TRACE_DEV_CTRL, "[TRACE_DBG] Init Tracer\n");
    rt_trace(RT_TRACE_DEV_CTRL, "[TRACE_DBG] rt_event_alloc\n");
    if (rt_event_alloc(NULL, 2))
	return -1;

    rt_trace(RT_TRACE_DEV_CTRL, "[TRACE_DBG] rt_alloc buffer 0\n");
    buffers[0] = rt_alloc(RT_ALLOC_PERIPH, BUFF_SIZE);
    if (buffers[0] == NULL)
	return -1;

    rt_trace(RT_TRACE_DEV_CTRL, "[TRACE_DBG] rt_alloc buffer 1\n");
    buffers[1] = rt_alloc(RT_ALLOC_PERIPH, BUFF_SIZE);
    if (buffers[1] == NULL)
	return -1;

    rt_trace(RT_TRACE_DEV_CTRL, "[TRACE_DBG] enable udma channel clock\n");
    plp_udma_cg_set(plp_udma_cg_get() | (1 << ARCHI_UDMA_TRACER_ID(0)));

    rt_trace(RT_TRACE_DEV_CTRL,
	     "[Trace_DBG] enable udma trace debugger rx event\n");
    soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_TRACER_RX(0));


    rt_trace(RT_TRACE_DEV_CTRL, "[Trace_DBG] rt_periph_copy start\n");

    rt_periph_copy(NULL, 2 * ARCHI_UDMA_TRACER_ID(0), (unsigned int)buffers[0],
		   BUFF_SIZE, 0,
		   rt_event_get(NULL, end_of_transfer, (void *)0));
    rt_periph_copy(NULL, 2 * ARCHI_UDMA_TRACER_ID(0), (unsigned int)buffers[1],
		   BUFF_SIZE, 0,
		   rt_event_get(NULL, end_of_transfer, (void *)1));


    rt_trace(RT_TRACE_DEV_CTRL, "[Trace_DBG] yield looping\n");
    while (1) {
	rt_event_yield(NULL);
    }

    return 0;
}
