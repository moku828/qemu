.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV     #0x7F, R2
	/* t2.stepi() */
	
	MOV     #0x4, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x0c000000, R1
	/* t2.stepi() */
	
	MOV.B   R2, @(R0,R1)
	/* t2.stepi() */
	/* t2.expect_memories({"0x0c000004":"0x7f"}, "1") */
	
	MOV     #0x80, R2
	/* t2.stepi() */
	
	MOV     #0x4, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x0c000000, R1
	/* t2.stepi() */
	
	MOV.B   R2, @(R0,R1)
	/* t2.stepi() */
	/* t2.expect_memories({"0x0c000004":"0x80"}, "1") */
	
	/* test_scenario end */
	.align  4
immdata_0x0c000000:
	.long   0x0C000000
