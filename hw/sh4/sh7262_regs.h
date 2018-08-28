#ifndef _SH7262_REGS_H_
#define _SH7262_REGS_H_

#define SH7262_RSPI_SIZE 0x00000800
#define SH7262_RSPI_BASE_CH0 0xFFFF8000
#define SH7262_RSPI_BASE_CH1 0xFFFF8800
#define SH7262_SPCR_OFS 0x00
#define SH7262_SPSR_OFS 0x03
#define SH7262_SPDR_OFS 0x04
#define SH7262_SPBFCR_OFS 0x20
#define SH7262_SPCR_0 (SH7262_RSPI_BASE_CH0|SH7262_SPCR_OFS)
#define SH7262_SPSR_0 (SH7262_RSPI_BASE_CH0|SH7262_SPSR_OFS)
#define SH7262_SPDR_0 (SH7262_RSPI_BASE_CH0|SH7262_SPDR_OFS)
#define SH7262_SPBFCR_0 (SH7262_RSPI_BASE_CH0|SH7262_SPBFCR_OFS)
#define SH7262_SPCR_1 (SH7262_RSPI_BASE_CH1|SH7262_SPCR_OFS)
#define SH7262_SPSR_1 (SH7262_RSPI_BASE_CH1|SH7262_SPSR_OFS)
#define SH7262_SPDR_1 (SH7262_RSPI_BASE_CH1|SH7262_SPDR_OFS)
#define SH7262_SPBFCR_1 (SH7262_RSPI_BASE_CH1|SH7262_SPBFCR_OFS)

#endif // _SH7262_REGS_H_