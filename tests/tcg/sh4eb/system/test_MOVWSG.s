.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.L   immdata_0x0c000000, R1
	/* t2.stepi() */
	
	LDC     R1, GBR
	/* t2.stepi() */
	
	MOV.W   immdata_0x7fff, R0
	/* t2.stepi() */
	
	MOV.W   R0, @(4, GBR)
	/* t2.stepi() */
	/* t2.expect_memories({"0x0c000004":"0x7fff"}, "2") */
	/* t2.expect_registers({"pc":"0xa0000008"}) */
	
	MOV.L   immdata_0x0c000000, R1
	/* t2.stepi() */
	
	LDC     R1, GBR
	/* t2.stepi() */
	
	MOV.W   immdata_0x8000, R0
	/* t2.stepi() */
	
	MOV.W   R0, @(4, GBR)
	/* t2.stepi() */
	/* t2.expect_memories({"0x0c000004":"0x8000"}, "2") */
	/* t2.expect_registers({"pc":"0xa0000010"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
immdata_0x7fff:
	.word   0x7FFF
immdata_0x8000:
	.word   0x8000
	.align  4
immdata_0x0c000000:
	.long   0x0C000000
