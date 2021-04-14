#include <stdio.h>
#include <stdarg.h>
#include <signal.h>

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
int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);

void textcolor(int attr, int fg, int bg);

void textcolor(int attr, int fg, int bg)
{	char command[13];

	/* Command is the control command to the terminal */
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}

int infof(const char *format, ...){
     va_list args;
     va_start(args, format);
     printer("information: ", format, BRIGHT, BLUE, 7, args);
     fflush(stdout);
     va_end(args);

}

int errorf(const char *format, ...){
     va_list args;
     va_start(args, format);
     printer("error: ", format, RED, BRIGHT, UNDERLINE, args);
     fflush(stdout);
     va_end(args);
}

int warnf(const char *format, ...){
    va_list args;
     va_start(args, format);
     printer("warning: ", format, UNDERLINE, 7, RED, args);
     fflush(stdout);
     va_end(args);
}

int panicf(const char *format, ...){
     va_list args;
     va_start(args, format);
     printer("panic: ", format, BLINK, MAGENTA, GREEN, args);
     fflush(stdout);
     va_end(args);
}

void printer(char *type, const char * format, int attr, int fg, int bg, va_list args) {
    textcolor(attr, fg, bg);

    printf("%s", type);
    textcolor(BLACK, -1, -1);
    printf(" ");
    vprintf(format, args);


}







