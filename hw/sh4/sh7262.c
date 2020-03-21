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
#include "hw/ptimer.h"

typedef struct {
  uint32_t sprx[8];
  uint32_t sptx[2];
  uint32_t shift_register;
  uint8_t pos;
  uint8_t transfer_bit_length; /* 8, 16, 32 */
  uint8_t spcr;
  uint8_t sslp;
  uint8_t sppcr;
  uint8_t spsr;
  uint8_t spscr;
  uint8_t spssr;
  uint8_t spbr;
  uint8_t spdcr;
  uint8_t spckd;
  uint8_t sslnd;
  uint8_t spnd;
  uint16_t spcmd0;
  uint16_t spcmd1;
  uint16_t spcmd2;
  uint16_t spcmd3;
  uint8_t spbfcr;
  uint16_t spbfdr;
  SSIBus *spi;
} SH7262_RSPI;

typedef struct {
  uint32_t sar;
  uint32_t dar;
  uint32_t dmatcr;
  uint32_t chcr;
  uint32_t rsar;
  uint32_t rdar;
  uint32_t rdmatcr;
} SH7262_DMAC_PER_CHANNEL;

typedef struct {
  ptimer_state* pts;
  qemu_irq cmi;
  uint16_t cmcsr;
  uint16_t cmcnt;
  uint16_t cmcor;
} SH7262_CMT_PER_CHANNEL;

typedef struct {
  uint16_t cmstr;
  SH7262_CMT_PER_CHANNEL pc[2];
} SH7262_CMT;

typedef struct {
  SH7262_DMAC_PER_CHANNEL pc[16];
  uint16_t dmaor;
  uint16_t dmars[8];
} SH7262_DMAC;

typedef struct SH7262State {
    MemoryRegion bootrom;
    MemoryRegion fastram;
    MemoryRegion largeram;
    MemoryRegion largeram_1c;
    MemoryRegion largeram_3c;
    MemoryRegion peripheral;
    MemoryRegion peripheral_fffc;
    AddressSpace sysmem_as;
    uint16_t frqcr;
    uint16_t stbcr5;
    uint16_t stbcr7;
    uint16_t ipr10;
    uint16_t pccr2;
    uint16_t pccr1;
    uint16_t pcior0;
    uint16_t pcdr0;
    uint16_t pfcr3;
    uint16_t pfcr2;
    uint16_t pgcr5;
    uint16_t pgior1;
    uint16_t pgdr1;
    uint16_t pjcr0;
    uint16_t pjior0;
    uint16_t pjpr0;
    /* CPU */
    SuperHCPU *cpu;
    /* Bus, controller */
    SH7262_RSPI rspi[2];
    struct intc_desc intc;
    qemu_irq cs_lines[2];
    SH7262_DMAC dmac;
    SH7262_CMT cmt;
} SH7262State;

static uint32_t sh7262_cmt_per_channel_read(SH7262State *s, unsigned ch, unsigned ofs, unsigned size)
{
    if (size == 1) {
        switch (ofs) {
        case OFS_LB(SH7262_CMCSR_OFS): return GET_LB(s->cmt.pc[ch].cmcsr);
        default:
            abort();
        }
    } else if (size == 2) {
        switch (ofs) {
        case SH7262_CMCSR_OFS: return s->cmt.pc[ch].cmcsr;
        default:
            abort();
        }
    } else {
        abort();
    }
    return 0;
}

static void sh7262_cmt_per_channel_write(SH7262State *s, unsigned ch, unsigned ofs,
                                  uint32_t mem_value, unsigned size)
{
    if (size == 1) {
        switch (ofs) {
        case OFS_LB(SH7262_CMCSR_OFS):
            {
                bool prvmatched = (SH7262_CMCSR_CMF(s->cmt.pc[ch].cmcsr) == SH7262_CMCSR_CMF_MATCHED) ? true : false;
                s->cmt.pc[ch].cmcsr = RPL_LB(s->cmt.pc[ch].cmcsr, mem_value);
                bool newmatched = (SH7262_CMCSR_CMF(s->cmt.pc[ch].cmcsr) == SH7262_CMCSR_CMF_MATCHED) ? true : false;
                if ((!newmatched) && (prvmatched != newmatched)) {
                    qemu_set_irq(s->cmt.pc[ch].cmi, 0);
                }
            }            
            break;
        default:
            abort();
        }
    } else if (size == 2) {
        switch (ofs) {
        case SH7262_CMCSR_OFS:
            {
                bool prvmatched = (SH7262_CMCSR_CMF(s->cmt.pc[ch].cmcsr) == SH7262_CMCSR_CMF_MATCHED) ? true : false;
                s->cmt.pc[ch].cmcsr = mem_value;
                bool newmatched = (SH7262_CMCSR_CMF(s->cmt.pc[ch].cmcsr) == SH7262_CMCSR_CMF_MATCHED) ? true : false;
                if ((!newmatched) && (prvmatched != newmatched)) {
                    qemu_set_irq(s->cmt.pc[ch].cmi, 0);
                }
            }            
            break;
        case SH7262_CMCOR_OFS: s->cmt.pc[ch].cmcor = mem_value; break;
        default:
            abort();
        }
    } else {
        abort();
    }
}

static uint32_t sh7262_cmt_cmstr_read(SH7262State *s, hwaddr addr, unsigned size)
{
    return GET_REG_WORD(s->cmt.cmstr, addr, size);
}

static void sh7262_cmt_0_tick(void *opaque)
{
    SH7262State* s = (SH7262State*)opaque;
    s->cmt.pc[0].cmcsr |= 0x0080;
    if (SH7262_CMCSR_CMIE(s->cmt.pc[0].cmcsr) == SH7262_CMCSR_CMIE_PERMIT) {
        qemu_set_irq(s->cmt.pc[0].cmi, 1);
    }
}

static void sh7262_cmt_1_tick(void *opaque)
{
    SH7262State* s = (SH7262State*)opaque;
    s->cmt.pc[1].cmcsr |= 0x0080;
    if (SH7262_CMCSR_CMIE(s->cmt.pc[1].cmcsr) == SH7262_CMCSR_CMIE_PERMIT) {
        qemu_set_irq(s->cmt.pc[1].cmi, 1);
    }
}

static void sh7262_cmt_init(SH7262State *s, unsigned ch, qemu_irq cmi)
{
    QEMUBH* bh;
    bh = qemu_bh_new(ch == 0 ? sh7262_cmt_0_tick : sh7262_cmt_1_tick, s);
    s->cmt.pc[ch].pts = ptimer_init(bh, PTIMER_POLICY_DEFAULT);
    s->cmt.pc[ch].cmi = cmi;
}

static void sh7262_cmt_cmstr_write(SH7262State *s, hwaddr addr,
                                  uint32_t mem_value, unsigned size)
{
    bool prvstp[2];
    bool newstp[2];
    prvstp[0] = SH7262_CMSTR_STR0(s->cmt.cmstr) == SH7262_CMSTR_STR0_CNTSTP ? true : false;
    prvstp[1] = SH7262_CMSTR_STR1(s->cmt.cmstr) == SH7262_CMSTR_STR1_CNTSTP ? true : false;
    SET_REG_WORD(s->cmt.cmstr, addr, mem_value, size);
    newstp[0] = SH7262_CMSTR_STR0(s->cmt.cmstr) == SH7262_CMSTR_STR0_CNTSTP ? true : false;
    newstp[1] = SH7262_CMSTR_STR1(s->cmt.cmstr) == SH7262_CMSTR_STR1_CNTSTP ? true : false;
    for (int i = 0; i < 2; i++) {
        if (prvstp[i] != newstp[i]) {
            if (newstp[i]) {
                ptimer_stop(s->cmt.pc[i].pts);
            } else {
                int cks = 0;
                switch (SH7262_CMCSR_CKS(s->cmt.pc[i].cmcsr)) {
                case SH7262_CMCSR_CKS_PCLKDIV8: cks = 8; break;
                case SH7262_CMCSR_CKS_PCLKDIV32: cks = 32; break;
                case SH7262_CMCSR_CKS_PCLKDIV128: cks = 128; break;
                case SH7262_CMCSR_CKS_PCLKDIV512: cks = 512; break;
                }
                int stc = 0;
                switch (SH7262_FRQCR_STC(s->frqcr)) {
                case SH7262_FRQCR_STC_MUL8: stc = 8; break;
                case SH7262_FRQCR_STC_MUL12: stc = 12; break;
                }
                int pfc = 0;
                switch (SH7262_FRQCR_PFC(s->frqcr)) {
                case SH7262_FRQCR_PFC_DIV4: pfc = 4; break;
                case SH7262_FRQCR_PFC_DIV6: pfc = 6; break;
                case SH7262_FRQCR_PFC_DIV8: pfc = 8; break;
                case SH7262_FRQCR_PFC_DIV12: pfc = 12; break;
                }
                int pclk = ((12 * stc) / pfc) * 1000000;
                ptimer_set_limit(s->cmt.pc[i].pts, s->cmt.pc[i].cmcor, 0);
                s->cmt.pc[i].cmcnt = 0;
                ptimer_set_count(s->cmt.pc[i].pts, s->cmt.pc[i].cmcnt);
                ptimer_set_freq(s->cmt.pc[i].pts, pclk / cks);
                ptimer_run(s->cmt.pc[i].pts, 0);
            }
        }
    }
}

static uint32_t sh7262_cmt_read(SH7262State *s, hwaddr addr, unsigned size)
{
    if (addr >= SH7262_CMT_BASE_CH1) {
        return sh7262_cmt_per_channel_read(s, 1, addr - SH7262_CMT_BASE_CH1, size);
    } else if (addr >= SH7262_CMT_BASE_CH0) {
        return sh7262_cmt_per_channel_read(s, 0, addr - SH7262_CMT_BASE_CH0, size);
    } else {
        return sh7262_cmt_cmstr_read(s, addr, size);
    }
}

static void sh7262_cmt_write(SH7262State *s, hwaddr addr,
                                  uint32_t mem_value, unsigned size)
{
    if (addr >= SH7262_CMT_BASE_CH1) {
        sh7262_cmt_per_channel_write(s, 1, addr - SH7262_CMT_BASE_CH1, mem_value, size);
    } else if (addr >= SH7262_CMT_BASE_CH0) {
        sh7262_cmt_per_channel_write(s, 0, addr - SH7262_CMT_BASE_CH0, mem_value, size);
    } else {
        sh7262_cmt_cmstr_write(s, addr, mem_value, size);
    }
}

static void sh7262_dma_transfer(SH7262State *s, unsigned ch)
{
    int ts = 0, dm = 0, sm = 0;
    switch (SH7262_CHCR_TS(s->dmac.pc[ch].chcr)) {
    case SH7262_CHCR_TS_BYTE: ts = 1; break;
    case SH7262_CHCR_TS_WORD: ts = 2; break;
    case SH7262_CHCR_TS_LONGWORD: ts = 4; break;
    case SH7262_CHCR_TS_16BYTE: ts = 16; break;
    }
    switch (SH7262_CHCR_DM(s->dmac.pc[ch].chcr)) {
    case SH7262_CHCR_DM_INC: dm = 1; break;
    case SH7262_CHCR_DM_DEC: dm = -1; break;
    }
    switch (SH7262_CHCR_SM(s->dmac.pc[ch].chcr)) {
    case SH7262_CHCR_SM_INC: sm = 1; break;
    case SH7262_CHCR_SM_DEC: sm = -1; break;
    }
    for (int i = 0, si = 0, di = 0; i < ts * s->dmac.pc[ch].dmatcr; i++) {
        stb_phys(&s->sysmem_as, s->dmac.pc[ch].dar + di, ldub_phys(&s->sysmem_as, s->dmac.pc[ch].sar + si));
        di += dm;
        si += sm;
    }
    s->dmac.pc[ch].dmatcr = 0;
    s->dmac.pc[ch].chcr |= 0x0002;
}

static uint32_t sh7262_dmac_per_channel_read(SH7262State *s, unsigned ch, unsigned ofs, unsigned size)
{
    if (size == 1) {
        switch (ofs) {
        case OFS_4B(SH7262_CHCR_OFS): return GET_4B(s->dmac.pc[ch].chcr);
        default:
            abort();
        }
    } else {
        abort();
    }
    return 0;
}

static void sh7262_dmac_per_channel_write(SH7262State *s, unsigned ch, unsigned ofs,
                                  uint32_t mem_value, unsigned size)
{
    if (size == 4) {
        switch (ofs) {
        case SH7262_SAR_OFS: s->dmac.pc[ch].sar = mem_value; break;
        case SH7262_DAR_OFS: s->dmac.pc[ch].dar = mem_value; break;
        case SH7262_DMATCR_OFS: s->dmac.pc[ch].dmatcr = mem_value; break;
        case SH7262_CHCR_OFS:
            s->dmac.pc[ch].chcr = mem_value;
            if (SH7262_CHCR_DE(s->dmac.pc[ch].chcr) == SH7262_CHCR_DE_PERMIT) {
                int do_tranfer = 0;
                switch (SH7262_DMARS_MID(SH7262_DMARS_CH(s->dmac.dmars[ch >> 1], ch))) {
                case SH7262_DMARS_MID_RSPI_CH0:
                    if (SH7262_SPDCR_TXDMY(s->rspi[0].spdcr) == SH7262_SPDCR_TXDMY_PERMIT) do_tranfer = 1;
                    break;
                case SH7262_DMARS_MID_RSPI_CH1:
                    if (SH7262_SPDCR_TXDMY(s->rspi[1].spdcr) == SH7262_SPDCR_TXDMY_PERMIT) do_tranfer = 1;
                    break;
                }
                if (do_tranfer) sh7262_dma_transfer(s, ch);
            }
            break;
        default:
            abort();
        }
    } else {
        abort();
    }
}

static uint32_t sh7262_dmac_dmaor_read(SH7262State *s, hwaddr addr, unsigned size)
{
    return GET_REG_WORD(s->dmac.dmaor, addr, size);
}

static void sh7262_dmac_dmaor_write(SH7262State *s, hwaddr addr,
                                  uint32_t mem_value, unsigned size)
{
    SET_REG_WORD(s->dmac.dmaor, addr, mem_value, size);
}

static uint32_t sh7262_dmac_dmars_read(SH7262State *s, hwaddr addr, unsigned size)
{
    return GET_REG_WORD(s->dmac.dmars[(addr >> 2) & 0x07], addr, size);
}

static void sh7262_dmac_dmars_write(SH7262State *s, hwaddr addr,
                                  uint32_t mem_value, unsigned size)
{
    SET_REG_WORD(s->dmac.dmars[(addr >> 2) & 0x07], addr, mem_value, size);
}

static uint32_t sh7262_dmac_read(SH7262State *s, hwaddr addr, unsigned size)
{
    if (addr < SH7262_DMAC_DMAOR) {
        return sh7262_dmac_per_channel_read(s, (addr >> 4) & 0x0F, addr & 0x0F0F, size);
    } else if (addr >= SH7262_DMAC_DMARS0) {
        return sh7262_dmac_dmars_read(s, addr, size);
    } else {
        return sh7262_dmac_dmaor_read(s, addr, size);
    }
}

static void sh7262_dmac_write(SH7262State *s, hwaddr addr,
                                  uint32_t mem_value, unsigned size)
{
    if (addr < SH7262_DMAC_DMAOR) {
        sh7262_dmac_per_channel_write(s, (addr >> 4) & 0x0F, addr & 0x0F0F, mem_value, size);
    } else if (addr >= SH7262_DMAC_DMARS0) {
        sh7262_dmac_dmars_write(s, addr, mem_value, size);
    } else {
        sh7262_dmac_dmaor_write(s, addr, mem_value, size);
    }
}

uint32_t sh7262_spdr_read(SH7262State *s, unsigned ch)
{
    uint32_t val;
    if (s->rspi[ch].pos == 0) {
        if (SH7262_SPDCR_TXDMY(s->rspi[ch].spdcr) == SH7262_SPDCR_TXDMY_PERMIT) {
            val = ssi_transfer(s->rspi[ch].spi, 0xCD);
        }
        else {
            val = 0xCD;
        }
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
    rspi->spsr &= 0x40;
    rspi->shift_register = ssi_transfer(s->rspi[ch].spi, rspi->shift_register);
    rspi->spsr |= 0x40;
    rspi->sprx[rspi->pos] = rspi->shift_register;
    rspi->pos++;
}

static void sh7262_rspi_init(SH7262State *s, unsigned ch)
{
    SH7262_RSPI *rspi = &s->rspi[ch];
    char name[16];
    sprintf(name, "spi%d", ch);
    rspi->spi = ssi_create_bus(NULL, name);
    rspi->spsr = 0x60;
}

static uint32_t sh7262_rspi_read(SH7262State *s, unsigned ch, unsigned ofs, unsigned size)
{
    if (size == 1) {
        switch (ofs) {
        case SH7262_SPCR_OFS:
            return s->rspi[ch].spcr;
        case SH7262_SPSR_OFS:
            return s->rspi[ch].spsr | 0x80;
        case SH7262_SPDR_OFS:
            return sh7262_spdr_read(s, ch);
        case SH7262_SPDCR_OFS:
            return s->rspi[ch].spdcr;
        case SH7262_SPBFCR_OFS:
            return s->rspi[ch].spbfcr;
        }
    } else if (size == 2) {
        switch (ofs) {
        case SH7262_SPCMD0_OFS:
            return s->rspi[ch].spcmd0;
        }
    } else {
        abort();
    }
    return 0;
}

static void sh7262_rspi_write(SH7262State *s, unsigned ch, unsigned ofs,
                                  uint32_t mem_value, unsigned size)
{
    if (size == 1) {
        switch (ofs) {
        case SH7262_SPCR_OFS:
            s->rspi[ch].spcr = mem_value;
            if (ch == 0) {
                qemu_set_irq(s->cs_lines[0], ((SH7262_SPCR_SPE(s->rspi[0].spcr) == SH7262_SPCR_SPE_ENABLE) && (SH7262_PFCR2_PF10MD(s->pfcr2) == SH7262_PFCR2_PF10MD_SSL00)) ? 0 : 1);
                qemu_set_irq(s->cs_lines[1], ((SH7262_SPCR_SPE(s->rspi[0].spcr) == SH7262_SPCR_SPE_ENABLE) && (SH7262_PGCR5_PG20MD(s->pgcr5) == SH7262_PGCR5_PG20MD_PG20) && (SH7262_PGIOR1_PG20IOR(s->pgior1) == 1) && (SH7262_PGDR1_PG20DR(s->pgdr1) == 0)) ? 0 : 1);
            }
            break;
        case SH7262_SSLP_OFS:
            s->rspi[ch].sslp = mem_value;
            break;
        case SH7262_SPPCR_OFS:
            s->rspi[ch].sppcr = mem_value;
            break;
        case SH7262_SPSR_OFS:
            s->rspi[ch].spsr = mem_value;
            break;
        case SH7262_SPDR_OFS:
            sh7262_spdr_write(s, ch, mem_value);
            break;
        case SH7262_SPBR_OFS:
            s->rspi[ch].spbr = mem_value;
            break;
        case SH7262_SPDCR_OFS:
            s->rspi[ch].spdcr = mem_value;
            if (SH7262_SPDCR_TXDMY(s->rspi[ch].spdcr) == SH7262_SPDCR_TXDMY_PERMIT) {
                int do_tranfer = 0;
                int mid = (ch == 0) ? SH7262_DMARS_MID_RSPI_CH0 : SH7262_DMARS_MID_RSPI_CH1;
                for (int i = 0; i < 16; i++) {
                    if ((SH7262_DMARS_MID(SH7262_DMARS_CH(s->dmac.dmars[i >> 1], i)) == mid) && (SH7262_CHCR_DE(s->dmac.pc[i].chcr) == SH7262_CHCR_DE_PERMIT)) {
                        do_tranfer = 1;
                        break;
                    }
                }
                if (do_tranfer) sh7262_dma_transfer(s, ch);
            }
            break;
        case SH7262_SPSCR_OFS:
            s->rspi[ch].spscr = mem_value;
            break;
        case SH7262_SPCKD_OFS:
            s->rspi[ch].spckd = mem_value;
            break;
        case SH7262_SSLND_OFS:
            s->rspi[ch].sslnd = mem_value;
            break;
        case SH7262_SPND_OFS:
            s->rspi[ch].spnd = mem_value;
            break;
        case SH7262_SPBFCR_OFS:
            s->rspi[ch].spbfcr = mem_value;
            if (SH7262_SPBFCR_RXRST(s->rspi[ch].spbfcr) == SH7262_SPBFCR_RXRST_PERMIT) s->rspi[ch].pos = 0;
            break;
        default:
            abort();
        }
    } else if (size == 2) {
        switch (ofs) {
        case SH7262_SPCMD0_OFS:
            s->rspi[ch].spcmd0 = mem_value;
            break;
        default:
            abort();
        }
    } else {
        abort();
    }
}

static uint32_t sh7262_peripheral_read(void *opaque, hwaddr addr, unsigned size)
{
    SH7262State* s = (SH7262State*)opaque;

    if (SH7262_RSPI_BASE_CH0 <= addr && addr < (SH7262_RSPI_BASE_CH0 + SH7262_RSPI_SIZE)) {
        return sh7262_rspi_read(s, 0, addr - SH7262_RSPI_BASE_CH0, size);
    } else if (SH7262_RSPI_BASE_CH1 <= addr && addr < (SH7262_RSPI_BASE_CH1 + SH7262_RSPI_SIZE)) {
        return sh7262_rspi_read(s, 1, addr - SH7262_RSPI_BASE_CH1, size);
    } else if (SH7262_DMAC_BASE <= addr && addr < (SH7262_DMAC_BASE + SH7262_DMAC_SIZE)) {
        return sh7262_dmac_read(s, addr, size);
    } else if (SH7262_CMT_CMSTR <= addr && addr < (SH7262_CMT_CMSTR + SH7262_CMT_SIZE)) {
        return sh7262_cmt_read(s, addr, size);
    } else if (size == 1) {
        switch (addr) {
        case SH7262_FRQCR_LB:
            return GET_LB(s->frqcr);
        case SH7262_IPR10_LB:
            return GET_LB(s->ipr10);
        case SH7262_PCCR2_UB:
            return GET_UB(s->pccr2);
        case SH7262_PCCR2_LB:
            return GET_LB(s->pccr2);
        case SH7262_PCCR1_UB:
            return GET_UB(s->pccr1);
        case SH7262_PCCR1_LB:
            return GET_LB(s->pccr1);
        case SH7262_PCIOR0_UB:
            return GET_UB(s->pcior0);
        case SH7262_PCDR0_UB:
            return GET_UB(s->pcdr0);
        case SH7262_PFCR3_LB:
            return GET_LB(s->pfcr3);
        case SH7262_PFCR2_UB:
            return GET_UB(s->pfcr2);
        case SH7262_PFCR2_LB:
            return GET_LB(s->pfcr2);
        case SH7262_PGDR1_LB:
            return GET_LB(s->pgdr1);
        case SH7262_STBCR5:
            return s->stbcr5;
        case SH7262_STBCR7:
            return s->stbcr7;
        default:
            abort();
        }
    } else if (size == 2) {
        switch (addr) {
        case SH7262_FRQCR:
            return s->frqcr;
        case SH7262_IPR10:
            return s->ipr10;
        case SH7262_ICR0:
            return 0x0000;
        case SH7262_PCIOR0:
            return s->pcior0;
        case SH7262_PCCR2:
            return s->pccr2;
        case SH7262_PCCR1:
            return s->pccr1;
        case SH7262_PFCR2:
            return s->pfcr2;
        case SH7262_PGCR5:
            return s->pgcr5;
        case SH7262_PGIOR1:
            return s->pgior1;
        case SH7262_PJCR0:
            return s->pjcr0;
        case SH7262_PJIOR0:
            return s->pjior0;
        case SH7262_PJPR0:
            return s->pjpr0;
        default:
            abort();
        }
    } else {
        abort();
    }

    return 0;
}

static void sh7262_peripheral_write(void *opaque, hwaddr addr,
                                    uint32_t mem_value, unsigned size)
{
    SH7262State* s = (SH7262State*)opaque;

    if (SH7262_RSPI_BASE_CH0 <= addr && addr < (SH7262_RSPI_BASE_CH0 + SH7262_RSPI_SIZE)) {
        sh7262_rspi_write(s, 0, addr - SH7262_RSPI_BASE_CH0, mem_value, size);
    } else if (SH7262_RSPI_BASE_CH1 <= addr && addr < (SH7262_RSPI_BASE_CH1 + SH7262_RSPI_SIZE)) {
        sh7262_rspi_write(s, 1, addr - SH7262_RSPI_BASE_CH1, mem_value, size);
    } else if (SH7262_DMAC_BASE <= addr && addr < (SH7262_DMAC_BASE + SH7262_DMAC_SIZE)) {
        sh7262_dmac_write(s, addr, mem_value, size);
    } else if (SH7262_CMT_CMSTR <= addr && addr < (SH7262_CMT_CMSTR + SH7262_CMT_SIZE)) {
        sh7262_cmt_write(s, addr, mem_value, size);
    } else if (size == 1) {
        switch (addr) {
        case SH7262_FRQCR_LB:
            s->frqcr = (s->frqcr & 0xff00) | (mem_value << 0);
            break;
        case SH7262_IPR10_LB:
            s->ipr10 = (s->ipr10 & 0xff00) | (mem_value << 0);
            break;
        case SH7262_PCCR2_UB:
            s->pccr2 = (s->pccr2 & 0x00ff) | (mem_value << 8);
            break;
        case SH7262_PCCR2_LB:
            s->pccr2 = (s->pccr2 & 0xff00) | (mem_value << 0);
            break;
        case SH7262_PCCR1_UB:
            s->pccr1 = (s->pccr1 & 0x00ff) | (mem_value << 8);
            break;
        case SH7262_PCCR1_LB:
            s->pccr1 = (s->pccr1 & 0xff00) | (mem_value << 0);
            break;
        case SH7262_PCIOR0_UB:
            s->pcior0 = (s->pcior0 & 0x00ff) | (mem_value << 8);
            break;
        case SH7262_PCDR0_UB:
            s->pcdr0 = (s->pcdr0 & 0x00ff) | (mem_value << 8);
            break;
        case SH7262_PFCR3_LB:
            s->pfcr3 = (s->pfcr3 & 0xff00) | (mem_value << 0);
            break;
        case SH7262_PFCR2_UB:
            s->pfcr2 = (s->pfcr2 & 0x00ff) | (mem_value << 8);
            qemu_set_irq(s->cs_lines[0], ((SH7262_SPCR_SPE(s->rspi[0].spcr) == SH7262_SPCR_SPE_ENABLE) && (SH7262_PFCR2_PF10MD(s->pfcr2) == SH7262_PFCR2_PF10MD_SSL00)) ? 0 : 1);
            break;
        case SH7262_PFCR2_LB:
            s->pfcr2 = (s->pfcr2 & 0xff00) | (mem_value << 0);
            break;
        case SH7262_PGDR1_LB:
            s->pgdr1 = (s->pgdr1 & 0xff00) | (mem_value << 0);
            qemu_set_irq(s->cs_lines[1], ((SH7262_SPCR_SPE(s->rspi[0].spcr) == SH7262_SPCR_SPE_ENABLE) && (SH7262_PGCR5_PG20MD(s->pgcr5) == SH7262_PGCR5_PG20MD_PG20) && (SH7262_PGIOR1_PG20IOR(s->pgior1) == 1) && (SH7262_PGDR1_PG20DR(s->pgdr1) == 0)) ? 0 : 1);
            break;
        case SH7262_STBCR5:
            s->stbcr5 = mem_value;
            break;
        case SH7262_STBCR7:
            s->stbcr7 = mem_value;
            break;
        default:
            abort();
        }
    } else if (size == 2) {
        switch (addr) {
        case SH7262_FRQCR:
            s->frqcr = mem_value;
            break;
        case SH7262_IPR10:
            s->ipr10 = mem_value;
            break;
        case SH7262_PCIOR0:
            s->pcior0 = mem_value;
            break;
        case SH7262_PCCR2:
            s->pccr2 = mem_value;
            break;
        case SH7262_PCCR1:
            s->pccr1 = mem_value;
            break;
        case SH7262_PFCR3:
            s->pfcr3 = mem_value;
            break;
        case SH7262_PFCR2:
            s->pfcr2 = mem_value;
            qemu_set_irq(s->cs_lines[0], ((SH7262_SPCR_SPE(s->rspi[0].spcr) == SH7262_SPCR_SPE_ENABLE) && (SH7262_PFCR2_PF10MD(s->pfcr2) == SH7262_PFCR2_PF10MD_SSL00)) ? 0 : 1);
            break;
        case SH7262_PGCR5:
            s->pgcr5 = mem_value;
            break;
        case SH7262_PGIOR1:
            s->pgior1 = mem_value;
            break;
        case SH7262_PGDR1:
            s->pgdr1 = mem_value;
            qemu_set_irq(s->cs_lines[1], ((SH7262_SPCR_SPE(s->rspi[0].spcr) == SH7262_SPCR_SPE_ENABLE) && (SH7262_PGCR5_PG20MD(s->pgcr5) == SH7262_PGCR5_PG20MD_PG20) && (SH7262_PGIOR1_PG20IOR(s->pgior1) == 1) && (SH7262_PGDR1_PG20DR(s->pgdr1) == 0)) ? 0 : 1);
            break;
        case SH7262_PJCR0:
            s->pjcr0 = mem_value;
            break;
        case SH7262_PJIOR0:
            s->pjior0 = mem_value;
            break;
        default:
            abort();
        }
    } else {
        abort();
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
    CMI0, CMI1,

	/* interrupt groups */

	NR_SOURCES,
};

static struct intc_vect vectors[] = {
	INTC_VECT(IRQ0, 64), INTC_VECT(IRQ1, 65),
	INTC_VECT(IRQ2, 66), INTC_VECT(IRQ3, 67),
	INTC_VECT(IRQ4, 68), INTC_VECT(IRQ5, 69),
	INTC_VECT(IRQ6, 70), INTC_VECT(IRQ7, 71),
	INTC_VECT(CMI0, 175), INTC_VECT(CMI1, 176),
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

    s->frqcr = 0x0124; // Mode 0, 1
    //s->frqcr = 0x0013; // Mode 2, 3
    s->pjpr0 = 0x0008; // PJ1:CD = L(SD card is available), PJ3:WP = H(Protected)

    // Internal ROM for Boot startup
    memory_region_init_ram(&s->bootrom, NULL, "bootrom", 0x10000, &error_fatal);
    memory_region_set_readonly(&s->bootrom, true);
    memory_region_add_subregion(sysmem, 0xA0000000, &s->bootrom);
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
    address_space_init(&s->sysmem_as, sysmem, "sysmem-as");

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
    sh7262_rspi_init(s, 0);
    sh7262_rspi_init(s, 1);

    // Compare match timer
    sh7262_cmt_init(s, 0, s->intc.irqs[CMI0]);
    sh7262_cmt_init(s, 1, s->intc.irqs[CMI1]);

    return s;
}

SSIBus* sh7262_get_spi_bus(struct SH7262State *s, unsigned ch)
{
    return s->rspi[ch].spi;
}

int sh7262_register_spi_cs_line(struct SH7262State *s, int n, qemu_irq cs_line)
{
    s->cs_lines[n] = cs_line;
    qemu_set_irq(s->cs_lines[n], 1);
    return 0;
}
