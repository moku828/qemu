/*
 * FRK-SH2A board description
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
#include "qemu-common.h"
#include "cpu.h"
#include "hw/hw.h"
#include "hw/sh4/sh.h"
#include "sysemu/sysemu.h"
#include "sysemu/qtest.h"
#include "hw/boards.h"
#include "hw/loader.h"
#include "exec/address-spaces.h"
#include "qemu/error-report.h"
#include "hw/ssi/ssi.h"

static void frksh2a_init(MachineState *machine)
{
    SuperHCPU *cpu;
    struct SH7262State *s;
    DeviceState *dev;
    DriveInfo *dinfo;
    MemoryRegion *sysmem = get_system_memory();
    
    cpu = SUPERH_CPU(cpu_create(machine->cpu_type));

    s = sh7262_init(cpu, sysmem);

    dev = ssi_create_slave_no_init(sh7262_get_spi_bus(s), "m25p05");
    dinfo = drive_get_next(IF_MTD);
    qdev_prop_set_drive(dev, "drive", blk_by_legacy_dinfo(dinfo), &error_fatal);
    qdev_init_nofail(dev);

}

static void frksh2a_machine_init(MachineClass *mc)
{
    mc->desc = "FRK-SH2A";
    mc->init = frksh2a_init;
    mc->is_default = 1;
    mc->default_cpu_type = TYPE_SH7262_CPU;
}

DEFINE_MACHINE("frksh2a", frksh2a_machine_init)
