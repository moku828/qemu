.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV     #0x7F, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x0c000004, R1
	/* t2.stepi() */
	
	MOV.B   R0, @-R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0xc000003","pc":"0xa0000006"}) */
	/* t2.expect_memories({"0x0c000003":"0x7f"}, "1") */
	
	MOV     #0x80, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x0c000004, R1
	/* t2.stepi() */
	
	MOV.B   R0, @-R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0xc000003","pc":"0xa000000c"}) */
	/* t2.expect_memories({"0x0c000003":"0x80"}, "1") */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0x0c000004:
	.long   0x0C000004