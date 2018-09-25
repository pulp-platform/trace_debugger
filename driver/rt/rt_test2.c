#include <stdio.h>
#include "rt/rt_api.h"
#include "rt_trace_debugger.h"


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
    unsigned int test_runs = 5;

    rt_trace_dbg_conf_t *conf =
	rt_alloc(RT_ALLOC_FC_DATA, sizeof(rt_trace_dbg_conf_t));
    rt_trace_debugger_conf_init(conf);

    rt_trace_dbg_t *handle =
	rt_trace_debugger_open("test000", conf, NULL, NULL);
    if (!handle){
	rt_error("[TEST] failed to open trace debugger\n");
	return -1;
    }

    printf("[TEST] looping\n");

    while (test_runs > 0) {
	test_runs--;
	rt_event_yield(NULL);
    }
    rt_trace_debugger_close(handle, NULL);

    rt_free(RT_ALLOC_FC_DATA, conf, sizeof(rt_trace_dbg_conf_t));

    return 0;
}
