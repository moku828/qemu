import sys
import auto_test2 as t2

elf = "test_MOVWI.elf"

def test_scenario():
	t2.stepi()
	t2.expect_registers({"r0":"0x7fff"})
	t2.stepi()
	t2.expect_registers({"r1":"0xffff8000"})

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
