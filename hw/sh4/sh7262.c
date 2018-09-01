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
#include "hw/ssi/ssi.h"
#include "sh7262_regs.h"
#include "hw/sh4/sh_intc.h"
#include "chardev/char-fe.h"

typedef struct {
  uint32_t sprx[8];
  uint32_t sptx[2];
  uint32_t shift_register;
  uint8_t pos;
  uint8_t transfer_bit_length; /* 8, 16, 32 */
} SH7262_RSPI;

typedef struct SH7262State {
    MemoryRegion bootrom;
    MemoryRegion fastram;
    MemoryRegion largeram;
    MemoryRegion largeram_1c;
    MemoryRegion largeram_3c;
    MemoryRegion peripheral;
    MemoryRegion peripheral_fffc;
    /* CPU */
    SuperHCPU *cpu;
    /* Bus, controller */
    SSIBus *spi;
    SH7262_RSPI rspi[2];
    struct intc_desc intc;
} SH7262State;

uint32_t sh7262_spdr_read(SH7262State *s, unsigned ch)
{
    uint32_t val;
    if (s->rspi[ch].pos == 0) {
        val = ssi_transfer(s->spi, 0xFF);
    }
    else {
        val = s->rspi[ch].sprx[0];
        for (int i = 0; i < s->rspi[ch].pos; i++) {
            s->rspi[ch].sprx[i] = s->rspi[ch].sprx[i+1];
        }
        s->rspi[ch].pos--;
    }
    return val;
}
void sh7262_spdr_write(SH7262State *s, unsigned ch, uint32_t val)
{
    SH7262_RSPI *rspi = &s->rspi[ch];
    rspi->sptx[0] = val; /* sptx[1] is not in use */
    rspi->shift_register = rspi->sptx[0];
    rspi->shift_register = ssi_transfer(s->spi, rspi->shift_register);
    rspi->sprx[rspi->pos] = rspi->shift_register;
    rspi->pos++;
}

static uint32_t sh7262_rspi_read(SH7262State *s, unsigned ch, unsigned ofs, unsigned size)
{
    if (size == 1) {
        switch (ofs) {
        case SH7262_SPCR_OFS:
            break;
        case SH7262_SPSR_OFS:
            return 0x80;
        case SH7262_SPDR_OFS:
            return sh7262_spdr_read(s, ch);
        }
    }
    return 0;
}

static void sh7262_rspi_write(SH7262State *s, unsigned ch, unsigned ofs,
                                  uint32_t mem_value, unsigned size)
{
    if (size == 1) {
        switch (ofs) {
        case SH7262_SPCR_OFS:
            break;
        case SH7262_SPSR_OFS:
            break;
        case SH7262_SPDR_OFS:
            sh7262_spdr_write(s, ch, mem_value);
            break;
        case SH7262_SPBFCR_OFS:
            if (mem_value & 0x40) s->rspi[ch].pos = 0;
            break;
        }
    }
    return 0;
}

static uint32_t sh7262_peripheral_read(void *opaque, hwaddr addr, unsigned size)
{
    SH7262State* s = (SH7262State*)opaque;

    if (SH7262_RSPI_BASE_CH0 <= addr && addr < (SH7262_RSPI_BASE_CH0 + SH7262_RSPI_SIZE)) {
        return sh7262_rspi_read(s, 0, addr - SH7262_RSPI_BASE_CH0, size);
    }
    if (SH7262_RSPI_BASE_CH1 <= addr && addr < (SH7262_RSPI_BASE_CH1 + SH7262_RSPI_SIZE)) {
        return sh7262_rspi_read(s, 1, addr - SH7262_RSPI_BASE_CH1, size);
    }

    return 0;
}

static void sh7262_peripheral_write(void *opaque, hwaddr addr,
                                    uint32_t mem_value, unsigned size)
{
    SH7262State* s = (SH7262State*)opaque;

    if (SH7262_RSPI_BASE_CH0 <= addr && addr < (SH7262_RSPI_BASE_CH0 + SH7262_RSPI_SIZE)) {
        sh7262_rspi_write(s, 0, addr - SH7262_RSPI_BASE_CH0, mem_value, size);
        return;
    }
    if (SH7262_RSPI_BASE_CH1 <= addr && addr < (SH7262_RSPI_BASE_CH1 + SH7262_RSPI_SIZE)) {
        sh7262_rspi_write(s, 1, addr - SH7262_RSPI_BASE_CH1, mem_value, size);
        return;
    }
}

static const MemoryRegionOps sh7262_peripheral_ops = {
    .read = sh7262_peripheral_read,
    .write = sh7262_peripheral_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

enum {
	UNUSED = 0,

	/* interrupt sources */
	IRQ0, IRQ1, IRQ2, IRQ3,
	IRQ4, IRQ5, IRQ6, IRQ7,

	/* interrupt groups */

	NR_SOURCES,
};

static struct intc_vect vectors[] = {
	INTC_VECT(IRQ0, 64), INTC_VECT(IRQ1, 65),
	INTC_VECT(IRQ2, 66), INTC_VECT(IRQ3, 67),
	INTC_VECT(IRQ4, 68), INTC_VECT(IRQ5, 69),
	INTC_VECT(IRQ6, 70), INTC_VECT(IRQ7, 71),
};

static struct intc_group groups[] = {
};

static struct intc_prio_reg prio_registers[] = {
};

static struct intc_mask_reg mask_registers[] = {
};

typedef struct {
    CharBackend chr;

    qemu_irq irq0;
    qemu_irq irq1;
    qemu_irq irq2;
    qemu_irq irq3;
    qemu_irq irq4;
    qemu_irq irq5;
    qemu_irq irq6;
    qemu_irq irq7;
} sh7262_irq_state;

static int sh7262_irq_can_receive1(void *opaque)
{
    return 8;
}

static void sh7262_irq_receive1(void *opaque, const uint8_t *buf, int size)
{
    sh7262_irq_state *s = opaque;

    if (size>4)
    {
        if (memcmp(buf,"irq0",4)==0) {
            qemu_set_irq(s->irq0, 1);
        } else if (memcmp(buf,"irq1",4)==0) {
            qemu_set_irq(s->irq1, 1);
        } else if (memcmp(buf,"irq2",4)==0) {
            qemu_set_irq(s->irq2, 1);
        } else if (memcmp(buf,"irq3",4)==0) {
            qemu_set_irq(s->irq3, 1);
        } else if (memcmp(buf,"irq4",4)==0) {
            qemu_set_irq(s->irq4, 1);
        } else if (memcmp(buf,"irq5",4)==0) {
            qemu_set_irq(s->irq5, 1);
        } else if (memcmp(buf,"irq6",4)==0) {
            qemu_set_irq(s->irq6, 1);
        } else if (memcmp(buf,"irq7",4)==0) {
            qemu_set_irq(s->irq7, 1);
        }
    }
}

void sh7262_irq_init(Chardev *chr,
                    qemu_irq irq0_source,
                    qemu_irq irq1_source,
                    qemu_irq irq2_source,
                    qemu_irq irq3_source,
                    qemu_irq irq4_source,
                    qemu_irq irq5_source,
                    qemu_irq irq6_source,
                    qemu_irq irq7_source)
{
    sh7262_irq_state *s;

    s = g_malloc0(sizeof(sh7262_irq_state));

    if (chr) {
        qemu_chr_fe_init(&s->chr, chr, &error_abort);
        qemu_chr_fe_set_handlers(&s->chr, sh7262_irq_can_receive1,
                                 sh7262_irq_receive1,
                                 NULL, NULL, s, NULL, true);
    }

    s->irq0 = irq0_source;
    s->irq1 = irq1_source;
    s->irq2 = irq2_source;
    s->irq3 = irq3_source;
    s->irq4 = irq4_source;
    s->irq5 = irq5_source;
    s->irq6 = irq6_source;
    s->irq7 = irq7_source;
}

SH7262State *sh7262_init(SuperHCPU *cpu, MemoryRegion *sysmem)
{
    SH7262State *s;
    int ret;

    s = g_malloc0(sizeof(SH7262State));
    s->cpu = cpu;

    cpu->env.bn_max = 14;

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

    // Large RAM
    memory_region_init_ram(&s->largeram, NULL, "largeram", 0x00100000, &error_fatal);
    memory_region_init_alias(&s->largeram_1c, NULL, "largeram_1c", &s->largeram, 0x00000000,
                             0x00100000);
    memory_region_add_subregion(sysmem, 0x1C000000, &s->largeram_1c);
    memory_region_init_alias(&s->largeram_3c, NULL, "largeram_3c", &s->largeram, 0x00000000,
                             0x00100000);
    memory_region_add_subregion(sysmem, 0x3C000000, &s->largeram_3c);

    // Peripheral
    memory_region_init_io(&s->peripheral, NULL, &sh7262_peripheral_ops, s,
                          "peripheral", 0x100000000);
    memory_region_init_alias(&s->peripheral_fffc, NULL, "peripheral-fffc",
                             &s->peripheral, 0xFFFC0000, 0x40000);
    memory_region_add_subregion(sysmem, 0xFFFC0000, &s->peripheral_fffc);

    sh_intc_init(sysmem, &s->intc, NR_SOURCES,
		 _INTC_ARRAY(mask_registers),
		 _INTC_ARRAY(prio_registers));

    sh_intc_register_sources(&s->intc,
			     _INTC_ARRAY(vectors),
			     _INTC_ARRAY(groups));

    cpu->env.intc_handle = &s->intc;

    sh7262_irq_init(serial_hd(0),
                   s->intc.irqs[IRQ0],
                   s->intc.irqs[IRQ1],
                   s->intc.irqs[IRQ2],
                   s->intc.irqs[IRQ3],
                   s->intc.irqs[IRQ4],
                   s->intc.irqs[IRQ5],
                   s->intc.irqs[IRQ6],
                   s->intc.irqs[IRQ7]);

    // SPI bus
    s->spi = ssi_create_bus(NULL, "spi");

    return s;
}

SSIBus* sh7262_get_spi_bus(struct SH7262State *s)
{
    return s->spi;
}
