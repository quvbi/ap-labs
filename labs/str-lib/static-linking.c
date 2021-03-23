#include <stdio.h>

int main(int argc, char **argv) {
    if(argc <= 3){
        printf("Not enough arguments\n");
    } else {

    if(strcmp(argv[1],"-add") == 0){
        printf("Initial Lenght      : %d\n", mystrlen(argv[2]));
        mystradd(argv[2], argv[3]);
        printf("New String          : %s\n", argv[2]);
        printf("New length          : %d\n", mystrlen(argv[2]));
    }

       if(strcmp(argv[1],"-find") == 0){
       printf("['%s'] string was found at [%d] position\n", argv[3], mystrfind(argv[2], argv[3]));
    }
    }
    

    return 0;
}
