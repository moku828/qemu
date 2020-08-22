def gen_test_scenario_content_from_asm(asmfile):
	s = ''
	in_test_scenario = False
	f = open('/qemu/tests/tcg/sh4eb/system/' + asmfile, 'r')
	for line in f:
		if ('test_scenario end' in line):
			in_test_scenario = False
		if in_test_scenario:
			if (('/*' in line) and ('*/' in line)):
				s = s + line.replace('/* ', '').replace(' */', '')
			elif (len(line.strip()) > 0):
				s = s + "\t" + '# ' + line.strip() + "\n"
			else:
				s = s + "\t" + "\n"
		if ('test_scenario start' in line):
			in_test_scenario = True
	return s

def main():
	import sys
	import string
	asmfile = sys.argv[1]
	f = open('/qemu/tests/tcg/sh4eb/system/test_INST.py.template', 'r')
	template = string.Template(f.read())
	print(template.substitute(elffile=asmfile.replace('.s', '.elf'), test_scenario_content=gen_test_scenario_content_from_asm(asmfile)))

main()
