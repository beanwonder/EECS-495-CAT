#include <stdio.h>
#include <CAT.h>

void CAT_execution (void){
	CATData	d1;
	CATData	d2;
	CATData	d3;

	d1	= CAT_create_signed_value(5);
	printf("H1: 	Value 1 = %ld\n", CAT_get_signed_value(d1));
	d2	= CAT_create_signed_value(8);
	printf("H1: 	Value 2 = %ld\n", CAT_get_signed_value(d2));

	d3	= CAT_create_signed_value(0);
	CAT_binary_add(d3, d1, d2);
	printf("H1: 	Result = %ld\n", CAT_get_signed_value(d3));

	return ;
}

int main (int argc, char *argv[]){
	printf("H1: Begin\n");
	CAT_execution();
	printf("H1: End\n");

	return 0;
}
