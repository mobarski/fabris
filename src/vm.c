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
#else
token *ip;		// instruction pointer
int *sp;		// data stack pointer - grows up
#endif
token **rp;	// return stack pointer - grows down

token *ibase;	// instructions base 
int *sbase;	// data stack base
token **rbase;	// return stack base

#define uint unsigned int
#define uchar unsigned char

int i;
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
		#include "dispatch.h"
	};
	
	JUMP;

	// === OTHER ==================
	op_nop:
		NEXT;
	op_count:
		sp[1] = *(uchar*)sp[0];
		sp[0] += 1;
		sp += 1;
		NEXT;
	op_clock:
		sp[1] = clock();
		sp += 1;
		NEXT;
	op_halt:
		exit(0);
		NEXT;
	op_dot:
		printf("%d ",sp[0]);
		NEXT;
	op_udot:
		printf("%u ",sp[0]);
		NEXT;
	op_xdot:
		printf("%x ",sp[0]);
		NEXT;
	op_emit:
		putchar((char)sp[0]);
		sp -= 1;
		NEXT;
	op_print:
		write(1, (char*)sp[-1], sp[0]);
		sp-=2;
		NEXT;
	op_trace:
		printf("ip:%u  ->  ",ip-ibase);
		for (i=-10; i<0; i++)
			printf("%d ",ip[i]);
		printf("[%x] ",ip[0]);
		for (i=1; i<=10; i++)
			printf("%d ",ip[i]);
		printf("\n");
		printf("sp:%u  ->  ",sp-sbase);
		for (i=3; i>0; i--)
			printf("%d ",sp[i]);
		printf("[%x] ",sp[0]);
		for (i=-1; i>=-6; i--)
			printf("%d ",sp[i]);
		printf("\n");
		NEXT;
	op_assert:
		if (sp[0]==sp[-1]) { sp-=1; // (x y -- x)
		} else {
			printf("ERROR: assertion failed at ip=%d, expected %d got %d\n",ip-ibase,sp[0],sp[-1]);
			exit(0);
		}
		NEXT;
	op_argc:
		sp[1] = argcnt;
		sp += 1;
		NEXT;
	op_argv:
		if (sp[0]<argcnt) {
			sp[1] = strlen(argval[sp[0]]);
			sp[0] = (uint)argval[sp[0]];
		} else {
			sp[1] = 0;
			sp[0] = 0;
		}
		sp += 1;
		NEXT;
	// === BASIC ARITHMETIC ============
	op_add:
		sp[-1] += sp[0];
		sp -=1;
		NEXT;
	op_sub:
		sp[-1] -= sp[0];
		sp -=1;
		NEXT;
	op_mul:
		sp[-1] *= sp[0];
		sp -=1;
		NEXT;
	op_div:
		sp[-1] /= sp[0];
		sp -=1;
		NEXT;
	op_mod:
		sp[-1] %= sp[0];
		sp -=1;
		NEXT;
	op_inc:
		sp[0] += 1;
		NEXT;
	op_dec:
		sp[0] -= 1;
		NEXT;
	op_abs:
		if (sp[0]<0) sp[0]=-sp[0];
		NEXT;
	op_neg:
		sp[0] = -sp[0];
		NEXT;

	// === MORE ARITHMETIC =======
	op_min:
		sp[-1] = sp[-1]<sp[0] ? sp[-1] : sp[0];
		sp -= 1;
		NEXT;
	op_max:
		sp[-1] = sp[-1]>sp[0] ? sp[-1] : sp[0];
		sp -= 1;
		NEXT;
	op_limit:
		sp[-2] = sp[-2]<sp[-1] ? sp[-1] : sp[-2]>sp[0] ? sp[0] : sp[-2] ;
		sp -= 2;
		NEXT;

	// === LOGIC ================
	op_and:
		sp[-1] &= sp[0];
		sp -=1;
		NEXT;
	op_or:
		sp[-1] |= sp[0];
		sp -=1;
		NEXT;
	op_xor:
		sp[-1] ^= sp[0];
		sp -=1;
		NEXT;
	op_shl:
		sp[-1] <<= sp[0];
		sp -=1;
		NEXT;
	op_shr:
		sp[-1] >>= sp[0];
		sp -=1;
		NEXT;
	op_inv:
		sp[0] = ~sp[0];
		NEXT;
	
	// === COMPARATORS ========
	op_zero:
		sp[1] = sp[0] ? 0 : 1 ;
		sp += 1;
		NEXT;
	op_positive:
		sp[1] = sp[0]>0 ? 1 : 0 ;
		sp += 1;
		NEXT;
	op_negative:
		sp[1] = sp[0]<0 ? 1 : 0 ;
		sp += 1;
		NEXT;
	op_le:
		sp[1] = sp[-1]<=sp[0] ? 1 : 0 ;
		sp += 1;
		NEXT;
	op_lt:
		sp[1] = sp[-1]<sp[0] ? 1 : 0 ;
		sp += 1;
		NEXT;
	op_ge:
		sp[1] = sp[-1]>=sp[0] ? 1 : 0 ;
		sp += 1;
		NEXT;
	op_gt:
		sp[1] = sp[-1]>sp[0] ? 1 : 0 ;
		sp += 1;
		NEXT;
	op_eq:
		sp[1] = sp[-1]==sp[0] ? 1 : 0 ;
		sp += 1;
		NEXT;
	op_ne:
		sp[1] = sp[-1]!=sp[0] ? 1 : 0 ;
		sp += 1;
		NEXT;
	
	// ===  STACK ==================
	op_dup:
		sp[1] = sp[0]; 
		sp += 1;
		NEXT;
	op_drop:
		sp -= 1;
		NEXT;
	op_over:
		sp[1] = sp[-1];
		sp += 1;
		NEXT;
	op_swap:
		tmp = sp[0];
		sp[0] = sp[-1];
		sp[-1] = tmp;
		NEXT;
	op_nip:
		sp[-1] = sp[0];
		sp -= 1;
		NEXT;
	op_tuck:
		sp[1] = sp[0];
		sp[0] = sp[-1];
		sp[-1] = sp[1];
		sp+=1;
		NEXT;
	op_rot:
		tmp = sp[-2];
		sp[-2] = sp[-1];
		sp[-1] = sp[0];
		sp[0] = tmp;
		NEXT;
	op_unrot:
		tmp = sp[0];
		sp[0] = sp[-1];
		sp[-1] = sp[-2];
		sp[-2] = tmp;
		NEXT;
	op_depth:
		sp[1] = (uint)(sp-sbase);
		sp += 1;
		NEXT;
	op_tor:
		rp[-1] = (token*)sp[0];
		sp -= 1;
		rp -= 1;
		NEXT;
	op_fromr:
		sp[1] = (uint)rp[0];
		sp += 1;
		rp += 1;
		NEXT;
	
	// === PUSH ==================
	op_pushc:
		sp[1] = (char)ip[1];
		sp += 1;
		ip += 2;
		JUMP;
	op_pushuc:
		sp[1] = (uchar)ip[1];
		sp += 1;
		ip += 2;
		JUMP;
	op_push:
		sp[1] = ip[1];
		sp += 1;
		ip += 2;
		JUMP;
	op_pushs: // TODO TEST
		UTMP_LOAD_4();
		sp[1] = (uint)&ip[2];
		sp[2] = utmp;
		sp += 2;
		if ((utmp+1) % 4==0)
			ip += 2 + (utmp+1)/4; // OP+LEN + STR+NULL
		else {
			ip += 2 + (utmp+1)/4 + 1 ; // OP+LEN + STR+NULL + PAD
		}
		JUMP;
	
	// ===  FLOW ===============
	op_skip:
		ip += (token)ip[1]+2;
		JUMP;
	op_back:
		ip -= (token)ip[1];
		JUMP;
	op_exe:
		rp[-1] = ip+1;
		ip = (token*)sp[0];
		rp -= 1;
		sp -= 1;
		JUMP;
	op_ret:
		ip=rp[0]; rp+=1;
		JUMP;
	op_call:
		UTMP_LOAD_4();
		rp[-1] = ip+2; // TODO vs token size
		rp -= 1;
		ip = ibase+utmp;
		JUMP;
	op_tailcall:
		UTMP_LOAD_4();
		ip = ibase+utmp;
		JUMP;
	op_times:
		if (rp[0]==0) {
			rp += 1;
			ip += (token)ip[1]+2;
		} else {
			rp[0] = (token*)((uint)rp[0]-1);
			//rp[0] -= 1;
			ip += 2;
		}
		JUMP;
	op_jump:
		TMP_LOAD_4();
		ip += tmp;
		JUMP;
	op_jumpz:
		if (sp[0]==0) {
			TMP_LOAD_4();
			ip+=tmp;
		} else {ip+=2;}
		sp -= 1;
		JUMP;
	op_jumpnz:
		if (sp[0]!=0) {
			TMP_LOAD_4();
			ip+=tmp;
		} else {ip+=2;}
		sp -= 1;
		JUMP;
	op_jumpm:
		if (sp[0]<0) {
			TMP_LOAD_4();
			ip+=tmp;
		} else {ip+=2;}
		sp -= 1;
		JUMP;
	op_jumpnm:
		if (sp[0]>=0) {
			TMP_LOAD_4();
			ip+=tmp;
		} else {ip+=2;}
		sp -= 1;
		JUMP;
	// === NEW ============================
	op_var:
		ip[1] = sp[0];
		sp -= 1;
		ip += 2;
		JUMP;
	op_pushv:
		sp[1] = ibase[ip[1]];
		sp += 1;
		ip += 2;
		JUMP;
	op_into:
		ibase[ip[1]] = sp[0];
		sp -= 1;
		ip += 2;
		JUMP;
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
		#include "code.h"
	};
	int mem[1024];
	init(&mem[0],1024,code);
	
	run_goto();
	//~ run_direct();
	//~ run_call();
	//~ run_switch();
	//~ run_repl_switch();
	//~ run_compiled_call();
	//~ run_compiled_inline();
}

int main(int argc, char *argv[]) {
	argcnt = argc;
	argval = argv;
	runcode();
	return 0;
}
