#ifndef _SH7262_REGS_H_
#define _SH7262_REGS_H_

#define GET_UW(reg) ((reg >> 16) & 0x0000FFFF)
#define GET_LW(reg) ((reg >>  0) & 0x0000FFFF)
#define GET_1B(reg) ((reg >> 24) & 0x00FF)
#define GET_2B(reg) ((reg >> 16) & 0x00FF)
#define GET_3B(reg) ((reg >>  8) & 0x00FF)
#define GET_4B(reg) ((reg >>  0) & 0x00FF)
#define GET_UB(reg) ((reg >>  8) & 0x00FF)
#define GET_LB(reg) ((reg >>  0) & 0x00FF)
#define RPL_UB(reg, mem_value) ((reg & 0x00FF) | (mem_value <<  8))
#define RPL_LB(reg, mem_value) ((reg & 0xFF00) | (mem_value <<  0))
#define OFS_UW(reg) (reg + 0x00)
#define OFS_LW(reg) (reg + 0x02)
#define OFS_1B(reg) (reg + 0x00)
#define OFS_2B(reg) (reg + 0x01)
#define OFS_3B(reg) (reg + 0x02)
#define OFS_4B(reg) (reg + 0x03)
#define OFS_UB(reg) (reg + 0x00)
#define OFS_LB(reg) (reg + 0x01)
#define GET_REG_WORD(reg, addr, size) ( \
  (size == 1) ? ( \
    ((addr & 0x01) == 0x00) ? \
      GET_UB(reg) : \
      GET_LB(reg) \
  ) : \
    reg \
  )
#define SET_REG_WORD(reg, addr, mem_value, size) reg = ( \
  (size == 1) ? ( \
    ((addr & 0x01) == 0x00) ? \
      RPL_UB(reg, mem_value) : \
      RPL_LB(reg, mem_value) \
  ) : \
    mem_value \
  )

#define SH7262_RSPI_SIZE 0x00000800
#define SH7262_RSPI_BASE_CH0 0xFFFF8000
#define SH7262_RSPI_BASE_CH1 0xFFFF8800
#define SH7262_SPCR_OFS 0x00
#define SH7262_SSLP_OFS 0x01
#define SH7262_SPPCR_OFS 0x02
#define SH7262_SPSR_OFS 0x03
#define SH7262_SPDR_OFS 0x04
#define SH7262_SPSCR_OFS 0x08
#define SH7262_SPSSR_OFS 0x09
#define SH7262_SPBR_OFS 0x0A
#define SH7262_SPDCR_OFS 0x0B
#define SH7262_SPCKD_OFS 0x0C
#define SH7262_SSLND_OFS 0x0D
#define SH7262_SPND_OFS 0x0E
#define SH7262_SPCMD0_OFS 0x10
#define SH7262_SPCMD1_OFS 0x12
#define SH7262_SPCMD2_OFS 0x14
#define SH7262_SPCMD3_OFS 0x16
#define SH7262_SPBFCR_OFS 0x20
#define SH7262_SPBFDR_OFS 0x22
#define SH7262_SPCR_0 (SH7262_RSPI_BASE_CH0|SH7262_SPCR_OFS)
#define SH7262_SPSR_0 (SH7262_RSPI_BASE_CH0|SH7262_SPSR_OFS)
#define SH7262_SPDR_0 (SH7262_RSPI_BASE_CH0|SH7262_SPDR_OFS)
#define SH7262_SPDCR_0 (SH7262_RSPI_BASE_CH0|SH7262_SPDCR_OFS)
#define SH7262_SPCMD_00 (SH7262_RSPI_BASE_CH0|SH7262_SPCMD0_OFS)
#define SH7262_SPBFCR_0 (SH7262_RSPI_BASE_CH0|SH7262_SPBFCR_OFS)
#define SH7262_SPCR_1 (SH7262_RSPI_BASE_CH1|SH7262_SPCR_OFS)
#define SH7262_SPSR_1 (SH7262_RSPI_BASE_CH1|SH7262_SPSR_OFS)
#define SH7262_SPDR_1 (SH7262_RSPI_BASE_CH1|SH7262_SPDR_OFS)
#define SH7262_SPDCR_1 (SH7262_RSPI_BASE_CH1|SH7262_SPDCR_OFS)
#define SH7262_SPCMD_10 (SH7262_RSPI_BASE_CH1|SH7262_SPCMD0_OFS)
#define SH7262_SPBFCR_1 (SH7262_RSPI_BASE_CH1|SH7262_SPBFCR_OFS)
#define SH7262_SPCR_SPE(reg) ((reg >> 6) & 0x01)
#define SH7262_SPCR_SPE_DISABLE 0
#define SH7262_SPCR_SPE_ENABLE 1
#define SH7262_SPDCR_TXDMY(reg) ((reg >> 7) & 0x01)
#define SH7262_SPDCR_TXDMY_PROHIBIT 0
#define SH7262_SPDCR_TXDMY_PERMIT 1
#define SH7262_SPCMD_CPOL(reg) ((reg >> 1) & 0x01)
#define SH7262_SPCMD_CPOL_RSPCK_ON_IDLE_IS_0 0
#define SH7262_SPCMD_CPOL_RSPCK_ON_IDLE_IS_1 1
#define SH7262_SPCMD_CPHA(reg) ((reg >> 0) & 0x01)
#define SH7262_SPCMD_CPHA_SMP_ON_ODD_EDGE_CHG_ON_EVEN_EDGE 0
#define SH7262_SPCMD_CPHA_CHG_ON_ODD_EDGE_SMP_ON_EVEN_EDGE 1
#define SH7262_SPBFCR_TXRST(reg) ((reg >> 7) & 0x01)
#define SH7262_SPBFCR_TXRST_PROHIBIT 0
#define SH7262_SPBFCR_TXRST_PERMIT 1
#define SH7262_SPBFCR_RXRST(reg) ((reg >> 6) & 0x01)
#define SH7262_SPBFCR_RXRST_PROHIBIT 0
#define SH7262_SPBFCR_RXRST_PERMIT 1

#define SH7262_PFCR2 0xFFFE38AA
#define SH7262_PFCR2_UB 0xFFFE38AA
#define SH7262_PFCR2_LB 0xFFFE38AB
#define SH7262_PFCR2_PF10MD(reg) ((reg >> 8) & 0x07)
#define SH7262_PFCR2_PF10MD_PF10 0
#define SH7262_PFCR2_PF10MD_A24 1
#define SH7262_PFCR2_PF10MD_SSIWS3 2
#define SH7262_PFCR2_PF10MD_SSL00 3
#define SH7262_PFCR2_PF10MD_TIOC3B 4
#define SH7262_PFCR2_PF10MD_nFCE 5

#define SH7262_DMAC_SIZE 0x00000320
#define SH7262_DMAC_BASE 0xFFFE1000
#define SH7262_DMAC_DMAOR 0xFFFE1200
#define SH7262_DMAC_DMARS0 0xFFFE1300
#define SH7262_DMAC_DMARS1 0xFFFE1304
#define SH7262_DMAC_DMARS2 0xFFFE1308
#define SH7262_DMAC_DMARS3 0xFFFE130C
#define SH7262_DMAC_DMARS4 0xFFFE1310
#define SH7262_DMAC_DMARS5 0xFFFE1314
#define SH7262_DMAC_DMARS6 0xFFFE1318
#define SH7262_DMAC_DMARS7 0xFFFE131C
#define SH7262_SAR_OFS 0x00
#define SH7262_DAR_OFS 0x04
#define SH7262_DMATCR_OFS 0x08
#define SH7262_CHCR_OFS 0x0C
#define SH7262_RSAR_OFS 0x100
#define SH7262_RDAR_OFS 0x104
#define SH7262_RDMATCR_OFS 0x108
#define SH7262_CHCR_DM(reg) ((reg >> 14) & 0x03)
#define SH7262_CHCR_DM_FIX 0
#define SH7262_CHCR_DM_INC 1
#define SH7262_CHCR_DM_DEC 2
#define SH7262_CHCR_SM(reg) ((reg >> 12) & 0x03)
#define SH7262_CHCR_SM_FIX 0
#define SH7262_CHCR_SM_INC 1
#define SH7262_CHCR_SM_DEC 2
#define SH7262_CHCR_TS(reg) ((reg >> 3) & 0x03)
#define SH7262_CHCR_TS_BYTE 0
#define SH7262_CHCR_TS_WORD 1
#define SH7262_CHCR_TS_LONGWORD 2
#define SH7262_CHCR_TS_16BYTE 3
#define SH7262_CHCR_DE(reg) ((reg >> 0) & 0x01)
#define SH7262_CHCR_DE_PROHIBIT 0
#define SH7262_CHCR_DE_PERMIT 1
#define SH7262_DMARS_CH(reg, ch) (((ch & 0x01) == 0x00) ? ((reg >> 0) & 0x00FF) : ((reg >> 8) & 0x00FF))
#define SH7262_DMARS_MID(reg) ((reg >> 2) & 0x3F)
#define SH7262_DMARS_MID_RSPI_CH0 20
#define SH7262_DMARS_MID_RSPI_CH1 21
#define SH7262_DMARS_RID(reg) ((reg >> 0) & 0x03)
#define SH7262_DMARS_RID_RSPI_TX 1
#define SH7262_DMARS_RID_RSPI_RX 2

#endif // _SH7262_REGS_H_
