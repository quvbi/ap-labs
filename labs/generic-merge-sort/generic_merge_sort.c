#include <stdio.h>
#include <string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>  
#include <unistd.h>  
#include <stdlib.h>  
int main(int argc, char **argv)
{
    if(argc <= 1){
        
         printf("error: Incomplete paramteres\n");
         return 0;
    } else
    if(strcmp(argv[1],"-n") == 0 && argv[2] != NULL){
         numericMergeSort(argv[2]);
          
     } else if(access(argv[1],F_OK) == 0){
         stringMergeSort(argv[1]);
     }else {
         printf("error: Incomplete paramteres\n");
         return 0;
     }

    return 0;
}


void numericMergeSort(char *ofile){
 printf("Results file can be found at ./sorted_numbers.txt\n");
   int file = open(ofile, O_RDONLY);
    if(file != -1){
       FILE * fPointer;
       fPointer = fopen(ofile,"r");  
        char line[100];
        int size = 0;
           while(!feof(fPointer)){
            fgets(line,100,fPointer);
               size++;

           }
       fPointer = fopen(ofile,"r");  

        int numbers[size];
        int n = 0;
   while(!feof(fPointer)){
           int num = 0;
           fgets(line,100,fPointer);
           int len = strlen(line);

            num = atoi(line);
        numbers[n] = num;
        n++;
          
		}

        mergeSort(numbers, 0, size -1);

fclose(fPointer);

       FILE *fnew = fopen("sorted_numbers.txt","w");

        int j;
        for (j=0; j < size; j++)
            fprintf(fnew, "%d\n", numbers[j]);
        

              fclose(fnew);
     close(file);

    } else{
        perror("ERROR");
        exit(0);
    } 







    
}

void merge(int arreglo[], int l, int m, int r)
{

int num1 = m - l + 1;
int num2 = r - m;
int i, j, k;
int L[num1], R[num2];
    for (i = 0; i < num1; i++)
            L[i] = arreglo[l + i];
                for (j = 0; j < num2; j++)
                    R[j] = arreglo[m + 1+ j];
                    i = 0;
                    j = 0;
                    k = l;
        while (i < num1 && j < num2) {
        
        if (L[i] <= R[j]) {

        arreglo[k] = L[i];
        i++;
        } else {

        arreglo[k] = R[j];
        j++;
        }
        k++;
        }
    while (i < num1) {

    arreglo[k] = L[i];
        i++;
        k++;
    }
    while (j < num2){
    arreglo[k] = R[j];
    j++;
    k++;
    }
}

void mergeSort(int arreglo[], int l, int r) {
    if (l < r) {
    int m = l+(r-l)/2;
    mergeSort(arreglo, l, m);
    mergeSort(arreglo, m+1, r);
    merge(arreglo, l, m, r);
    }
}

void charmergeSort(char arreglo[100][100], int l, int r) {
    int mid;
if(l<r){
    mid=(l+r)/2;
    charmergeSort(arreglo,l,mid);
    charmergeSort(arreglo,mid+1,r);
    charMerge(arreglo,l,mid+1,mid,r); 
    }
}


void stringMergeSort(char *ofile){
 printf("Results file can be found at ./sorted_strings.txt\n");
   int file = open(ofile, O_RDONLY);
    if(file != -1){
       FILE * fPointer;
       fPointer = fopen(ofile,"r");  
        char line[100];
        int size = 0;
           while(!feof(fPointer)){
            fgets(line,100,fPointer);
               size++;

           }
       fPointer = fopen(ofile,"r");  

        char strings[size][100];
        int n = 0;
   while(!feof(fPointer)){
           char word[100];
           fgets(line,100,fPointer);
           int len = strlen(line);
            
            
			strcpy(strings[n],line);  
          //  printf("%s \n", strings[n]);
		
        n++;
          
		}
char temp[size][100];
for(int i=0,j=0;i<size;i++,j++){
    strcpy(temp[j],strings[i]);
}
        charmergeSort(temp, 0, size -1);

fclose(fPointer);

       FILE *fnew = fopen("sorted_strings.txt","w");

        int j;
        for (j=0; j < size; j++){

            fprintf(fnew, "%s", temp[j]);
        }
        

              fclose(fnew);
     close(file);

    } else{
        perror("ERROR");
        exit(0);
    } 


}

void charMerge(char arreglo[100][100],int l,int m,int m1,int r){
    char **arreglo1;

    int i,j ,k;
   
arreglo1=(char **)malloc((r-l+1)*sizeof(char *));
    for(i=0;i<r-l+1;i++){
        arreglo1[i]=(char *)malloc(100*sizeof(char));
        }
  j=m; 
  i=l; 
  k=0;
        while(i<=m1 && j<=r){

            if(strcmp(arreglo[i],arreglo[j])>0){ 
                strcpy(arreglo1[k++], arreglo[j++]);
            }
            else{
                strcpy(arreglo1[k++], arreglo[i++]);
            }
        }

                while(i<=m1){            
                    strcpy(arreglo1[k++], arreglo[i++]);
                }

                while(j<=r){
                    strcpy(arreglo1[k++], arreglo[j++]);
                }

                for(i=l,j=0;j<k;i++,j++){
                    strcpy(arreglo[i], arreglo1[j]);
                }
}