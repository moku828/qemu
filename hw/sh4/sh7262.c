/*
 * SH7262 device
 *
 * Copyright (c) 2018 Masayuki Okumura
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "qemu/osdep.h"
#include "qapi/error.h"
#include "hw/hw.h"
#include "hw/sh4/sh.h"
#include "sysemu/sysemu.h"
#include "sysemu/qtest.h"
#include "cpu.h"
#include "exec/exec-all.h"

typedef struct SH7262State {
    MemoryRegion bootrom;
    MemoryRegion fastram;
    /* CPU */
    SuperHCPU *cpu;
} SH7262State;

SH7262State *sh7262_init(SuperHCPU *cpu, MemoryRegion *sysmem)
{
    SH7262State *s;
    int ret;

    s = g_malloc0(sizeof(SH7262State));
    s->cpu = cpu;

    // Internal ROM for Boot startup
    memory_region_init_ram(&s->bootrom, NULL, "bootrom", 0x10000, &error_fatal);
    memory_region_set_readonly(&s->bootrom, true);
    memory_region_add_subregion(sysmem, 0x40000000, &s->bootrom);
    ret = load_image_mr("bootrom.bin", &s->bootrom);
    if (ret < 0 && !qtest_enabled()) {
        error_report("Could not load SH7262 bootrom '%s'", "bootrom.bin");
        exit(1);
    }

    // Fast RAM
    memory_region_init_ram(&s->fastram, NULL, "fastram", 0x10000, &error_fatal);
    memory_region_add_subregion(sysmem, 0xFFF80000, &s->fastram);

    return s;
}
