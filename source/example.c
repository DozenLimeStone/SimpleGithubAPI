#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strnstr.h"
#include "githubapi.h"
#include "jsonfind.h"
#include "base64.h"

int main(void){
	struct requestdata data;

	initrequest();
	settoken(&data,"token");
	setlink(&data,"owner","repo");

	// Create file
	const char* cfout =createfile(&data,"textfile.txt","Normal commit message.",base64_encode("HELLO"));

	// Update file
	const char* filec = getfile(&data,"textfile.txt"); // Get file
	const char* sha = jsongetpairvalue(filec,jsonfindpairindex(filec,"sha"));
	if(sha==NULL){ // Checks if sha is NULL, this means that it cannot be found.
		printf("%s\n",sha);
		return 0;
	}
	const char* ufout = updatefile(&data,"textfile.txt","Normal commit message.",base64_encode("HELLO from updatefile function."),sha);
	filec = getfile(&data,"textfile.txt");
	printf("%s\n",base64_decode(jsongetpairvalue(filec,jsonfindpairindex(filec,"content"))));
	// Delete file
	sha=jsongetpairvalue(filec,jsonfindpairindex(filec,"sha"));
	const char* dfout = deletefile(&data,"textfile.txt","Normal commit message.",sha);
	
	// Clean
	freetoken(&data);
	freelink(&data);
	cleanrequest();

	return 0;
}
