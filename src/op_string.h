	// === STRING ===========================
	op_begins:
		if (sp[0]>sp[-2]) {
			sp[-1]=0;
		} else {
			tmp=1;
			for (i=0;i<sp[0];i++) {
				if (((char*)sp[-3])[i] != ((char*)sp[-1])[i]) {
					tmp=0;
					break;
				}
			}
			sp[-1]=tmp;
		}
		sp -= 1;
		NEXT;
	op_ends:
		if (sp[0]>sp[-2]) {
			sp[-1]=0;
		} else {
			tmp=1;
			for (i=0;i<sp[0];i++) {
				if (((char*)sp[-3])[sp[-2]-1-i] != ((char*)sp[-1])[sp[0]-1-i]) {
					tmp=0;
					break;
				}
			}
			sp[-1]=tmp;
		}
		sp -= 1;
		NEXT;
	op_substr:
		sp[-1] += sp[-3];
		sp[0] = sp[0];
		NEXT;
	op_index:
		tmp=-1;
		if (sp[0]<=sp[-2]) {
			for (i=0;i<sp[-2]-sp[0]+1;i++) {
				tmp=i;
				for (j=0;j<sp[0];j++) {
					if (((char*)sp[-3])[i+j] != ((char*)sp[-1])[j]) {
						tmp = -1;
						break;
					}
				}
				if (tmp>=0) break;
			}
		}
		sp[-1] = tmp;
		sp -= 1;
		NEXT;
	op_lstrip:
		for (i=0;i<sp[0];i++) {
			if (((char*)sp[-1])[i] > 32) break;
		}
		sp[-1] += i;
		sp[0] -= i;
		NEXT;
	op_rstrip:
		for (i=0;i<sp[0];i++) {
			if (((char*)sp[-1])[sp[0]-i-1] > 32) break;
		}
		sp[0] -= i;
		NEXT;
	op_strip:
		// rstrip
		for (i=0;i<sp[0];i++) {
			if (((char*)sp[-1])[sp[0]-i-1] > 32) break;
		}
		sp[0] -= i;
		// lstrip
		for (i=0;i<sp[0];i++) {
			if (((char*)sp[-1])[i] > 32) break;
		}
		sp[-1] += i;
		sp[0] -= i;
		NEXT;
	op_split:
		// lstrip
		for (i=0;i<sp[0];i++) {
			if (((char*)sp[-1])[i] > 32) break;
		}
		sp[-1] += i;
		sp[0] -= i;
		// reversed lstrip
		for (j=0;j<sp[0];j++) {
			if (((char*)sp[-1])[j] <= 32) break;
		}
		sp[1] = sp[-1];
		sp[2] = j;
		sp[-1] = sp[-1]+j;
		sp[0] -= j;
		sp += 2;
		NEXT;
	op_char:
		if (sp[0]>=0) {
			sp[0] = ((char*)sp[-2])[sp[0]];
		} else {
			sp[0] = ((char*)sp[-2])[sp[-1]+sp[0]];
		}
		NEXT;
		