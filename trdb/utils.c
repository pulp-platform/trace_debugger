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
    (void)ctx;
    (void)format;
}

uint32_t sign_extendable_bits(uint32_t addr)
{
    if (addr == 0 || addr == UINT32_MAX)
        return 32;
    int clz = __builtin_clz(addr);
    int clo = __builtin_clz(~addr);
    return clz > clo ? clz : clo;
}

uint32_t sign_extendable_bits64(uint64_t addr)
{
    if (addr == 0 || addr == UINT64_MAX)
        return 64;
    int clz = __builtin_clzll(addr);
    int clo = __builtin_clzll(~addr);
    return clz > clo ? clz : clo;
}

uint32_t sign_extendable_bits128(__uint128_t addr)
{
    int clz = clz_u128(addr);
    int clo = clz_u128(~addr);
    return clz > clo ? clz : clo;
}

/* Sign extend a value @p val with @p bits to 32 bits. */
uint32_t sext32(uint32_t val, uint32_t bit)
{
    if (bit == 0)
        return 0;

    if (bit == 32)
        return val;

    int m = 1U << (bit - 1);

    val = val & ((1U << bit) - 1);
    return (val ^ m) - m;
}

__uint128_t sext128(__uint128_t val, __uint128_t bit)
{
    if (bit == 0)
        return 0;

    if (bit == 128)
        return val;

    __uint128_t m = (__uint128_t)1 << (bit - 1);

    val = val & (((__uint128_t)1 << bit) - 1);
    return (val ^ m) - m;
}

int clz_u128(__uint128_t u)
{
    uint64_t hi   = u >> 64;
    uint64_t lo   = u;
    int retval[3] = {__builtin_clzll(hi), __builtin_clzll(lo) + 64, 128};
    int idx       = !hi + ((!lo) & (!hi));
    return retval[idx];
}
