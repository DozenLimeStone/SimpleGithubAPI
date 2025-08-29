#pragma once
#include <stdlib.h>
char* strremovestr(const char* data,const char* what){
    const size_t ws=strlen(what);
    const size_t ds=strlen(data);
    size_t currsize=ds;
    char* newdata=malloc(currsize+1);
    strcpy(newdata,data);
    const char* hehe=NULL;
    while((hehe=strstr(newdata,what))){
    size_t detent=hehe-newdata;
    memmove(newdata+detent,newdata+detent+ws,currsize-detent-ws+1);
    currsize-=ws;
    }
    newdata=realloc(newdata,currsize+1);
    newdata[currsize]='\0';
    return newdata;
}
