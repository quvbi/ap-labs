#include <stdio.h>

int main(){
    // Place your magic here
    return 0;
}
#include <stdio.h>

#define IN   1   /* inside a word */
#define OUT  0   /* outside a word */


char *reverse(int lenght, char arr[]) {

    int i, tmp;

    for (i = 0;  i < lenght/2; i++) {
	tmp = arr[i];
	arr[i] = arr[lenght - i - 1];
	arr[lenght - i - 1] = tmp;
    }

    return arr;
}

int main()

{
    int i, state;
    state = OUT;
    i = -1;
    char c, word[100];
    while ((c = getchar()) != EOF) {
  if (state == OUT) {

	        state = IN;
            i = 0;
                   
	    }
	else if (c == ' ' || c == '\n' || c == '\t'){

	    state = OUT;
        printf("%s", reverse(i,word));
     //   printf("%d",i);
        //i = 0;
	} else {

            word[i] = c;
            i++;

        }

    }


    return 0;
}