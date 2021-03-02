#include "stdlib.h"
#include <sys/stat.h> 
#include <iostream>

const char* userHomeDir = std::getenv("HOME");
const char* nodeDirName = "test-node";

const char* getEnvVar(const char* envVarToGet) {
    char* zshVar = std::getenv(envVarToGet);
    return zshVar;
}

int setupDirStructure (char* nodeDirPath) {

    // store stat() info in a struct
    struct stat info;
    // we can use dot-notation to access information about the folder/file
    // https://linux.die.net/man/2/stat
    // similar to running stat test-node in command-line

    if ( stat(nodeDirPath, &info) == -1 ) {
        // this means stat() was not able to contact the folder
        // since it does not exist, we create it
        printf("Test Node directory does not exist, creating it now...");

        int dirResponse = mkdir(nodeDirPath, 0777);
        return dirResponse;
    }
    else {
        //  this means stat() was able to contact the folder
        // since it already exists, we skip and return.
        printf("Test Node directory already exists, skipping...");
        return 0;
    };
}

int main () {
    std::cout << "Executing Script..." << std::endl;
    
    char fullNodePath [50];
    sprintf(fullNodePath, "%s/%s", userHomeDir, nodeDirName);
    
    printf("Node Directory Path is %s\n", fullNodePath);
    setupDirStructure(fullNodePath);
}