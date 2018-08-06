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
#include <inttypes.h>

#define LOG_ERR(format, ...)                                                   \
    do {                                                                       \
        fprintf(stderr, "%s:%d:0: %s(): " format, __FILE__, __LINE__,          \
                __func__, ##__VA_ARGS__);                                      \
    } while (false)
/* ## is gcc extension */
#define LOG_INFO(format, ...)                                                  \
    do {                                                                       \
        fprintf(stdout, "%s:%d:0: %s(): " format, __FILE__, __LINE__,          \
                __func__, ##__VA_ARGS__);                                      \
    } while (false)


/* TODO: make this typesafer */
#define TRDB_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define MASK_FROM(len) ((1ull << len) - 1)

uint8_t *file_to_char(FILE *fp, long *length);
