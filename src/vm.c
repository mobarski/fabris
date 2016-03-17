#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"

// === CONFIG =========================================

#define OTHER_RUN_FUNCTIONS	0
#define USE_REGISTERS			1
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

int fpb;		// fixed point base
int *ap;		// allocator stack pointer - grows up
int *bp;		// buffer stack pointer - grows down
int ir;		// index register

int *abase;	// allocator stack base
int *bbase;	// buffer stack base
token **rbase;	// return stack base
token *ibase;	// instructions base 
int *sbase;	// data stack base


#define uint unsigned int
#define wint long long int
#define uchar unsigned char

int i,j,k;
int tmp;
uint utmp;
wint wtmp;

#define UTMP_LOAD_4() 	utmp = (uint)ip[1];
#define TMP_LOAD_4()	tmp = (int)ip[1];

int argcnt = 0;
char **argval;

#define TRUE -1
#define FALSE 0

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

void init(int *stack, int cells, int *stack2, int cells2, token *code) {
	ip=code;
	sp=stack;
	rp=(token**)(stack+cells);
	ap=stack2;
	bp=stack2+cells2;
	
	abase=ap;
	bbase=bp;
	rbase=rp;
	ibase=ip;
	sbase=sp;
	fpb=1;
}

void runcode() {
	token code[] = {
		#include "gen/code.h"
	};
	int mem[1024];
	int mem2[4*1024];
	init(&mem[0],1024,&mem2[0],4*1024,code);
	
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
