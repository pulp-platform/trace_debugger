PULP_APP = rt_test2
PULP_APP_FC_SRCS = rt_test2.c rt_trace_debugger.c
PULP_APP_HOST_SRCS = rt_test2.c
PULP_CFLAGS = -O1 -g -mnohwloop

PULP_USER_CONFIG = $(CURDIR)/config.json
override CONFIG_OPT += runner/peripherals=true

include $(PULP_SDK_HOME)/install/rules/pulp_rt.mk
#make conf CONFIG_OPT=rt/trace=1 to debug
