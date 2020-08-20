.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	CLRT
	/* t2.stepi() */
	
	MOV.L   immdata_0x11111111, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x11111111, R1
	/* t2.stepi() */
	
	CMP/STR R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"sr":"0x700000f1","pc":"0xa0000008"}) */
	
	CLRT
	/* t2.stepi() */
	
	MOV.L   immdata_0x22111111, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x11111111, R1
	/* t2.stepi() */
	
	CMP/STR R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"sr":"0x700000f1","pc":"0xa0000010"}) */
	
	CLRT
	/* t2.stepi() */
	
	MOV.L   immdata_0x11221111, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x11111111, R1
	/* t2.stepi() */
	
	CMP/STR R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"sr":"0x700000f1","pc":"0xa0000018"}) */
	
	CLRT
	/* t2.stepi() */
	
	MOV.L   immdata_0x11112211, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x11111111, R1
	/* t2.stepi() */
	
	CMP/STR R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"sr":"0x700000f1","pc":"0xa0000020"}) */
	
	CLRT
	/* t2.stepi() */
	
	MOV.L   immdata_0x11111122, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x11111111, R1
	/* t2.stepi() */
	
	CMP/STR R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"sr":"0x700000f1","pc":"0xa0000028"}) */
	
	CLRT
	/* t2.stepi() */
	
	MOV.L   immdata_0x22222222, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x11111111, R1
	/* t2.stepi() */
	
	CMP/STR R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"sr":"0x700000f0","pc":"0xa0000030"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
immdata_0x11111111:
	.long   0x11111111
immdata_0x22111111:
	.long   0x22111111
immdata_0x11221111:
	.long   0x11221111
immdata_0x11112211:
	.long   0x11112211
immdata_0x11111122:
	.long   0x11111122
immdata_0x22222222:
	.long   0x22222222
