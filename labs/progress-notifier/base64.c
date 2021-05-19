#include <stdio.h>
#include "logger.h"
#include <inttypes.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <math.h>

#define WHITESPACE 64
#define EQUALS     65
#define INVALID    66

int encode;
size_t completed;
size_t origin = 0;


static const unsigned char d[] = {
    66,66,66,66,66,66,66,66,66,66,64,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,63,52,53,
    54,55,56,57,58,59,60,61,66,66,66,65,66,66,66, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,66,66,66,66,66,66,26,27,28,
    29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66
};




void funSignal(){
    int progress = (float)completed / origin * 100;
    infof("%s is %d%% complete\n", encode ? "Encoding of the file" : "Decoding of the file", progress);
}

void signInit(){
    signal(SIGINT, funSignal);
    signal(SIGUSR1, funSignal);
}

int main(int argc, char **argv){
    if(argc != 3){
        errorf("input parameters are wrong\n");
        return 0;
    }

    signInit();
    char *srcFile, *srcBuff, *finalBuff;
    FILE *fileOG, *endFile;
    encode = strcmp(argv[1], "--decode")==0?0:1;
    srcFile = argv[2];

    if ((fileOG = fopen(srcFile, "r")) == NULL){
        errorf("source file opened failed\n");
        return 0;
    }
    struct stat stadistics;
    stat(srcFile, &stadistics);
    size_t dstLength = 4 * ((stadistics.st_size + 2) / 3.0) + 3;
    finalBuff = malloc(sizeof(char) * dstLength);
    srcBuff = malloc(sizeof(char) * stadistics.st_size);

    int line;
    char buf[BUFSIZ];
    while((line = fread(buf, sizeof(char), BUFSIZ, fileOG)) > 0){
        size_t toCopy;
        if(BUFSIZ > stadistics.st_size - origin){
            toCopy = stadistics.st_size - origin;
        }else{
            toCopy = BUFSIZ;
        }
        strncpy(srcBuff + origin, buf, toCopy);
        origin += line;
    }
    fclose(fileOG);

    int res= encode? base64encode(srcBuff, origin, finalBuff, dstLength) : 
                     base64decode(srcBuff, origin, (unsigned char *)finalBuff, &dstLength);
    
    if (res!=0){
        errorf("Something went wrong %s the file\n",encode? "encoding":"decoding");
        free(srcBuff);
        free(finalBuff);
        return 1;
    }
    char * endFileName = strchr(srcFile, '.');
    *endFileName=0;
    sprintf(endFileName, "%s-%s.txt", srcFile, encode ? "encoded" : "decoded");
    
    if((endFile = fopen(endFileName, "w")) == NULL){
        errorf("Unable to open the result file\n");
        free(srcBuff);
        free(finalBuff);
        return 1;
    }
    size_t maxRead;
    size_t lwritten;
    size_t totWritten = 0;
    size_t dstLen = strlen(finalBuff);
    while(totWritten < dstLen){
        if(BUFSIZ > dstLen - totWritten){
            maxRead = dstLen - totWritten;
        }else{
            maxRead = BUFSIZ;
        }
        if((lwritten = fwrite(finalBuff + totWritten, sizeof(char), maxRead, endFile)) != maxRead){
            errorf("ERROR In the result file\n");
            free(srcBuff);
            free(finalBuff);
            fclose(endFile);
            return 1;
        }

       totWritten += lwritten; 
    }
    fclose(endFile);
    free(srcBuff);
    free(finalBuff);
    return 0;
}

// code stracted from https://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64#C
int base64encode(const void* data_buf, size_t dataLength, char* result, size_t resultSize)
{
   const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
   const uint8_t *data = (const uint8_t *)data_buf;
   size_t resultIndex = 0;
   uint32_t n = 0;
   int padCount = dataLength % 3;
   uint8_t n0, n1, n2, n3;

   /* increment over the length of the string, three characters at a time */
   for (completed= 0; completed < dataLength; completed += 3) 
   {
      /* these three 8-bit (ASCII) characters become one 24-bit number */
      n = ((uint32_t)data[completed]) << 16; //parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0
      
      if((completed+1) < dataLength)
         n += ((uint32_t)data[completed+1]) << 8;//parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0
      
      if((completed+2) < dataLength)
         n += data[completed+2];

      /* this 24-bit number gets separated into four 6-bit numbers */
      n0 = (uint8_t)(n >> 18) & 63;
      n1 = (uint8_t)(n >> 12) & 63;
      n2 = (uint8_t)(n >> 6) & 63;
      n3 = (uint8_t)n & 63;
            
      /*
       * if we have one byte available, then its encoding is spread
       * out over two characters
       */
      if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
      result[resultIndex++] = base64chars[n0];
      if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
      result[resultIndex++] = base64chars[n1];

      /*
       * if we have only two bytes available, then their encoding is
       * spread out over three chars
       */
      if((completed+1) < dataLength)
      {
         if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
         result[resultIndex++] = base64chars[n2];
      }

      /*
       * if we have all three bytes available, then their encoding is spread
       * out over four characters
       */
      if((completed+2) < dataLength)
      {
         if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
         result[resultIndex++] = base64chars[n3];
      }
   }

   /*
    * create and add padding that is required if we did not have a multiple of 3
    * number of characters available
    */
   if (padCount > 0) 
   { 
      for (; padCount < 3; padCount++) 
      { 
         if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
         result[resultIndex++] = '=';
      } 
   }
   if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
   result[resultIndex] = 0;
   return 0;   /* indicate success */
}

// code stracted from https://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64#C
int base64decode (char *in, size_t inLen, unsigned char *out, size_t *outLen) { 
    char *end = in + inLen;
    char iter = 0;
    uint32_t buf = 0;
    size_t len = 0;
    
    completed = 0;
    while (in < end) {
        completed++;
        unsigned char c = d[*in++];
        
        switch (c) {
        case WHITESPACE: continue;   /* skip whitespace */
        case INVALID:    return 1;   /* invalid input, return error */
        case EQUALS:                 /* pad character, end of data */
            in = end;
            continue;
        default:
            buf = buf << 6 | c;
            iter++; // increment the number of iteration
            /* If the buffer is full, split it into bytes */
            if (iter == 4) {
                if ((len += 3) > *outLen) return 1; /* buffer overflow */
                *(out++) = (buf >> 16) & 255;
                *(out++) = (buf >> 8) & 255;
                *(out++) = buf & 255;
                buf = 0; iter = 0;
            }   
        }
    }
   
    if (iter == 3) {
        if ((len += 2) > *outLen) return 1; /* buffer overflow */
        *(out++) = (buf >> 10) & 255;
        *(out++) = (buf >> 2) & 255;
    }
    else if (iter == 2) {
        if (++len > *outLen) return 1; /* buffer overflow */
        *(out++) = (buf >> 4) & 255;
    }

    *outLen = len; /* modify to reflect the actual output size */
    return 0;
}

