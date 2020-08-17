.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV     #0x7F, R0
	/* t2.stepi() */
	
	MOV     R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0x7f","pc":"0xa0000004"}) */
	
	MOV     #0x80, R0
	/* t2.stepi() */
	
	MOV     R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0xffffff80","pc":"0xa0000008"}) */
	
	MOV.W   immdata_0x7fff, R0
	/* t2.stepi() */
	
	MOV     R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0x7fff"}) */
	
	MOV.W   immdata_0x8000, R0
	/* t2.stepi() */
	
	MOV     R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0xffff8000"}) */
	
	MOV.L   immdata_0x7fffffff, R0
	/* t2.stepi() */
	
	MOV     R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0x7fffffff"}) */
	
	MOV.L   immdata_0x80000000, R0
	/* t2.stepi() */
	
	MOV     R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0x80000000"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
immdata_0x7fff:
	.word   0x7FFF
immdata_0x8000:
	.word   0x8000
	.align  4
immdata_0x7fffffff:
	.long   0x7FFFFFFF
immdata_0x80000000:
	.long   0x80000000
