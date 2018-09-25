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

#ifndef __RT_TRACE_DEBUGGER_H__
#define __RT_TRACE_DEBUGGER_H__
/* TODO: make this include via rt_data.h, like the other drivers do */
#include "rt_data_trace_debugger.h"

/* address of memory mapped register and controlflags settings the desired
 * bits
 */
#define TRDB_REG_CFG 0x1a120000
#define TRDB_FLAG_ENABLE 1
#define TRDB_FLAG_DISABLE 0

#define TRDB_REG_CTRL 0x1a120004
#define TRDB_FLAG_FLUSH 1

#define TRDB_REG_DUMP 0x1a120008


void rt_trace_debugger_conf_init(rt_trace_dbg_conf_t *conf);
rt_trace_dbg_t *rt_trace_debugger_open(char *, rt_trace_dbg_conf_t *,
				       rt_event_sched_t *, rt_event_t *);

void rt_trace_debugger_control(rt_trace_dbg_t *);
void rt_trace_debugger_close(rt_trace_dbg_t *, rt_event_t *);
void __rt_trace_debugger_eot(void *);
#endif
