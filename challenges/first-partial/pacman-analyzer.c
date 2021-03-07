#include <stdio.h>
#include <string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>  
#include <unistd.h>  
#include <stdlib.h>  

#include <errno.h>

#define REPORT_FILE "packages_report.txt"

void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./pacman-analizer.o \n");
	return 1;
    } 
    
    
     if(strcmp(argv[1],"-input") != 0){
         access(argv[1],F_OK);
         perror("ERROR");
         return 0;
     }
/*
     if(access(argv[2],F_OK) != 0){
         printf("ERROR: no such input data file exists\n");
         return 0;
     }
     */
    
  
    analizeLog(argv[2], argv[4]);
    

    return 0;
}

struct package{
       char title[30];
        char date_inst[30];
        char date_upg[30];
        char date_rem[30];
        int numUp;
};



void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);
    // Implement your solution here.
    int file = open(logFile, O_RDONLY);

    if(file != -1){
       FILE * fPointer;
       fPointer = fopen(logFile,"r");    
       char line[100];
           struct package pckgs[700];
          int pac =0;
       int PCMANcount = 0, ALPMcount = 0, ALPMSCRcount = 0, instCount = 0, remCount =0, upgdCount = 0, currPack = 0;
       char latest[30];
       char newest[30];
       int word;
       while(!feof(fPointer)){
           int b =0;
           int s = 0;
           fgets(line,100,fPointer);
             for(int l = 0; l < 100; l++){
            if(line[l] == ']'){
                b++;
            }
           if(line[l]!=' ' || (line[l] == ' ' && b ==0)){
                s++;
		}
             }
           char words[s][strlen(line)];
           int i=0;
           int brackets = 0;
        
           int n = 0;
           int k = 0;
           for(i = 0; i < 100; i++){
            if(line[i] == ']'){
                brackets++;
            }
           if(line[i]!=' ' || (line[i] == ' ' && brackets ==0)){
			words[n][k++]=line[i];  
		}
		else{
			words[n][k++]='\0';//insert NULL
			n++;
			k=0;
		}
		if(line[i]=='\0')
		    break;
	}

           
        //printf("%s \n",words[2]);
        if(strcmp(words[1],"[PACMAN]") == 0){
            PCMANcount++;
           } else if(strcmp(words[1],"[ALPM]") == 0){
            ++ALPMcount;
           } else if(strcmp(words[1],"[ALPM-SCRIPTLET]") == 0){
            ++ALPMSCRcount;
           }  
            if(strcmp(words[2],"installed") == 0){
            if(pac==0){
                strcpy(latest,words[3]);
            }
            strcpy(newest,words[3]);
            ++instCount;
            strcpy(pckgs[pac].title,words[3]);
           strcpy(pckgs[pac].date_inst,words[0]);

            pac++;
            

           }

           if(strcmp(words[2],"removed") == 0){
            ++remCount;
            for(int w = 0; w<pac; w++){
                if(strcmp(pckgs[w].title,words[3])==0){
                   strcpy(pckgs[w].date_rem,words[0]);
                }
            }
           }

           if(strcmp(words[2],"upgraded") == 0){
            ++upgdCount;

              for(int w = 0; w<pac; w++){
                if(strcmp(pckgs[w].title,words[3])==0){
                  strcpy(pckgs[w].date_upg,words[0]);
                  pckgs[w].numUp++;
                }
            }
           }

          

       } 
       
       fclose(fPointer);

       FILE *fnew = fopen(report,"w");
        
       fprintf(fnew,"----------------------\n");
       fprintf(fnew,"- Installed packages : %d  \n",instCount);
       fprintf(fnew,"- Removed packages   : %d  \n",remCount);
       fprintf(fnew,"- Upgraded packages  : %d  \n", upgdCount);
       fprintf(fnew,"- Current installed  : %d  \n",instCount-remCount);
       fprintf(fnew,"-------------\n");
       fprintf(fnew,"General Stats\n");
       fprintf(fnew,"-------------\n");
       fprintf(fnew,"- Oldest package               : %s\n",latest);
       fprintf(fnew,"- Newest package               : %s\n",newest);
       fprintf(fnew,"- Package with no upgrades     : "); 
       for(int w = 0; w<pac; w++){
           if(pckgs[w].numUp==0){
           fprintf(fnew, " %s",pckgs[w].title);
           }
       }
       fprintf(fnew,"\n");
       
       fprintf(fnew,"- [ALPM-SCRIPTTLET] type count : %d  \n",ALPMSCRcount);
       fprintf(fnew,"- [ALPM] count                 : %d  \n",ALPMcount);
       fprintf(fnew,"- [PACMAN] count               : %d  \n",PCMANcount);
       fprintf(fnew,"-------------\n");
       fprintf(fnew,"List of packages\n");
       fprintf(fnew,"-------------\n");
       
       for(int v = 0; v <pac; v++){
      fprintf(fnew,"- Package Name        : %s\n", pckgs[v].title);
      fprintf(fnew,"  - Install date      : %s\n", pckgs[v].date_inst);
      fprintf(fnew,"  - Last update date  : %s\n", pckgs[v].date_upg);
      fprintf(fnew,"  - How many updates  : %d\n", pckgs[v].numUp);
      fprintf(fnew,"  - Removal date      : %s\n", pckgs[v].date_rem);

      }

      fclose(fnew);
       

           



     close(file);
    } else{
        perror("ERROR");
        exit(0);
    } 

      
        printf("Report is generated at: [%s]\n", report);
}
