#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){

    if(pathName == NULL || pathName[0] == '\0'){
	printf("MKDIR ERROR: no path provided\n");
	return;
    }
    // used later
    char forwardslash[2] = "/";

    //make a directory
    struct NODE* newdir = (struct NODE *)malloc(sizeof(struct NODE));

    //extract name and location from pathname
    //save current directory
    struct NODE* basedir = cwd;

    char baseName[100] = {'\0'};
    char dirName[100] = {'\0'};
    struct NODE* current = splitPath(pathName, baseName, dirName);

    if(current == NULL){
	// no path. delete.
        free(newdir);
	return;
    }
    
    strcpy(newdir->name, baseName);
    newdir->parentPtr = current;

    //set rest of values (easy)
    //we don't have any siblings yet
    newdir->siblingPtr = NULL;
    //this is a directory
    newdir->fileType = 'D';
    //no children yet
    newdir->childPtr = NULL;
    //set the sibling ptr of neighboring entry in parent dir
    if(newdir->parentPtr->childPtr == NULL){newdir->parentPtr->childPtr = newdir;}
    else{
        struct NODE* cfile = newdir->parentPtr->childPtr;
        while(cfile != NULL){
	    if(!strcmp(cfile->name, newdir->name)){
		printf("MKDIR ERROR: directory %s already exists\n", pathName);
		free(newdir);
		return;
	    }
	    if(cfile->siblingPtr == NULL){break;}
            cfile = cfile->siblingPtr;
        }
        cfile->siblingPtr = newdir;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){

    char* pathNameCopy = (char*)malloc(sizeof(char)*100);
    strcpy(pathNameCopy, pathName);
    // make sure both base and dirName are empty
    char emptyString[100] = {'\0'};
    strcpy(baseName, emptyString);
    strcpy(dirName, emptyString);

    // used later
    char forwardslash[2] = "/";

    // see if pathName has no / in it
    if(strchr(pathNameCopy, '/') == NULL){
        strcpy(baseName, pathNameCopy);
        // dirName unchanged
        return cwd;
    }

    //set up for traversal
    struct NODE* current;
    if(pathName[0] == '/'){
        dirName[0] = '/';
        dirName[1] = '\0';
        current = root;
    }
    else{
        current = cwd;
    }

    // insure that our path is valid
    char* snippet = strtok(pathNameCopy, "/");
    
    if(snippet == NULL){
	printf("MKDIR ERROR: no path provided\n");
	free(pathNameCopy);
	return NULL;
    }    
    strcat(dirName, snippet);

    current = current->childPtr;
    while(current != NULL && strcmp(current->name, snippet)){
        current = current->siblingPtr;
    }
    if(current == NULL || current->fileType != 'D'){
        printf("ERROR: directory %s does not exist\n", snippet);
	return NULL;
    }

    snippet = strtok(NULL,"/");
    while(snippet != NULL){

	//see if this is the last snippet
	char* next = strtok(NULL, "/");
	if(next == NULL){
	    strcpy(baseName, snippet);
	    break;
	}
	    
	    
	// ensure path is valid
        current = current->childPtr;
        while(current != NULL && strcmp(current->name, snippet)){
            current = current->siblingPtr;
        }
        if(current == NULL || current->fileType != 'D'){
            printf("ERROR: directory %s does not exist\n", snippet);
	    return NULL;
        }
        strcat(dirName, forwardslash);
        strcat(dirName, snippet);

        snippet = next;
    }
    
    free(pathNameCopy);

    return current;
    
    // 

}
