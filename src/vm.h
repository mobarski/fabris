// === RUN CALL =======================================

#include "gen/functions.h"

void run_call() {
	void (*f)(void);
	void *dispatch[] = {
		#include "gen/dispatch_call.h"
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
		#include "gen/code_direct.h"
	};
	ibase=(void*)dispatch;
	ip = (token*)&dispatch[0];
	JUMP;
	#include "gen/run_repl_switch.h"
}

#undef NEXT
#undef JUMP

// === RUN SWITCH =====================================

#define NEXT			ip += 1; break
#define JUMP			break

void run_switch() {
	for (;;) {
		switch (*ip) {
			#include "gen/run_switch.h"
			default: NEXT;
		}
	}
}

#undef NEXT
#undef JUMP

// === RUN REPLICATED SWITCH =============================

#if REPL_SWITCH

#include "gen/repl_switch.h"

void run_repl_switch() {
	JUMP;
	#include "gen/run_repl_switch.h"
}

#undef NEXT
#undef JUMP

#endif

// === RUN COMPILED CALL ================================

#define NEXT			
#define JUMP			goto *dispatch[ip-ibase]

void run_compiled_call() {
	void *dispatch[] = {
		#include "gen/dispatch_comp.h"
	};
	JUMP;
	#include "gen/compiled_call.h"
}

#undef NEXT
#undef JUMP


// === RUN COMPILED INLINE ==============================

#define NEXT			ip += 1
#define JUMP			goto *dispatch[ip-ibase]

void run_compiled_inline() {
	void *dispatch[] = {
		#include "gen/dispatch_comp.h"
	};
	JUMP;
	#include "gen/compiled_inline.h"
}

#undef NEXT
#undef JUMP
