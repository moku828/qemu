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

#define SH7262_FRQCR 0xFFFE0010
#define SH7262_FRQCR_UB 0xFFFE0010
#define SH7262_FRQCR_LB 0xFFFE0011
#define SH7262_FRQCR_STC(reg) ((reg >> 8) & 0x01)
#define SH7262_FRQCR_STC_MUL8 0
#define SH7262_FRQCR_STC_MUL12 1
#define SH7262_FRQCR_PFC(reg) ((reg >> 0) & 0x07)
#define SH7262_FRQCR_PFC_DIV4 3
#define SH7262_FRQCR_PFC_DIV6 4
#define SH7262_FRQCR_PFC_DIV8 5
#define SH7262_FRQCR_PFC_DIV12 6

#define SH7262_STBCR1 0xFFFE0014
#define SH7262_STBCR2 0xFFFE0018
#define SH7262_STBCR3 0xFFFE0408
#define SH7262_STBCR4 0xFFFE040C
#define SH7262_STBCR5 0xFFFE0410
#define SH7262_STBCR6 0xFFFE0414
#define SH7262_STBCR7 0xFFFE0418
#define SH7262_STBCR8 0xFFFE041C
#define SH7262_SYSCR1 0xFFFE0400
#define SH7262_SYSCR2 0xFFFE0404
#define SH7262_ICR0 0xFFFE0800
#define SH7262_ICR1 0xFFFE0802
#define SH7262_IBNR 0xFFFE080E
#define SH7262_IPR01 0xFFFE0818
#define SH7262_IPR02 0xFFFE081A
#define SH7262_IPR05 0xFFFE0820
#define SH7262_IPR06 0xFFFE0C00
#define SH7262_IPR07 0xFFFE0C02
#define SH7262_IPR08 0xFFFE0C04
#define SH7262_IPR09 0xFFFE0C06
#define SH7262_IPR10 0xFFFE0C08
#define SH7262_IPR10_UB 0xFFFE0C08
#define SH7262_IPR10_LB 0xFFFE0C09
#define SH7262_IPR11 0xFFFE0C0A
#define SH7262_IPR12 0xFFFE0C0C
#define SH7262_IPR13 0xFFFE0C0E
#define SH7262_IPR14 0xFFFE0C10
#define SH7262_IPR15 0xFFFE0C12
#define SH7262_IPR16 0xFFFE0C14
#define SH7262_IPR17 0xFFFE0C16
#define SH7262_IPR18 0xFFFE0C18
#define SH7262_IPR19 0xFFFE0C1A
#define SH7262_IPR20 0xFFFE0C1C
#define SH7262_IPR21 0xFFFE0C1E
#define SH7262_IPR22 0xFFFE0C20

#define SH7262_CMT_SIZE 0x0000000E
#define SH7262_CMT_CMSTR 0xFFFEC000
#define SH7262_CMT_BASE_CH0 0xFFFEC002
#define SH7262_CMT_BASE_CH1 0xFFFEC008
#define SH7262_CMCSR_OFS 0x00
#define SH7262_CMCNT_OFS 0x02
#define SH7262_CMCOR_OFS 0x04
#define SH7262_CMSTR_STR1(reg) ((reg >> 1) & 0x01)
#define SH7262_CMSTR_STR1_CNTSTP 0
#define SH7262_CMSTR_STR1_CNTSTA 1
#define SH7262_CMSTR_STR0(reg) ((reg >> 0) & 0x01)
#define SH7262_CMSTR_STR0_CNTSTP 0
#define SH7262_CMSTR_STR0_CNTSTA 1
#define SH7262_CMCSR_CMF(reg) ((reg >> 7) & 0x01)
#define SH7262_CMCSR_CMF_NOTMATCHED 0
#define SH7262_CMCSR_CMF_MATCHED 1
#define SH7262_CMCSR_CMIE(reg) ((reg >> 6) & 0x01)
#define SH7262_CMCSR_CMIE_PROHIBIT 0
#define SH7262_CMCSR_CMIE_PERMIT 1
#define SH7262_CMCSR_CKS(reg) ((reg >> 0) & 0x03)
#define SH7262_CMCSR_CKS_PCLKDIV8 0
#define SH7262_CMCSR_CKS_PCLKDIV32 1
#define SH7262_CMCSR_CKS_PCLKDIV128 2
#define SH7262_CMCSR_CKS_PCLKDIV512 3

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

#define SH7262_SCIF_BASE_CH3 0xFFFE9800

#define SH7262_PCCR2 0xFFFE384A
#define SH7262_PCCR2_UB 0xFFFE384A
#define SH7262_PCCR2_LB 0xFFFE384B
#define SH7262_PCCR1 0xFFFE384C
#define SH7262_PCCR1_UB 0xFFFE384C
#define SH7262_PCCR1_LB 0xFFFE384D
#define SH7262_PCCR1_PC7MD(reg) ((reg >> 12) & 0x03)
#define SH7262_PCCR1_PC7MD_PC7 0
#define SH7262_PCCR1_PC7MD_CKE 1
#define SH7262_PCCR1_PC7MD_TIOC4C 2
#define SH7262_PCCR1_PC7MD_IRQ6 3
#define SH7262_PCIOR0 0xFFFE3852
#define SH7262_PCIOR0_UB 0xFFFE3852
#define SH7262_PCIOR0_LB 0xFFFE3853
#define SH7262_PCIOR0_PC7IOR(reg) ((reg >> 7) & 0x01)
#define SH7262_PCDR0 0xFFFE3856
#define SH7262_PCDR0_UB 0xFFFE3856
#define SH7262_PCDR0_LB 0xFFFE3857
#define SH7262_PCDR0_PC7DR(reg) ((reg >> 7) & 0x01)
#define SH7262_PCPR0 0xFFFE385A
#define SH7262_PCPR0_UB 0xFFFE385A
#define SH7262_PCPR0_LB 0xFFFE385B

#define SH7262_PFCR3 0xFFFE38A8
#define SH7262_PFCR3_UB 0xFFFE38A8
#define SH7262_PFCR3_LB 0xFFFE38A9
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
#define SH7262_PFCR1 0xFFFE38AC
#define SH7262_PFCR1_PF6MD(reg) ((reg >> 8) & 0x07)
#define SH7262_PFCR1_PF6MD_PF6 0
#define SH7262_PFCR1_PF6MD_nCS6nCE1B 1
#define SH7262_PFCR1_PF6MD_SSISCK3 2
#define SH7262_PFCR1_PF6MD_DV_DATA6 3
#define SH7262_PFCR1_PF6MD_TCLKB 4
#define SH7262_PFCR1_PF4MD(reg) ((reg >> 0) & 0x07)
#define SH7262_PFCR1_PF4MD_PF4 0
#define SH7262_PFCR1_PF4MD_nICIOWRnAH 1
#define SH7262_PFCR1_PF4MD_SSIWS2 2
#define SH7262_PFCR1_PF4MD_DV_DATA4 3
#define SH7262_PFCR1_PF4MD_TxD3 4
#define SH7262_PFCR1_PF4MD_SD_CLK 6
#define SH7262_PFCR0 0xFFFE38AE
#define SH7262_PFCR0_PF3MD(reg) ((reg >> 12) & 0x07)
#define SH7262_PFCR0_PF3MD_PF3 0
#define SH7262_PFCR0_PF3MD_nICIORD 1
#define SH7262_PFCR0_PF3MD_SSISCK2 2
#define SH7262_PFCR0_PF3MD_DV_DATA3 3
#define SH7262_PFCR0_PF3MD_RxD3 4
#define SH7262_PFCR0_PF3MD_SD_CMD 6
#define SH7262_PFCR0_PF2MD(reg) ((reg >> 8) & 0x07)
#define SH7262_PFCR0_PF2MD_PF2 0
#define SH7262_PFCR0_PF2MD_nBACK 1
#define SH7262_PFCR0_PF2MD_SSIDATA1 2
#define SH7262_PFCR0_PF2MD_DV_DATA2 3
#define SH7262_PFCR0_PF2MD_TxD2 4
#define SH7262_PFCR0_PF2MD_DACK0 5
#define SH7262_PFCR0_PF1MD(reg) ((reg >> 4) & 0x07)
#define SH7262_PFCR0_PF1MD_PF1 0
#define SH7262_PFCR0_PF1MD_nBREQ 1
#define SH7262_PFCR0_PF1MD_SSIWS1 2
#define SH7262_PFCR0_PF1MD_DV_DATA1 3
#define SH7262_PFCR0_PF1MD_RxD2 4
#define SH7262_PFCR0_PF1MD_DREQ0 5
#define SH7262_PGCR5 0xFFFE38C4
#define SH7262_PGCR5_PG20MD(reg) ((reg >> 0) & 0x07)
#define SH7262_PGCR5_PG20MD_PG20 0
#define SH7262_PGCR5_PG20MD_LCD_EXTCLK 1
#define SH7262_PGCR5_PG20MD_MISO1 3
#define SH7262_PGCR5_PG20MD_TxD7 4
#define SH7262_PGIOR1 0xFFFE38D0
#define SH7262_PGIOR1_PG20IOR(reg) ((reg >> 4) & 0x01)
#define SH7262_PGDR1 0xFFFE38D4
#define SH7262_PGDR1_UB 0xFFFE38D4
#define SH7262_PGDR1_LB 0xFFFE38D5
#define SH7262_PGDR1_PG20DR(reg) ((reg >> 4) & 0x01)
#define SH7262_PJCR0 0xFFFE390E
#define SH7262_PJIOR0 0xFFFE3912
#define SH7262_PJPR0 0xFFFE391A

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
