#include <stdio.h>
#include "rt/rt_api.h"

#define BUFF_SIZE 8192

static uint8_t *buffers[2];

static void end_of_transfer(void *arg)
{
    int index = (int)arg;
    //
    rt_periph_copy(NULL, 8, (unsigned int)buffers[index], BUFF_SIZE, 0,
		   rt_event_get(NULL, end_of_transfer, (void *)index));
}

int main()
{
    if (rt_event_alloc(NULL, 2))
	return -1;

    buffers[0] = rt_alloc(RT_ALLOC_PERIPH, BUFF_SIZE);
    if (buffers[0] == NULL)
	return -1;

    buffers[1] = rt_alloc(RT_ALLOC_PERIPH, BUFF_SIZE);
    if (buffers[1] == NULL)
	return -1;

    plp_udma_cg_set(plp_udma_cg_get() | (1 << 8));

    soc_eu_fcEventMask_setEvent(8 * 2);


    rt_periph_copy(NULL, 8, (unsigned int)buffers[0], BUFF_SIZE, 0,
		   rt_event_get(NULL, end_of_transfer, (void *)0));
    rt_periph_copy(NULL, 8, (unsigned int)buffers[1], BUFF_SIZE, 0,
		   rt_event_get(NULL, end_of_transfer, (void *)1));

    while (1) {
	rt_event_yield(NULL);
    }

    return 0;
}
