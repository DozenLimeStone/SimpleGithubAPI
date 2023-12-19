### Simple Github API made with C language.
##### _Please do not do pull requests, rather comment in issues._
##### _The code is in pretty bad structure. If you find any better solutions, do pull requests_

It works briefly for now.


**JSON support**: \
  - is in jsonfind.h header file \
  - for now contains only two functions: \
    - _int jsonfindpairindex(const char* data, const char* key)_ \
      - it finds pair index of given pair key \
      - returns -1 if it can't find \
    - _char* jsongetpairvalue(const char* data, const int pairindex)_ \
      - it finds pair value based on pair index \
      - returns NULL if it doesn't succeed \
  - I will add more function to jsonfind.h soon. \

**Base64 encryption**: \
  - is in base64.h header file \
  - got from https://nachtimwald.com/2017/11/18/base64-encode-and-decode-in-c/, slightly modified \
  - offers you some functions, important two are *base64_encode()* and *base64_decode()*, both accept _char*_ as input. \

**Github API**: \
  - is in githubapi.h header file \
  - requires linking curl to work. if you want to lecture me on how to make a library out of my project, \
    do comment in issues. \
  - every function you need to pass a _struct requestdata_ data type \
    - _struct requestdata_ contains: \
      - _char* token_ \
      - _char* link_ \
  - provides you with some basic functions: \
    - init ones: \
      - _void initrequest()_ initialization \
      - _void settoken(struct requestdata* data, const char ntoken[])_ sets the authentication token, \
        https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/managing-your-personal-access-tokens \
      - _void setlink(struct requestdata* data, const char nowner[], const char nrepo[])_ sets owner and repo to write in \
    - freeing ones: \
      - _void cleanrequest()_ cleans curl \
      - _void freetoken()_ frees token \
      - _void freelink()_ frees repo and owner \
    - usage ones: \
      - _char* getfile(struct requestdata* data,const char* path)_ \
      - _char* createfile(struct requestdata* data, const char* path, const char* message, const char* content)_ \
      - _char* updatefile(struct requestdata* data, const char* path, const char* message, const char* content,_ \
              _const char* sha)_ \
      - _char* deletefile(struct requestdata* data, const char* path, const char* message, const char* sha)_ \
      - additional sha argument is needed, you get it by using getfile() and then obtaining the sha value \
        using jsonfind.h functions. it is shown in example main file \

**Strnstr**: \
  - we should not forget about that! it is licensed, I got it from somewhere \
  - details in strnstr.h header file

That was it! Farther details are shown in example main file. \
This project will get updated, fixed over time.





