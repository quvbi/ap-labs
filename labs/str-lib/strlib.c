

int mystrlen(char *str){
    int c = 0;
     while (str[c] != '\0'){
    c++;
     }
    return c;
}

char *mystradd(char *origin, char *addition){
       while(*origin)
origin++;
while(*addition)
{
*origin = *addition;
addition++;
origin++;
}
*origin = '\0';

return origin;
}

int mystrfind(char *origin, char *substr){
  const char *st = origin; 
const char *pa = substr; 
while (*st){ 
    ++st;
    if( *st == *pa){ 
        int i = 1; 

        for(i;*st == *pa;i++){ 
            ++st;
            ++pa;
        }

        if(*pa == 0){ 
            return (st-i)-origin ; 
        }

        pa-i; 
        st-i; 
    }
}
return -1;
  
    
}
