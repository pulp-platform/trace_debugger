/*
 * trdb - Trace Debugger Software for the PULP platform
 *
 * Copyright (C) 2018 ETH Zurich and University of Bologna
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

/*
 * Author: Robert Balas (balasr@student.ethz.ch)
 * Description: Small utility functions
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"

void trdb_log_null(struct trdb_ctx *ctx, const char *format, ...)
{
}

uint8_t *file_to_char(FILE *fp, long *length)
{
    uint8_t *buf = NULL;
    if (!fp) {
        LOG_ERRT("FILE pointer is NULL\n");
        return NULL;
    }
    if (fseek(fp, 0, SEEK_END)) {
        perror("fseek");
        goto fail;
    }
    long len = ftell(fp);
    *length = len;
    if (len == -1) {
        perror("ftell");
        goto fail;
    }

    buf = malloc(len);
    if (!buf) {
        perror("malloc");
        goto fail;
    }

    rewind(fp);
    if (fread(buf, 1, len, fp) != len) {
        perror("fread");
        goto fail;
    }
    return buf;
fail:
    free(buf);
    return NULL;
}
