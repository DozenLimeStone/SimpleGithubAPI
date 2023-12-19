#pragma once
#include <stdlib.h>
#include <string.h>
#include "strnstr.h"

int jsonfindpairindex(const char* data,const char* key){
	int numpairs=0;
	int curr=0;
	int insideanother=0;
	const int keylen=strlen(key);
	for(int i=0;i<strlen(data);i++){
		char ch = data[i];
		if(ch==key[curr]){
			if(curr==keylen-1&&data[i+1]=='"'&&data[i+2]==':'){
				return numpairs;
			}curr++;
		}else if(curr>0){
			curr=0;
		}if(ch=='"'){
			insideanother=!insideanother;
		}else if(ch==':'&&!insideanother){
			numpairs++;
		}
	}
	return -1;
}

char* jsongetpairvalue(const char* data,const int pairindex){
	int insideanother=0;
	int numpairs=0;
	const int datalen=strlen(data);
	for(int i=0;i<datalen;i++){
		char ch = data[i];
		if(ch=='"'){
			insideanother=!insideanother;
		}else if(ch==':'&&!insideanother){
			if(numpairs==pairindex){
				char* foot = strnstr(&data[i],"\"",datalen-i);
				const int footjum=foot-data-i+1;
				i+=footjum;
				char* out = strnstr(&data[i],"\"",datalen-i);
				const int jumpam = out-data-i;
				char* value=malloc(jumpam+1);
				strncpy(value,&data[i],jumpam);
				value[jumpam]='\0';
				return value;
			}
			numpairs++;
		}
	}
	return NULL;
}
