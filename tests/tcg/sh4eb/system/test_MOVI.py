import sys
import auto_test2 as t2

elf = "test_MOVI.elf"

def test_scenario():
	
	# MOV     #0x7F, R0
	t2.stepi()
	t2.expect_registers({"r0":"0x7f"})
	
	# MOV     #0x80, R1
	t2.stepi()
	t2.expect_registers({"r1":"0xffffff80"})
	

def main():
	global elf
	pyscript = sys.argv[0]
	result = -1
	try:
		t2.setup(elf)

		test_scenario()

		print(pyscript+" ["+t2.pycolor.GREEN+"PASSED"+t2.pycolor.END+"]")
		result = 0

	except AssertionError as e:
		print(pyscript+" ["+t2.pycolor.RED+"FAILED"+t2.pycolor.END+"]")
		print(e)

	finally:
		t2.teardown()
		sys.exit(result)

if __name__ == "__main__":
	main()
