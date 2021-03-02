#include "stdlib.h"
#include <sys/stat.h> 
#include <iostream>

const char* userHomeDir = std::getenv("HOME");
const char* nodeDirName = "test-node";

const char* getEnvVar(const char* envVarToGet) {
    char* zshVar = std::getenv(envVarToGet);
    return zshVar;
}

int main () {
    std::cout << "Executing Script..." << std::endl;
    
    char fullNodePath [50];
    sprintf(fullNodePath, "%s/%s", userHomeDir, nodeDirName);
    
    printf("Node Directory Path is %s", fullNodePath);
    int dirResponse = mkdir(fullNodePath, 0777);
}