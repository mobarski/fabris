source = """
    1000 tof
    3.14159 2 mul fdot
    --3.14159 2.0 fmul fdot 
    --3141 2 mul fdot
    --3141 2000 fmul fdot
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

ops = re.findall("(?xms) op_([a-z][a-z0-9_]*): (.*?) (NEXT|JUMP);",raw)

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

def tokenize(text):
	regexp = """(?x)
		-- [^\n]* \n
	|	[(] [^)]* [)]
	|	" [^"]* "
	|	' [^']* '
	|	\[ | \]
	|	[^\s]+
	"""
	tokens = re.findall(regexp,text)
	return tokens

tokens = tokenize(source)
tokens+=['halt']
code = []
ctrl = []
var = {}
word = {}
macro = {} # name -> [tokens]
inline = {} # name -> (start,len)
mods = set()
op_ips = set()
def_var = False
into_var = False
def_word = False
def_macro = False
use_mod = False
curr_def = ''
curr_macro = ''
out_tokens = []
macro_depth = 0
undefined = {}# name -> [pos,pos,...]
dynamic = set()
def_dyn = False
as_word = False
ref_word = False
def_input = False
def_output = False
curr_input = []
curr_output = None


while tokens:
	t = tokens.pop(0)
	
	#print('token',t,'depth',macro_depth)
	if macro_depth:
		if t=='mend' and macro_depth==1:
			macro_depth = 0
		else:
			if t=='macro': macro_depth+=1
			if t=='mend': macro_depth-=1
			macro[curr_macro] += [t]
		continue

	out_tokens += [t]

	op_ips.add(len(code))
	
	# names / modes
	if def_var:
		var[t] = len(code)
		code += [0]
		def_var = False
		continue
	elif def_word:
		if t in dynamic:
			code[word[t]+1] = len(code)
		else:
			word[t] = len(code)
		curr_def = t
		def_word = False
		if t in undefined:
			for pos in undefined[t]:
				code[pos] = word[t]
			del undefined[t]
		curr_input = []
		curr_output = None
		continue
	elif into_var:
		code += [var[t]]
		into_var = False
		continue
	elif use_mod:
		if t not in mods:
			fn = os.path.join('mod',t+'.fabris') # TODO module paths
			mod_code = open(fn,'r').read()
			mod_tokens = tokenize(mod_code)
			tokens[0:0] = mod_tokens
		use_mod = False
		continue
	elif def_macro:
		macro[t] = []
		curr_macro = t
		macro_depth = 1
		def_macro = False
		continue
	elif def_dyn:
		dynamic.add(t)
		word[t] = len(code)
		code += [opcode['tailcallx']]
		code += [0]
		def_dyn = False
		continue
	elif as_word:
		code += [opcode['into']]
		code += [word[t]+1]
		as_word = False
		continue
	elif ref_word:
		if t in word:
			code += [opcode['pushx']]
			code += [word[t]]
		else:
			raise # TODO ? wrap opcode
		ref_word = False
		continue
	elif def_input:
		if t=='out':
			def_input=False
			def_output=True
			code += [opcode['in']]
			code += [len(curr_input)]
		else:
			curr_input += [t]
		continue
	elif def_output:
		curr_output = int(t)
		def_output = False
		continue

	# conversion
	if re.match('([+-]?\d+[.]\d*)|([+-]?\d*[.]\d+)',t):
		a,b = t.split('.')
		if not a: a='0'
		if not b: b='0'
		a=int(a)
		b=int(b)
		print("fixed point",t,a,b)
		code += [opcode['pushxf']]
		code += [a]		
		code += [b]		
		continue
	try:
		x = int(t)
		code += [opcode['pushx']]
		code += [x]
		continue
	except: pass

	# normal
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
		code += [opcode['do']]
		ctrl += [('do',len(code))]
		code += [0]
	elif t=='loop':
		code += [opcode['back']]
		here = len(code)
		pprint(ctrl)
		tmp = []
		while True:
			kind,there = ctrl.pop(-1)
			if kind=='times':
				code += [here-there]
				code[there] = here-there
				tmp.reverse()
				ctrl += tmp
				break
			elif kind=='do':
				code += [here-there-2]
				code[there] = here-there+2
				tmp.reverse()
				ctrl += tmp
				break
			else:
				tmp +=[(kind,there)]
	elif t=='break':
		code += [opcode['break']]
	elif t=='then':
		code += [opcode['skipz']]
		ctrl += [('then',len(code))]
		code += [0]
	elif t=='else':
		code += [opcode['skip']]
		here = len(code)
		kind,there = ctrl.pop(-1)
		code[there] = here-there + 2
		#ctrl += [(kind,there)]
		ctrl += [('else',len(code))]
		code += [0]
	elif t=='end':
		here = len(code)
		kind,there = ctrl.pop(-1)
		code[there] = here-there + 1
		if kind=='else':
			while ctrl and ctrl[-1][0]=='else':
				kind,there = ctrl.pop(-1)
				code[there] = here-there + 1
	elif t=='var':
		code += [opcode['var']]
		def_var = True
	elif t=='into':
		code += [opcode['into']]
		into_var = True
	elif t=='dyn':
		code += [opcode['skip']]
		code += [4]
		def_dyn = True
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
			if curr_output == None:
				code += [opcode['ret']]
			else:
				code += [opcode['iret']]
				code += [curr_output]
		here = len(code)
		kind,there = ctrl.pop(-1)
		code[there] = here-there + 1
	elif t=='[':
		code += [opcode['lambda']]
		ctrl += [('[',len(code))]
		code += [0]
	elif t=='_':
		code += [opcode['pushx']]
		ctrl += [('_',len(code))]
		code += [0]
	elif t==']':
		code += [opcode['ret']]
		here = len(code)
		slot_cnt = 0
		while True:
			kind,there = ctrl.pop(-1)
			if kind=='[':
				code[there] = here-there + 1
				if slot_cnt>0:
					code += [opcode['r']]
				break
			elif kind=='_':
				if slot_cnt==0:
					code += [opcode['tor']]
				code += [opcode['into']]
				code += [there]
				slot_cnt += 1
	elif t=='as':
		as_word = True
	elif t=='ref':
		ref_word = True
	elif t in ['test','if']:
		continue
	elif t=='use':
		use_mod = True
	elif t=='macro':
		def_macro = True
	elif t=='in':
		def_input = True
	###
	elif t in macro:
		tokens[0:0] = macro[t]
	elif t in curr_input:
		code += [opcode['ix']]
		code += [curr_input.index(t)]
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
	elif t.replace('-','_') in opcode:
		code += [opcode[t.replace('-','_')]]
	else:
		code += [opcode['callx']]
		if t not in undefined: undefined[t] = []
		undefined[t] += [len(code)]
		code += [0]
print(out_tokens)
print('code:',code)
print('ctrl:',ctrl)
print('macro:',macro)
print('undef:',undefined)

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