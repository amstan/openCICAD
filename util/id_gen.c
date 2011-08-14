#include <stdio.h>
void main(void) {
	unsigned int id;
	
	printf("Generates up to 3 bytes for the id when sending a packet.\n");
	printf("id=");
	scanf("%d",&id);
	
	//ID
	unsigned char id2, id1, id0;
	#define MORE_ID 0b10000000;
	id0=id; id>>=8;
	if(id>0) {
		id1=id | MORE_ID; id>>=7;
		if(id>0) {
			id2=id | MORE_ID;
			printf("#%d# ",id2);
		}
		printf("#%d# ",id1);
	}
	printf("#%d#\n",id0);
	
}