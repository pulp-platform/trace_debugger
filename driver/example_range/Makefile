PULP_APP = range
PULP_APP_FC_SRCS = range.c ../rt/rt_trace_debugger.c
PULP_APP_HOST_SRCS =
PULP_CFLAGS = -O0 -g -mnohwloop

PULP_USER_CONFIG = $(CURDIR)/config.json
override CONFIG_OPT += runner/peripherals=true

include $(PULP_SDK_HOME)/install/rules/pulp_rt.mk
#make conf CONFIG_OPT=rt/trace=1 to debug
