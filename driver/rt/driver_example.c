#include <stdio.h>
#include "pulp.h"
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

    /* First configure the SPI device */
    rt_spim_conf_t spi_conf;
    /* Get default configuration */
    rt_spim_conf_init(&spi_conf);
    /* Set maximum baudrate. Can actually be lower than
     * that depending on the best divider found
     */
    spi_conf.max_baudrate = 1000000;
    /* SPI interface identifier as the Pulp chip can have
     * several interfaces
     */
    spi_conf.id = 0;
    /* Chip select */
    spi_conf.cs = 1;

    /* Then open the device */
    rt_spim_t *spim = rt_spim_open(NULL, &spi_conf, NULL);
    if (spim == NULL)
	return -1;

    /* setup trace debugger configuration */
    rt_trace_dbg_conf_t trdb_conf;
    rt_trace_debugger_conf_init(&trdb_conf);
    trdb_conf.buffer_size = 512;

    /* and open it */
    rt_trace_dbg_t *trdb =
	rt_trace_debugger_open("test000", &trdb_conf, spim, NULL, NULL);
    if (!trdb) {
	rt_error("[TEST] failed to open trace debugger\n");
	return -1;
    }


    /* Let's call some functions to generated some code */
    printf("Hello World!\n");
    printf("Memory base address: %x\n", (unsigned int)rt_l2_base());
    printf("Memory CAMERA RX: %x\n", UDMA_CAM_RX_ADDR(0));
    printf("Memory TRACER RX: %x\n", UDMA_TRACER_RX_ADDR(0));
    printf("Whoami: %d\n", (is_fc()));

    /* just loop for now */
    printf("[TEST] looping\n");
    while (test_runs > 0) {
	test_runs--;
	rt_event_yield(NULL);
    }
    rt_trace_debugger_close(trdb, NULL);

    rt_spim_close(spim, NULL);

    return 0;
}