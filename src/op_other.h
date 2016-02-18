
// === OTHER ==================

	op_nop:
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
		
	op_sprint:
		printf("depth:%u  ->  ",sp-sbase);
		printf("[%d] ",sp[0]);
		for (i=-1; i>=sbase-sp; i--)
			printf("%d ",sp[i]);
		printf("\n");
		NEXT;
		
	op_ok:
		if (sp[0]==sp[-1]) { sp-=2; // (xy--)
		} else {
			printf("ERROR: test not ok at ip=%d, expected %d got %d\n",ip-ibase,sp[0],sp[-1]);
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
		
	op_hash:
		utmp = 0;
		for (i=0; i<sp[0]; i++) {
			utmp *= 65599;
			utmp += (uchar)((uchar*)(sp[-1]))[i];
		}
		sp[-1] = utmp;
		sp -= 1;
		NEXT;
