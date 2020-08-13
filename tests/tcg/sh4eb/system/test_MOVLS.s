.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.L   immdata_0x7fffffff, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x0c000000, R1
	/* t2.stepi() */
	
	MOV.L   R0, @R1
	/* t2.stepi() */
	/* t2.expect_memories({"0x0c000000":"0x7fffffff"}, "4") */
	
	/* test_scenario end */
	.align  4
immdata_0x7fffffff:
	.long   0x7FFFFFFF
immdata_0x0c000000:
	.long   0x0C000000
