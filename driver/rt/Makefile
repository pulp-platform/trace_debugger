PULP_APP = driver_example
PULP_APP_FC_SRCS = driver_example.c rt_trace_debugger.c
PULP_APP_HOST_SRCS = driver_example.c
PULP_CFLAGS = -O3 -g -mnohwloop

PULP_USER_CONFIG = $(CURDIR)/config.json
override CONFIG_OPT += runner/peripherals=true

include $(PULP_SDK_HOME)/install/rules/pulp_rt.mk
#make conf CONFIG_OPT=rt/trace=1 to debug
