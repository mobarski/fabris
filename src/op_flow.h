// === PUSH ==================

	op_pushc: //ARG 1
		sp[1] = (char)ip[1];
		sp += 1;
		ip += 2;
		JUMP;
		
	op_pushuc: //ARG 1
		sp[1] = (uchar)ip[1];
		sp += 1;
		ip += 2;
		JUMP;
		
	op_pushx: //ARG 1234
		sp[1] = ip[1];
		sp += 1;
		ip += 2;
		JUMP;
		
	op_pushs: //ARG 12.34
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
		
	op_skip: //ARG 12
		ip += (token)ip[1];
		JUMP;
		
	op_back: //ARG 12
		ip -= (token)ip[1];
		JUMP;
		
	op_callx: //ARG 1234
		UTMP_LOAD_4();
		rp[-1] = ip+2; // TODO vs token size
		rp -= 1;
		ip = ibase+utmp;
		JUMP;
		
	op_tailcallx: //ARG 1234
		UTMP_LOAD_4();
		ip = ibase+utmp;
		JUMP;
		
	op_times: //ARG 12
		if (rp[0]==0) {
			rp += 1;
			ip += (token)ip[1]+2;
		} else {
			rp[0] = (token*)((uint)rp[0]-1);
			//rp[0] -= 1;
			ip += 2;
		}
		JUMP;
	
	op_do: //ARG 12
		rp[-1] = (token*)(ip+ip[1]-ibase);
		rp -= 1;
		ip += 2;
		JUMP;
	
	op_break:
		ip = ibase+(int)rp[0];
		rp += 1;
		JUMP;

	op_skipz: //ARG 12
		if (sp[0]==0) {
			TMP_LOAD_4();
			ip+=tmp;
		} else {ip+=2;}
		sp -= 1;
		JUMP;
		
	op_lambda: //ARG 12
		sp[1] = (uint)(&ip[2]-ibase);
		sp += 1;
		ip += ip[1];
		JUMP;
		
	op_vars: // TEST  //ARG 1
		for (i=0;i<ip[1];i++) {
			rp[-1-i] = (token*)sp[-i];
		}
		sp -= ip[1];
		rp -= ip[1]+1;
		rp[0] = (token*)ip[1];
		ip += 2;
		JUMP;
		
	op_varv: // TEST  //ARG 1
		sp[1] = (int)rp[(int)rp[0]-(int)ip[1]];
		sp += 1;
		ip += 2;
		JUMP;
		
	op_call:
		rp[-1] = ip+1;
		ip = (token*)(sp[0]+ibase);
		rp -= 1;
		sp -= 1;
		JUMP;
		
	op_ret:
		ip=rp[0]; rp+=1;
		JUMP;
		
	op_retv: // TEST 
		rp += 1+(uint)rp[0];
		ip=rp[0]; rp+=1;
		JUMP;
		
// === ??? ============================

// TODO change to be able to run from ROM

	op_var:  //ARG 4
		ip[1] = sp[0];
		sp -= 1;
		ip += 2;
		JUMP;
		
	op_pushv: //ARG 4
		sp[1] = ibase[ip[1]];
		sp += 1;
		ip += 2;
		JUMP;

	op_into: //ARG 4
		ibase[ip[1]] = sp[0];
		sp -= 1;
		ip += 2;
		JUMP;
