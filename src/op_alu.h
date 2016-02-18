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
		
	op_plus:
		sp[1] = sp[0]>0 ? 1 : 0 ;
		sp += 1;
		NEXT;
		
	op_minus:
		sp[1] = sp[0]<0 ? 1 : 0 ;
		sp += 1;
		NEXT;
		
	op_less:
		sp[0] = sp[-1]<sp[0] ? 1 : 0 ;
		NEXT;
		
	op_more:
		sp[0] = sp[-1]>sp[0] ? 1 : 0 ;
		NEXT;
		
	op_equal:
		sp[0] = sp[-1]==sp[0] ? 1 : 0 ;
		NEXT;
