#include <stdio.h>
#include <stdlib.h>
         /* step size */

/* print Fahrenheit-Celsius table */

int main(int argc, char **argv)
{
 
 if(argv[2]== NULL){
printf("Fahrenheit: %3d, Celcius: %6.1f\n", atoi(argv[1]), (5.0/9.0)*(atoi(argv[1])-32));
 } else{
  int LOWER = 0;   /* lower limit of table */
  int  UPPER  = atoi(argv[2]);     /* upper limit */
  int STEP  = atoi(argv[3]);
  int fahr = atoi(argv[1]);



    for (fahr = LOWER; fahr <= UPPER; fahr = fahr + STEP){
   
	printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
    }
  
  }
    return 0;


}