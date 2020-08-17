.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.L   immdata_0x0c000000, R1
	/* t2.stepi() */
	
	MOV     #0x7F, R0
	/* t2.stepi() */
	
	MOV.B   R0, @(4, R1)
	/* t2.stepi() */
	/* t2.expect_memories({"0x0c000004":"0x7f"}, "1") */
	/* t2.expect_registers({"pc":"0xa0000006"}) */
	
	MOV.L   immdata_0x0c000000, R1
	/* t2.stepi() */
	
	MOV     #0x80, R0
	/* t2.stepi() */
	
	MOV.B   R0, @(4, R1)
	/* t2.stepi() */
	/* t2.expect_memories({"0x0c000004":"0x80"}, "1") */
	/* t2.expect_registers({"pc":"0xa000000c"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0x0c000000:
	.long   0x0C000000
