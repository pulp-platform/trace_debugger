#include <stdio.h>
#include "pulp.h"
#include "rt/rt_api.h"
#include "../rt/rt_trace_debugger.h"


int trace_me(int a, int b);
int dont_trace_me(int a, int b, int c);

int main()
{
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
    trdb_conf.buffer_size = 28;
    trdb_conf.ctrl_reg = TRDB_ENABLE; /* enable clock */

    /* and open it */
    rt_trace_dbg_t *trdb =
	rt_trace_debugger_open("test000", &trdb_conf, spim, NULL, NULL);
    if (!trdb) {
	rt_error("[TEST] failed to open trace debugger\n");
	return -1;
    }
    /* configure range which we want to trace. We looked at the disassembly to
     * figure out the boundaries of trace_me().
     */
    rt_trace_debugger_ctrl(TRDB_LOWER_ADDR, 0x1c008eca);
    rt_trace_debugger_ctrl(TRDB_HIGHER_ADDR, 0x1c008f0c);

    /* want to trace range */
    rt_trace_debugger_ctrl(TRDB_REG_FILTER,
			   TRDB_APPLY_FILTERS | TRDB_TRACE_RANGE_EVENT);

    /* enable tracing */
    rt_trace_debugger_ctrl(TRDB_REG_CTRL, TRDB_ENABLE | TRDB_TRACE_ACTIVATED);

    trace_me(0, 5);
    dont_trace_me(1, 2, 3);
    trace_me(10, 2);

    /* Let's call some functions to generated some code */
    printf("Hello World!\n");
    printf("Memory base address: %x\n", (unsigned int)rt_l2_base());
    printf("Memory CAMERA RX: %x\n", UDMA_CAM_RX_ADDR(0));
    printf("Memory TRACER RX: %x\n", UDMA_TRACER_RX_ADDR(0));
    printf("Whoami: %d\n", (is_fc()));

    /* Run trdb -dSl --bfd driver_example/driver_example -x tx_spi
     * to recover trace
     */

    /* TODO: wait for dma to finish, for now we just loop */
    for (;;) {
	rt_event_yield(NULL);
    }

    rt_trace_debugger_close(trdb, NULL);
    rt_spim_close(spim, NULL);

    return 0;
}

int trace_me(int a, int b)
{
    int c = a * b;
    int d = c * c;
    return a + b + c;
}

int dont_trace_me(int a, int b, int c)
{
    return -a - b - c;
}
