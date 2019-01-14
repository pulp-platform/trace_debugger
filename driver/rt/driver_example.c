#include <stdio.h>
#include "pulp.h"
#include "rt/rt_api.h"
#include "rt_trace_debugger.h"


unsigned do_some_computation(unsigned arg0, unsigned arg1, unsigned arg3)
{
    unsigned buffer_size = 24;
    volatile char buffer[buffer_size];

    unsigned tmp = arg0 + arg1;

    for (unsigned i = 0; i < buffer_size; i++) {
	buffer[i] = 0xff;
    }

    return tmp * arg3 - arg0;
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

    /*
     *  QSPI Setup
     */
    rt_spim_conf_t spi_conf;
    /* Get default configuration */
    rt_spim_conf_init(&spi_conf);
    /* Set maximum baudrate. Can actually be lower than that depending on the
     * best divider found
     */
    spi_conf.max_baudrate = 1000000000;
    /* SPI interface identifier as the Pulp chip can have several interfaces
     */
    spi_conf.id = 0;
    /* Chip select */
    spi_conf.cs = 1;
    /* Then open the device */
    rt_spim_t *spim = rt_spim_open(NULL, &spi_conf, NULL);
    if (spim == NULL)
	return -1;


    /*
     *  Trace Debugger Setup
     */
    rt_trace_dbg_conf_t trdb_conf;
    rt_trace_debugger_conf_init(&trdb_conf);
    trdb_conf.buffer_size = 4 * 1024;
    trdb_conf.ctrl_reg =
	TRDB_ENABLE | TRDB_TRACE_ACTIVATED; /* enable clock and tracer*/
    /* Open it */
    rt_trace_dbg_t *trdb =
	rt_trace_debugger_open("test000", &trdb_conf, spim, NULL, NULL);
    if (!trdb) {
	rt_error("[TEST] failed to open trace debugger\n");
	return -1;
    }

    /*
     * Tracing begins ...
     */

    /* Let's call some functions to generated some code */
    printf("Hello World!\n");
    printf("Memory base address: %x\n", (unsigned int)rt_l2_base());

    int *abuf = rt_alloc(RT_ALLOC_FC_DATA, sizeof(int) * 128);
    if (!abuf) {
	rt_error("[TEST] out of memory\m");
	return -1;
    }

    printf("Memory CAMERA RX: %x\n", UDMA_CAM_RX_ADDR(0));
    printf("Memory TRACER RX: %x\n", UDMA_TRACER_RX_ADDR(0));
    printf("Whoami: %d\n", (is_fc()));

    rt_free(RT_ALLOC_FC_DATA, abuf, sizeof(int) * 128);

    /* infinite loop */
    for (;;) {
	rt_event_execute(NULL, 0);
    }
    rt_trace_debugger_close(trdb, NULL);

    rt_spim_close(spim, NULL);

    return 0;
}
