#include <stdio.h>

#define IN   1   /* inside a word */
#define OUT  0   /* outside a word */


char reverse(int lenght, char arr[]) {

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
    i = 0;
    char c, word[100];
    while ((c = getchar()) != EOF) {
  
	if (c == ' ' || c == '\n' || c == '\t'){

	    state = OUT;
        printf("%s", word);
        i = 0;
	}
    else if (state == OUT) {

	        state = IN;
            i = 0;
            

	    } else {
            word[i] = c;
            i++;
        }

    }


    return 0;
}