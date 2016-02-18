#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"

// === CONFIG =========================================

#define OTHER_RUN_FUNCTIONS	1
#define USE_REGISTERS			0
#define REPL_SWITCH			0

// === CORE ===========================================

typedef unsigned int token;

#if USE_REGISTERS
register token *ip asm ("edi"); // instruction pointer
register int *sp asm ("esi"); // data stack pointer - grows up
register token **rp asm ("ebx"); // return stack pointer - grows down
#else
token *ip;		// instruction pointer
int *sp;		// data stack pointer - grows up
token **rp;	// return stack pointer - grows down
#endif

token **fp;	// frame pointer

token *ibase;	// instructions base 
int *sbase;	// data stack base
token **rbase;	// return stack base

#define uint unsigned int
#define uchar unsigned char

int i,j,k;
int tmp;
uint utmp;

#define UTMP_LOAD_4() 	utmp = (uint)ip[1];
#define TMP_LOAD_4()	tmp = (int)ip[1];

int argcnt = 0;
char **argval;

// === RUN GOTO ======================================

#define NEXT			goto *dispatch[*++ip]
#define JUMP			goto *dispatch[*ip]

void run_goto() {
	void* dispatch[] = {
		#include "gen/dispatch.h"
	};
	
	JUMP;

	#include "op_other.h"
	#include "op_alu.h"
	#include "op_stack.h"
	#include "op_string.h"
	#include "op_flow.h"

}

#undef NEXT
#undef JUMP

// ==================================================

#if OTHER_RUN_FUNCTIONS
	#include "vm.h"
#endif

// ==================================================

void init(int *stack, int cells, token *code) {
	ip=code;
	sp=stack;
	rp=(token**)(stack+cells);
	ibase=ip;
	sbase=sp;
	rbase=rp;
}

void runcode() {
	token code[] = {
		#include "gen/code.h"
	};
	int mem[1024];
	init(&mem[0],1024,code);
	
	run_goto();
	//run_switch();
	//run_call();
	//run_direct();
	//run_repl_switch();
	//run_compiled_call();
	//run_compiled_inline();
}

int main(int argc, char *argv[]) {
	argcnt = argc;
	argval = argv;
	runcode();
	return 0;
}
