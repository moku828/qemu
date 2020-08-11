import sys
import pexpect
from gdb_mi import Output
from gdb_mi import StreamRecord
from gdb_mi import AsyncRecord
from gdb_mi import TerminationRecord
from gdb_mi import ResultRecord

gdb = 0

class pycolor:
	RED = '\033[31m'
	GREEN = '\033[32m'
	END = '\033[0m'

def expect(expected_object):
	text = gdb.readline().decode('utf-8').replace('\r','')
	out = Output()
	record = out.parse_line(text)
	assert type(record) == expected_object, '{0} is expected but actual is {1}, complete output is {2}'.format(expected_object, type(record), text)
	return record

def inforeg():
	gdb.sendline("-data-list-register-values x")
	gdb.readline()
	resultrecord = expect(ResultRecord)
	num2name = {
		0:"r0", 1:"r1", 2:"r2", 3:"r3", 4:"r4", 5:"r5", 6:"r6", 7:"r7",
		8:"r8", 9:"r9", 10:"r10", 11:"r11", 12:"r12", 13:"r13", 14:"r14", 15:"r15",
		16:"pc", 17:"pr", 18:"gbr", 19:"vbr", 20:"mach", 21:"macl", 22:"sr"
	}
	registers = {}
	for result in resultrecord.results:
		for register in result.value.value.value:
			i = int(register.value.value[0].value.value.value)
			if i in num2name:
				registers[num2name[i]] = register.value.value[1].value.value.value
	expect(TerminationRecord)
	return registers

def stepi():
	gdb.sendline("-exec-step-instruction")
	gdb.readline()
	expect(ResultRecord)
	expect(AsyncRecord)
	expect(TerminationRecord)
	expect(AsyncRecord)
	expect(TerminationRecord)

def expect_registers(expected_registers):
	registers = inforeg()
	for key in expected_registers:
		assert expected_registers[key] == registers[key], pycolor.RED+'{0} is expected for {2} but actual is {1}, complete output is {3}'.format(expected_registers[key], registers[key], key, registers)+pycolor.END

def setup(elf):
	global gdb
	gdb = pexpect.spawn("sh-elf-gdb -q -i=mi "+elf)
	expect(AsyncRecord)
	expect(StreamRecord)
	expect(StreamRecord)
	expect(TerminationRecord)
	gdb.sendline("-target-select remote :1234")
	gdb.readline()
	expect(AsyncRecord)
	expect(AsyncRecord)
	expect(StreamRecord)
	expect(StreamRecord)
	expect(AsyncRecord)
	expect(ResultRecord)
	expect(TerminationRecord)

def teardown():
	global gdb
	gdb.sendline("-interpreter-exec console \"kill\"")
