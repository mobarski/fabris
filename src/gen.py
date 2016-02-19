source = """

"xyz" tor dup 'v' swap poke tos print

"""

##############################################

import re
import os
from pprint import pprint

try:
	os.mkdir('gen')
except: pass

raw = ""
for fn in """
op_other.h 
op_alu.h
op_stack.h
op_flow.h
op_string.h
vm.c
""".split():
	raw += open(fn,'r').read()

f1 = open('gen/dispatch.h','w')
f2 = open('gen/functions.h','w')
f3 = open('gen/code.h','w')
f4 = open('gen/dispatch_call.h','w')
f5 = open('gen/run_switch.h','w')
f6 = open('gen/repl_switch.h','w')
f7 = open('gen/run_repl_switch.h','w')
f8 = open('gen/dispatch_comp.h','w')
f9 = open('gen/compiled_call.h','w')
f10 = open('gen/compiled_inline.h','w')
f11 = open('gen/code_direct.h','w')

ops = re.findall("(?xms) op_([a-z][a-z0-9]+): (.*?) (NEXT|JUMP);",raw)

# dispatch
opcode = {}
opname = {}
opbody = {}
opkind = {}

for i,(op,body,kind) in enumerate(ops):
	f1.write('&&op_%s, // %d\n'%(op,i))
	opcode[op] = i
	opname[i] = op
	opbody[i] = body
	opkind[i] = kind

f2.write('#define NEXT ip+=1 \n')
f2.write('#define JUMP \n')
for i,(op,body,kind) in enumerate(ops):
	f2.write("void do_%s() {\n%s%s;}\n" % (op,body,kind))
	f4.write("do_%s,\n" % (op))

for i,(op,body,kind) in enumerate(ops):
	f5.write('case %d:\n%s%s;\n'%(i,body,kind))

f6.write("#define NEXT switch(*++ip) { \\\n")
for i,(op,body,kind) in enumerate(ops):
	f6.write('case %d: goto op_%s; \\\n'%(i,op))
f6.write("}\n\n")
f6.write("#define JUMP switch(*ip) { \\\n")
for i,(op,body,kind) in enumerate(ops):
	f6.write('case %d: goto op_%s; \\\n'%(i,op))
f6.write("}\n\n")

for i,(op,body,kind) in enumerate(ops):
	f7.write('op_%s:%s%s;\n' % (op,body,kind))

############################################
############################################
############################################

regexp = """(?x)
	-- [^\n]* \n
|	[(] [^)]* [)]
|	" [^"]* "
|	' [^']* '
|	\[ | \]
|	[a-z0-9+-]+
"""
tokens = re.findall(regexp,source)
tokens+=['halt']
print(tokens)
code = []
ctrl = []
var = {}
word = {}
inline = {} # name -> (start,len)
op_ips = set()
def_var = False
into_var = False
def_word = False
curr_def = ''

for t in tokens:
	op_ips.add(len(code))
	
	# conversion
	try:
		x = int(t)
		code += [opcode['pushx']]
		code += [x]
		continue
	except: pass
	
	# names
	if def_var:
		var[t] = len(code)
		code += [0]
		def_var = False
		continue
	elif def_word:
		word[t] = len(code)
		curr_def = t
		def_word = False
		continue
	elif into_var:
		code += [var[t]]
		into_var = False
		continue
	
	if t[0]=='(':
		continue
	elif t.startswith('--'):
		continue
	elif t[0]=='"' and t[-1]=='"':
		pass # TODO string
		t=t[1:-1]
		code += [opcode['pushs']]
		code += [len(t)]
		x = 0
		for i,c in enumerate(t+'\x00\x00\x00\x00'):
			# ENDIAN SPECIFIC :(
			if 0:
				x <<= 8
				x += ord(c)
			else:
				x >>= 8
				x += ord(c)<<24
			if i>0 and i%4==3:
				code += [x]
				print("%x"%x)
				x = 0
	elif t[0]=="'" and t[-1]=="'":
		t=t[1:-1]
		code += [opcode['pushc']]
		code += [ord(t)]
	elif t=='times':
		code += [opcode['tor']]
		op_ips.add(len(code))
		code += [opcode['times']]
		ctrl += [('times',len(code))]
		code += [0]
	elif t=='do':
		ctrl += [('do',len(code)+1)]
	elif t=='loop':
		code += [opcode['back']]
		here = len(code)
		kind,there = ctrl.pop(-1)
		code += [here-there]
		if kind=='times':
			code[there] = here-there
	elif t=='then':
		code += [opcode['skipz']]
		ctrl += [('then',len(code))]
		code += [0]
	elif t=='else':
		code += [opcode['skip']]
		here = len(code)
		kind,there = ctrl.pop(-1)
		code[there+1] = here-there + 3
		ctrl += [('else',len(code))]
		code += [0]
	elif t=='end':
		here = len(code)
		kind,there = ctrl.pop(-1)
		code[there] = here-there + 1
	elif t=='var':
		code += [opcode['var']]
		def_var = True
	elif t=='into':
		code += [opcode['into']]
		into_var = True
	elif t=='def':
		code += [opcode['skip']]
		ctrl += [('def',len(code))]
		code += [0]
		def_word = True
	elif t=='ret':
		if len(ctrl)>1:
			code += [opcode['ret']]
			continue
		if code[-2]==opcode['callx']:
			code[-2] = opcode['tailcallx']
		else:
			code += [opcode['ret']]
		here = len(code)
		kind,there = ctrl.pop(-1)
		code[there] = here-there + 1
	elif t=='inline':
		here = len(code)
		kind,there = ctrl.pop(-1)
		code[there] = here-there + 1
		inline[curr_def] = (word[curr_def],here-there-1)
	elif t=='[':
		code += [opcode['lambda']]
		ctrl += [('lambda',len(code))]
		code += [0]
	elif t==']':
		code += [opcode['ret']]
		here = len(code)
		kind,there = ctrl.pop(-1)
		code[there] = here-there + 1
	elif t in ['test','if']:
		continue
	###
	elif t in var:
		code += [opcode['pushv']]
		code += [var[t]]
	elif t in inline:
		start,cnt = inline[t]
		for i in range(cnt):
			code += [code[start+i]]
	elif t in word:
		code += [opcode['callx']]
		code += [word[t]]
	else:
		code += [opcode[t]]
print(code)
print(ctrl)

#############################################
#############################################
#############################################

for i,x in enumerate(code):
	if i in op_ips:
		op = '%s' % opname[x]
	else:
		op = ''
	f3.write('%d, // ip=%d %s\n'%(x,i,op))

for i,x in enumerate(code):
	f8.write('&&ip_%d,\n'%i)
	if x in opname:
		f9.write('ip_%d: do_%s(); %s;\n'%(i,opname[x],opkind[x]))
		f10.write('ip_%d:%s%s;\n'%(i,opbody[x],opkind[x]))
	else:
		f9.write('ip_%d: NEXT;\n' % i)
		f10.write('ip_%d: NEXT;\n' % i)

for i,x in enumerate(code):
	if i in op_ips:
		op = '%s' % opname[x]
		f11.write('&&op_%s, // ip=%d\n'%(opname[x],i))
	else:
		f11.write('(void*) %d, // ip=%d\n'%(x,i))

print(inline)