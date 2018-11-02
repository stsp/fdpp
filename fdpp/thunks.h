/*
 *  FDPP - freedos port to modern C++
 *  Copyright (C) 2018  Stas Sergeev (stsp)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef THUNKS_H
#define THUNKS_H

#include <stdint.h>
#include <stdarg.h>

#define FDPP_API_VER 11

#ifdef __cplusplus
extern "C" {
#endif

struct vm86_regs;
void FdppCall(struct vm86_regs *regs);

typedef void (*FdppAsmCall_t)(struct vm86_regs *regs, uint16_t seg,
        uint16_t off, uint8_t *sp, uint8_t len);

enum { FDPP_PRINT_LOG, FDPP_PRINT_TERMINAL, FDPP_PRINT_SCREEN };

struct fdpp_api {
    uint8_t *(*so2lin)(uint16_t seg, uint16_t off);
    void (*exit)(int rc);
    void (*abort)(const char *file, int line);
    void (*print)(int prio, const char *format, va_list ap);
    void (*debug)(const char *msg);
    void (*panic)(const char *msg);
    void (*cpu_relax)(void);
    FdppAsmCall_t asm_call;
    FdppAsmCall_t asm_call_noret;
};
int FdppInit(struct fdpp_api *api, int ver, int *req_ver);

const char *FdppDataDir(void);
const char *FdppKernelName(void);

#ifdef __cplusplus
}
#endif

#endif
