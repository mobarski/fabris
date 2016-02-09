// === RUN CALL =======================================

#include "functions.h"

void run_call() {
	void (*f)(void);
	void *dispatch[] = {
		#include "dispatch_call.h"
	};
	for (;;) {
		//f = dispatch[*ip];
		//f();
		((void(*)(void))dispatch[*ip])();
	}
}

#undef NEXT
#undef JUMP

// === RUN DIRECT =====================================

#define NEXT			goto **++ip
#define JUMP			goto **ip

void run_direct() {
	void *dispatch[] = {
		#include "code_direct.h"
	};
	ip = (token*)&dispatch[0];
	JUMP;
	#include "run_repl_switch.h"
}

#undef NEXT
#undef JUMP

// === RUN SWITCH =====================================

#define NEXT			ip += 1; break
#define JUMP			break

void run_switch() {
	for (;;) {
		switch (*ip) {
			#include "run_switch.h"
			default: NEXT;
		}
	}
}

#undef NEXT
#undef JUMP

// === RUN REPLICATED SWITCH =============================

#if REPL_SWITCH

#include "repl_switch.h"

void run_repl_switch() {
	JUMP;
	#include "run_repl_switch.h"
}

#undef NEXT
#undef JUMP

#endif

// === RUN COMPILED CALL ================================

#define NEXT			
#define JUMP			goto *dispatch[ip-ibase]

void run_compiled_call() {
	void *dispatch[] = {
		#include "dispatch_comp.h"
	};
	JUMP;
	#include "compiled_call.h"
}

#undef NEXT
#undef JUMP


// === RUN COMPILED INLINE ==============================

#define NEXT			ip += 1
#define JUMP			goto *dispatch[ip-ibase]

void run_compiled_inline() {
	void *dispatch[] = {
		#include "dispatch_comp.h"
	};
	JUMP;
	#include "compiled_inline.h"
}

#undef NEXT
#undef JUMP
