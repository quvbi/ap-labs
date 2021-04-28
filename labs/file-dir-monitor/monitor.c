#include <stdio.h>
#include "logger.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ftw.h>
#include <unistd.h>
#include <string.h>
#include <sys/inotify.h>

#define BFERLENGTH (10* ( sizeof(struct inotify_event) + 1000  + 1))

int Wd;
int Fd;

struct ws {
    int Wd;
    char name[1000];

};

int in = 0;
int size = 0;
struct ws wd_arraY[(10* (sizeof(struct ws) + 1000 +1))];

static void printInstance(struct inotify_event *i){
    infof("wd: %d detected event: %s in file %s", i->wd, i->mask & IN_CREATE ? "IN CREATE" : i->mask & IN_DELETE ? "IN DELETE" :  i->mask & IN_MODIFY ? "IN MODIFY" : "???", i->name);  
         }

    char * find (int Wd) {
        int i;
            for (i = 0; i < size; i++) {
        if (wd_arraY[i].Wd == Wd) {
            return wd_arraY[i].name;
        }
            }
        return NULL;
    }

    static int addWd(const char *filePath, const struct stat *sb, int type){
        if(type != FTW_D){
            return 0;
        }

        int watchD;
        if((watchD = inotify_add_watch(Fd, filePath, IN_CREATE | IN_DELETE | IN_MODIFY)) == -1){
            errorf("ERROR CREATING WATCH DESCP.");
            return -1;
        }

    createWd(filePath, watchD);
    return 0;
        
    }
    

    void createWd( const char * name, int Wd){
        struct ws wsArr;
        strcpy(wsArr.name, name);
        wsArr.Wd = Wd;
        wd_arraY[in++] = wsArr;
        size++;
    }


    

 
    
int main(int argc, char** argv){

       if(argc < 2){
        errorf("ERROR in input arg");
        exit(1);
    }

    const char *path;
    char buffer[BFERLENGTH];
    ssize_t numrd;
    char *pr;
    struct inotify_event *event;

    infof("Init inotify");
    path = argv[1];
    Fd = inotify_init();
    
    if(Fd== -1){
        errorf("Error: failure at the inotify initialization");
        exit(1);
    }

    if(ftw(path, addWd, 100) == -1){
        errorf("Error: failed at traversing the file tree.");
        exit(1);
    }

    for(;;){
        if((numrd = read(Fd, buffer, BFERLENGTH)) == 0){
            errorf("Error: failed reading inotify file descriptor.");
            exit(1);
        }

        if(numrd == -1){
            errorf("Error: failed reading inotify file descriptor.");
            exit(1);
        }

        for(pr = buffer; pr < buffer + numrd;){
            event = (struct inotify_event * ) pr;
            printInstance(event);
            pr += sizeof(struct inotify_event) + event->len;
        }
    }

    return 0;
}
