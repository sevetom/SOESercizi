#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "def.h"
#include "A.h"

int main(void){ 
	SD A={33.666}, B; 

	B = fA( &A );
	printf("%f\n", B.x );
        if( B.x > 1.1 ) {
		printf("se");
                return( 1 );
	}
        else {
		printf("ses");
                return( 0 );
	}
}


