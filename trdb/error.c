/*
 * trdb - Trace Debugger Software for the PULP platform
 *
 * Copyright (C) 2018 Robert Balas
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "trace_debugger.h"

const char *trdb_errstr(enum trdb_error_code err)
{
    switch (err) {
    case trdb_ok:
        return "success";

    case trdb_internal:
        return "internal error";

    case trdb_invalid:
        return "invalid argument";

    case trdb_bad_instr:
        return "unknown instruction";

    case trdb_bad_packet:
        return "unknown packet";

    case trdb_bad_config:
        return "bad configuration";

    case trdb_bad_ras:
        return "bad internal return address stack";
    case trdb_bad_cvs_header:
        return "bad cvs header";

    case trdb_unimplemented:
        return "unimplemented";

    case trdb_nomem:
        return "out of memory";

    case trdb_file_open:
        return "cannot open file";

    case trdb_file_size:
        return "cannot get file size";

    case trdb_file_read:
        return "cannot read file";

    case trdb_file_write:
        return "cannot write file";

    case trdb_scan_file:
        return "cannot scan file";

    case trdb_scan_state_invalid:
        return "scan state invalid, bad number of tokens";

    case trdb_arch_support:
        return "architecture not supported";

    case trdb_section_empty:
        return "section is empty";

    case trdb_bad_vma:
        return "vma is pointing to no section";
    }

    return "missing error string";
}

enum trdb_error_code trdb_errcode(int status)
{
    return status >= 0 ? trdb_ok : (enum trdb_error_code)(-status);
}
