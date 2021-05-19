#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <string.h>
#include <syslog.h>
#include "logger.h"
#define RESET		0
#define BRIGHT 		1
#define DIM		2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8

#define BLACK 		0
#define RED		1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6

int typeLog =0;


void textcolor(int attr, int fg, int bg);

void textcolor(int attr, int fg, int bg)
{	char command[13];

	/* Command is the control command to the terminal */
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}

int initLogger(char *logType){
    if( strcmp(logType, "stdout")== 0  || strcmp(logType, "") == 0 ){
        typeLog = 0;
    } else if(strcmp(logType, "syslog") == 0){
        typeLog = 1;
    } else {
        perror("error: you have to specify the type");
        return 1;
    }

    printf("Logger is gettig initialiazed in: %s\n", logType);

}
int infof(const char *format, ...){
     va_list args;
     va_start(args, format);
     printer("information: ", format, BRIGHT, BLUE, 7, args);
     if(typeLog){
         openlog("INFO", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1 );
         vsyslog(LOG_INFO, format, args);
     }
      else {
          vfprintf(stdout,format,args);

      }
     va_end(args);

     if(typeLog == 0){
         fprintf(stdout, "\n");
              fflush(stdout);

     }


     textcolor(RESET, 7, BLACK);
        return 0;

}

int errorf(const char *format, ...){
     va_list args;
     va_start(args, format);
     printer("error: ", format, RED, BRIGHT, UNDERLINE, args);
     if(typeLog){
         openlog("ERROR", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1 );
         vsyslog(LOG_ERR, format, args);
         closelog();
     }
      else {
          vfprintf(stdout,format,args);

      }
     va_end(args);

     if(typeLog == 0){
         fprintf(stdout, "\n");
              fflush(stdout);

     }


     textcolor(RESET, 7, BLACK);
        return 0;
}

int warnf(const char *format, ...){
    va_list args;
     va_start(args, format);
     printer("warning: ", format, UNDERLINE, 7, RED, args);
      if(typeLog){
         openlog("WARN", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1 );
         vsyslog(LOG_WARNING, format, args);
         closelog();
     }
      else {
          vfprintf(stdout,format,args);

      }
     va_end(args);

     if(typeLog == 0){
         fprintf(stdout, "\n");
              fflush(stdout);

     }
   textcolor(RESET, 7, BLACK);
        return 0;
    
}

int panicf(const char *format, ...){
     va_list args;
     va_start(args, format);
     printer("panic: ", format, BLINK, MAGENTA, GREEN, args);
     if(typeLog){
         openlog("PANIC", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1 );
         vsyslog(LOG_EMERG, format, args);
         closelog();
     }
      else {
          vfprintf(stdout,format,args);

      }
     va_end(args);

     if(typeLog == 0){
         fprintf(stdout, "\n");
              fflush(stdout);

     }


     textcolor(RESET, 7, BLACK);
        return 0;
}

void printer(char *type, const char * format, int attr, int fg, int bg, va_list args) {
    textcolor(attr, fg, bg);

    printf("%s", type);
    textcolor(BLACK, -1, -1);
    printf(" ");
    vprintf(format, args);


}
