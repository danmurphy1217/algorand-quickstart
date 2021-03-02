#include "stdlib.h"
#include <sys/stat.h> 
#include <iostream>



const char* userHomeDir = std::getenv("HOME");
const char* nodeDirName = "test-node";
const char* algorandDataDirName = "data";
const char* updateScriptFileName = "update.sh";

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
        printf("Test Node directory does not exist, creating it now...\n");

        int dirResponse = mkdir(nodeDirPath, 0777);
        return dirResponse;
    }
    else {
        //  this means stat() was able to contact the folder
        // since it already exists, we skip and return.
        printf("Test Node directory already exists, skipping...\n");
        return 0;
    };
}

char* setEnvVar(const char* varName, char* value) {

    setenv(varName, value, true);

    return value;
}

void downloadFile(const char* urlToDownload, char* downloadLocation){
    char* command;
    sprintf(command, "wget %s -P %s", urlToDownload, downloadLocation);

    system(command);
}

void changeFilePermissions() {
    int result = chmod("/Users/danielmurphy/test-node/update.sh", S_IRWXU);
    printf("PERMISSIONS: %d", result);
}

int main () {
    std::cout << "Executing Script..." << std::endl;
    
    char fullNodePath [50];
    char dataPath [50];

    const char* algorandDataEnvVar = "ALGORAND_DATA";
    const char* updateScript = "https://raw.githubusercontent.com/algorand/go-algorand-doc/master/downloads/installers/update.sh";

    sprintf(fullNodePath, "%s/%s", userHomeDir, nodeDirName);
    sprintf(dataPath, "%s/%s", fullNodePath, algorandDataDirName);
    
    printf("Node Directory Path is %s\n", fullNodePath);
    setupDirStructure(fullNodePath);

    setEnvVar(algorandDataEnvVar, dataPath);
    printf("Algorand Data Directory: %s\n", getEnvVar("ALGORAND_DATA"));

    downloadFile(updateScript, fullNodePath);
    changeFilePermissions();
}