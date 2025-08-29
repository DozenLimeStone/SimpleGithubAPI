#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct requestdata{
    char* token;
    char* link;
};

struct userdata{
	char* data;
	int len;
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
	size_t lent=strlen(githublink)+strlen(nowner)+strlen(nrepo)-4;
	data->link=(char*)malloc(lent+1);
	sprintf(data->link,githublink,nowner,nrepo);
	data->link[lent]='\0';
}

void setdirectlink(struct requestdata* data,const char nlink[]){
	data->link=(char*)malloc(strlen(nlink));
	strcpy(data->link,&nlink[0]);
}

void setlink_silent(struct requestdata* data,const char nowner[],const char nrepo[],const char nbranch[]){
	char* githublink="https://raw.githubusercontent.com/%s/%s/raw/%s/";
	char* sentlink=malloc(strlen(githublink)-6+strlen(nowner)+strlen(nrepo)+strlen(nbranch));
	sprintf(sentlink,githublink,nowner,nrepo,nbranch);
	setdirectlink(data,sentlink);
}

void freetoken(struct requestdata* data){free(data->token);}
void freelink(struct requestdata* data){free(data->link);}
void cleanrequest(){
	curl_global_cleanup();
}

static size_t writecallback(const char* contents, size_t size, size_t nmemb, void* userdata) {
	char** stream=(char**)userdata;
	size_t csize=size*nmemb;
    size_t psize=strlen(*stream);
    size_t tsize=csize+psize;
    *stream=(char*)realloc(*stream,tsize+1);
    memcpy(*stream+psize,contents,csize);
    (*stream)[tsize]='\0';
    return csize;
}


void setrequest(CURL* curl,struct curl_slist** list,struct requestdata* data,const char* requesttype,const char* path){
	char authtext[] = "Authorization: token ";
	size_t hesite=strlen(data->token)+strlen(authtext);
	char *buffa=(char*)malloc(hesite+1);
	sprintf(buffa,"%s%s",authtext,data->token);
	buffa[hesite]='\0';
	*list = curl_slist_append(*list,buffa);
	*list = curl_slist_append(*list,"User-Agent: ABCDEF");
	*list = curl_slist_append(*list,"X-GitHub-Api-Version: 2022-11-28");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, *list);
	free(buffa);

	hesite=strlen(data->link)+strlen(path);
	char* buff=malloc(hesite+1);
	sprintf(buff,"%s%s",data->link,path);
	buff[hesite]='\0';

	curl_easy_setopt(curl, CURLOPT_URL, buff);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, requesttype);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writecallback);
	free(buff);
}

void request(CURL* curl){
	CURLcode res = curl_easy_perform(curl);
	if(res!=CURLE_OK){
		printf("Failed performing actions,\n%s\n", curl_easy_strerror(res));
	}
}

char* customrequest(struct requestdata* silent_data,const char* requesttype){ //this shit more special
	CURL* curl=curl_easy_init();
	struct curl_slist* header=NULL;

	char* output=malloc(1);
	output[0]='\0';

	curl_easy_setopt(curl, CURLOPT_URL, silent_data->link);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, requesttype);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writecallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);
	request(curl);

	curl_slist_free_all(header);
	curl_easy_cleanup(curl);
	return output;
}

char* getfile_silent(struct requestdata* silent_data,const char* path){
	CURL* curl=curl_easy_init();
	struct curl_slist* header=NULL;
	header=curl_slist_append(header,"Accept: application/vnd.github.v3.raw");

	char* output=malloc(1);
	output[0]='\0';
    // todo use private token GHSAT0AAAAAACMGSFIZCHDUKT6ZRJ4J4RFWZMSBJVQ
	setrequest(curl,&header,silent_data,"GET",path);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);

	request(curl);

	curl_slist_free_all(header);
	curl_easy_cleanup(curl);
	return output;
}

char* getfile(struct requestdata* data,const char* path) {
    CURL* curl = curl_easy_init();
    struct curl_slist* header=NULL;

    char* output=malloc(1);
	output[0]='\0';

    setrequest(curl,&header,data,"GET",path);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,&output);

    request(curl);

    curl_slist_free_all(header);
    curl_easy_cleanup(curl);

    return output;
}

char* createfile(struct requestdata* data,const char* path,const char* message,const char* content){
	CURL* curl=curl_easy_init();
	struct curl_slist* header=NULL;
	char* output=malloc(1);
	output[0]='\0';

	setrequest(curl,&header,data,"PUT",path);
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

	char* output=malloc(1);
	output[0]='\0';

	setrequest(curl,&header,data,"PUT",path);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);

	char postformat[] = "{\"message\":\"%s\",\"content\":\"%s\",\"sha\":\"%s\"}";
	size_t tlen=strlen(postformat)+strlen(message)+strlen(content)+strlen(sha)-6;
	char* posttext=malloc(tlen+1);
	posttext[tlen]='\0';
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
	char* output=malloc(1);
	output[0]='\0';

	setrequest(curl,&header,data,"DELETE",path);
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
