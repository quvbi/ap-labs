#include <stdio.h>

#define   LOWER  0       /* lower limit of table */
#define   UPPER  300     /* upper limit */
#define   STEP   20      /* step size */

/* print Fahrenheit-Celsius table */

int main(int argc, int **argv)
{
    int i;

    for (i = 0; i < argc; i++){
	printf("Fahrenheit: %3d, Celcius: %6.1f\n", argv[i], (5.0/9.0) * (argv[i]-32) );
    }
    printf("\n");

    return 0;

}