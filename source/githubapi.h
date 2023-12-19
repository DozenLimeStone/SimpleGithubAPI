#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct requestdata{
    char* token;
    char* link;
};

void initrequest(){
	curl_global_init(CURL_GLOBAL_ALL);
}

void settoken(struct requestdata* data,const char ntoken[]){
	data->token=(char*)malloc(strlen(ntoken)+1);
	strcpy(data->token,ntoken);
}

void setlink(struct requestdata* data,const char nowner[],const char nrepo[]){
	char githublink[] ="https://api.github.com/repos/%s/%s/contents/";
	data->link=(char*)malloc(strlen(githublink)+strlen(nowner)+strlen(nrepo)-4);
	sprintf(data->link,githublink,nowner,nrepo);
}

void freetoken(struct requestdata* data){free(data->token);}
void freelink(struct requestdata* data){free(data->link);}
void cleanrequest(){
	curl_global_cleanup();
}

static size_t writecallback(char* contents, size_t size, size_t nmemb, char** stream){
	size_t tsize=size*nmemb;
	(*stream)=(char*)realloc(*stream,tsize);
	strncpy(*stream,contents,tsize);
	return tsize;
}

void setrequest(CURL* curl,struct curl_slist* list,struct requestdata* data,const char* requesttype,const char* path){
	char authtext[] = "Authorization: token ";
	char *buffa=(char*)malloc(strlen(data->token)+strlen(authtext));
	sprintf(buffa,"%s%s",authtext,data->token);
	list = curl_slist_append(list,buffa);
	list = curl_slist_append(list,"User-Agent: ABCDEF");
	list = curl_slist_append(list,"Accept: application/vnd.github+json");
	list = curl_slist_append(list,"X-GitHub-Api-Version: 2022-11-28");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

	char buff[strlen(data->link)+strlen(path)];
	sprintf(buff,"%s%s",data->link,path);
	curl_easy_setopt(curl, CURLOPT_URL, buff);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, requesttype);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writecallback);
}

void request(CURL* curl){
	CURLcode res = curl_easy_perform(curl);
	if(res!=CURLE_OK){
		printf("Failed performing actions,\n%s\n", curl_easy_strerror(res));
	}
}

char* getfile(struct requestdata* data,const char* path){
	CURL* curl=curl_easy_init();
	struct curl_slist* header=NULL;

	char* output=malloc(0);

	setrequest(curl,header,data,"GET",path);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);

	request(curl);

	curl_slist_free_all(header);
	curl_easy_cleanup(curl);

	return output;
}

char* createfile(struct requestdata* data,const char* path,const char* message,const char* content){
	CURL* curl=curl_easy_init();
	struct curl_slist* header=NULL;
	char* output=malloc(0);

	setrequest(curl,header,data,"PUT",path);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);

	char postformat[] = "{\"message\":\"%s\",\"content\":\"%s\"}";
	char* posttext=malloc(strlen(postformat)+strlen(message)+strlen(content)-4);
	sprintf(posttext,postformat,message,content);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, posttext);

	request(curl);

	curl_slist_free_all(header);
	curl_easy_cleanup(curl);
	free(posttext);

	return output;
}

char* updatefile(struct requestdata* data,const char* path,const char* message,const char* content,const char* sha){
	CURL* curl=curl_easy_init();
	struct curl_slist* header=NULL;

	char* output=malloc(0);

	setrequest(curl,header,data,"PUT",path);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);

	char postformat[] = "{\"message\":\"%s\",\"content\":\"%s\",\"sha\":\"%s\"}";
	char* posttext=malloc(strlen(postformat)+strlen(message)+strlen(content)+strlen(sha)-6);
	sprintf(posttext,postformat,message,content,sha);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, posttext);

	request(curl);

	curl_slist_free_all(header);
	curl_easy_cleanup(curl);
	free(posttext);

	return output;
}

char* deletefile(struct requestdata* data,const char* path,const char* message,const char* sha){
	CURL* curl=curl_easy_init();
	struct curl_slist* header=NULL;
	char* output=malloc(0);

	setrequest(curl,header,data,"DELETE",path);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);

	char postformat[] = "{\"message\":\"%s\",\"sha\":\"%s\"}";
	char* posttext=malloc(strlen(postformat)+strlen(message)+strlen(sha)-4);
	sprintf(posttext,postformat,message,sha);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, posttext);

	request(curl);

	curl_slist_free_all(header);
	curl_easy_cleanup(curl);
	free(posttext);

	return output;
}
