
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

	// === MORE STACK ==============
	op_dup2:
		sp[2] = sp[0];
		sp[1] = sp[-1];
		sp += 2;
		NEXT;
	op_swap2:
		tmp=sp[0];
		sp[0]=sp[-2];
		sp[-2]=tmp;
		tmp=sp[-1];
		sp[-1]=sp[-3];
		sp[-3]=tmp;
		NEXT;
	op_drop2:
		sp -= 2;
		NEXT;
	op_drop4:
		sp -= 4;
		NEXT;
	op_ndrop:
		sp -= 1+sp[0];
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
	op_tos:
		sp[1] = (uint)rp[0];
		sp += 1;
		rp += 1;
		NEXT;
	op_reverse:
		for (i=0;i<sp[0]>>1;i++) {
			tmp = sp[-i-1];
			sp[-i-1] = sp[-sp[0]+i];
			sp[-sp[0]+i] = tmp;
		}
		sp -= 1;
		NEXT;
	op_mark:
		rp[-1] = (token*)(sp-sbase);
		rp -= 1;
		NEXT;
	op_count:
		sp[1] = (int)(token*)(sp-sbase)-(int)rp[0];
		rp += 1;
		sp += 1;
		NEXT;
	op_push:
		for (i=0;i<sp[0];i++) {
			rp[-1-i] = (token*) sp[-1-i];
		}
		rp -= sp[0];
		sp -= sp[0]+1;
		NEXT;
	op_pop:
		tmp = sp[0];
		for (i=0;i<tmp;i++) {
			sp[i]=(int)rp[i];
		}
		rp += tmp;
		sp += tmp-1;
		NEXT;
	op_revpop:
		tmp = sp[0];
		for (i=0;i<tmp;i++) {
			sp[i]=(int)rp[tmp-i-1];
		}
		rp += tmp;
		sp += tmp-1;
		NEXT;
	op_pick:
		sp[0] = sp[-sp[0]-1];
		NEXT;