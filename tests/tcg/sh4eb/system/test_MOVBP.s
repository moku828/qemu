.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV     #0x7F, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x0c000000, R1
	/* t2.stepi() */
	
	MOV.B   R0, @R1
	/* t2.stepi() */
	
	MOV.B   @R1+, R2
	/* t2.stepi() */
	/* t2.expect_registers({"r2":"0x7f", "r1":"0xc000001"}) */
	
	MOV     #0x80, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x0c000000, R1
	/* t2.stepi() */
	
	MOV.B   R0, @R1
	/* t2.stepi() */
	
	MOV.B   @R1+, R2
	/* t2.stepi() */
	/* t2.expect_registers({"r2":"0xffffff80", "r1":"0xc000001"}) */
	
	/* test_scenario end */
	.align  4
immdata_0x0c000000:
	.long   0x0C000000
