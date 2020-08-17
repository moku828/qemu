.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.W   immdata_0x7fff, R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0x7fff","pc":"0xa0000002"}) */

	MOV.W   immdata_0x8000, R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0xffff8000","pc":"0xa0000004"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
immdata_0x7fff:
	.word   0x7FFF
immdata_0x8000:
	.word   0x8000
