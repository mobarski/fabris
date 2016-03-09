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
	
	op_fmul:
		wtmp = sp[-1] * sp[0] / fpb;
		sp[-1] = wtmp;
		sp -= 1;
		NEXT;

	op_fdiv:
		wtmp = sp[-1] * fpb / sp[0] ;
		sp[-1] = wtmp;
		sp -= 1;
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
	
	op_not:
		sp[0] = sp[0] ? FALSE : TRUE ;
		NEXT;
	
// === COMPARATORS ========
	
	op_zero:
		sp[1] = sp[0] ? FALSE : TRUE ;
		sp += 1;
		NEXT;
		
	op_positive:
		sp[1] = sp[0]>0 ? TRUE : FALSE ;
		sp += 1;
		NEXT;
		
	op_negative:
		sp[1] = sp[0]<0 ? TRUE : FALSE ;
		sp += 1;
		NEXT;
		
	op_less:
		sp[0] = sp[-1]<sp[0] ? TRUE : FALSE ;
		NEXT;

	op_or_less:
		sp[0] = sp[-1]<=sp[0] ? TRUE : FALSE ;
		NEXT;

	op_more:
		sp[0] = sp[-1]>sp[0] ? TRUE : FALSE ;
		NEXT;

	op_or_more:
		sp[0] = sp[-1]>=sp[0] ? TRUE : FALSE ;
		NEXT;

	op_equal:
		sp[0] = sp[-1]==sp[0] ? TRUE : FALSE ;
		NEXT;
	
	op_within:
		sp[-1] = ((sp[-1]<=sp[-2]) & (sp[-2]<=sp[0])) ? TRUE : FALSE ;
		sp -= 1;
		NEXT;
