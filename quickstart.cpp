#include "stdlib.h"
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

const char *userHomeDir = std::getenv("HOME");
const char *nodeDirName = "node";
const char *algorandDataDirName = "data";
const char *updateScriptFileName = "update.sh";

const char *getEnvVar(const char *envVarToGet)
{
    char *zshVar = std::getenv(envVarToGet);
    return zshVar;
}

int setupDirStructure(char *nodeDirPath)
{
    /**
    * check if the path exists. If it does, return 0. If it does not,
    * create the directory, set it to 777 so that everyone has RWX
    * permissions, and return from the function.
    * 
    * @param nodeDirPath -> the path to create (if it does not already exist).
    * @return 0 (success).
    */

    // store stat() info in a struct
    struct stat info;
    // we can use dot-notation to access information about the folder/file
    // https://linux.die.net/man/2/stat
    // similar to running stat node in command-line

    if (stat(nodeDirPath, &info) == -1)
    {
        // this means stat() was not able to contact the folder
        // since it does not exist, we create it
        printf("Node directory does not exist, creating it now...\n");

        int dirResponse = mkdir(nodeDirPath, 0777);
        return dirResponse;
    }
    else
    {
        //  this means stat() was able to contact the folder
        // since it already exists, we skip and return.
        printf("Node directory already exists, skipping...\n");
        return 0;
    };
}

char *setEnvVar(const char *varName, char *value)
{
    /**
    * set environment variable where `varName`=`value`.
    * 
    * @param varName -> the name of the environment variable.
    * @param value -> the value to assign to `varName`.
    * 
    * @return -> `value`, the value assigned to `varName`.
    */
    printf("VAR NAME: %s\n", varName);
    printf("VALUE: %s\n", value);
    setenv(varName, value, true);

    return value;
}

int downloadFile(char *urlToDownload, char *downloadLocation)
{
    /**
    * download a file from `urlToDownload` into `downloadLocation`.
    * 
    * @param urlToDownload -> must be a valid URL to download from.
    * @param downloadLocation -> must be a folder that exists.
    * 
    * @return none.
    */

    char command_one[] = "wget";
    char command_two[] = "-P";

    char *argument_list[] = {command_one, urlToDownload, command_two, downloadLocation, NULL};
    int response = execvp(argument_list[0], argument_list);
    return response;
    return 0;
}

void changeFilePermissions(const char *filePath)
{
    /**
    * change the permissions of a file to everyone can RWX.
    * 
    * @param none.
    * @return none.
    */
    printf("FILE: %s\n", filePath);
    int result = chmod(filePath, S_IRWXU);
    printf("PERMISSIONS: %d\n", result);
}

int runUpdateScript(char *nodeDir)
{
    /**
    * run the update.sh script
    * 
    * @param nodeDir -> the directory the Algorand node files are stored in.
    * @return 0
    */
    char command[250];
    sprintf(command, "cd %1$s && yes | ./update.sh -i -c stable -p %1$s -d %2$s -n", nodeDir, algorandDataDirName);
    // TODO: replace with exec()
    system(command);
    return 0;
}

int startNode(char *nodeDir)
{
    /**
    * start the Algorand node by running `./goal node start -d data`.
    * 
    * @param nodeDir -> the directory the Algorand node files are stored in.
    * @return 0
    */
    char command[250];
    sprintf(command, "cd %1$s && ./goal node start -d %2$s", nodeDir, algorandDataDirName);
    // TODO: replace with exec()
    system(command);
    return 0;
};

int checkNodeStatus(char *nodeDir)
{
    /**
    * check the status of the Algorand node with `./goal node status -d data`.
    *
    * @param nodeDir -> the directory the Algorand node files are stored in.
    * @return 0
    */
    char command[250];
    sprintf(command, "cd %1$s && ./goal node status -d %2$s", nodeDir, algorandDataDirName);
    // TODO: replace with exec()
    system(command);
    return 0;
};

int main()
{
    std::cout << "Executing Script..." << std::endl;

    char fullNodePath[50];
    char dataPath[50];
    char updateScriptPath[50];

    const char *algorandDataEnvVar = "ALGORAND_DATA";
    char updateScriptUrl[] = "https://raw.githubusercontent.com/algorand/go-algorand-doc/master/downloads/installers/update.sh";

    sprintf(fullNodePath, "%s/%s", userHomeDir, nodeDirName);
    printf("Node Directory Path is %s\n", fullNodePath);
    setupDirStructure(fullNodePath);

    sprintf(dataPath, "%s/%s", fullNodePath, algorandDataDirName);
    setEnvVar(algorandDataEnvVar, dataPath);
    printf("Algorand Data Directory: %s\n", getEnvVar("ALGORAND_DATA"));

    pid_t pid;
    pid = fork();

    if (pid < 0)
    {
        printf("Error ocurred");
        exit(-1);
    }
    else
    {
        printf("PID: %d\n", pid);
        if (pid == 0) // child process
        {
            downloadFile(updateScriptUrl, fullNodePath);
        }
        else
        {
            wait(0);
            printf("In parent process, continuing...\n");
            sprintf(updateScriptPath, "%s/%s", fullNodePath, updateScriptFileName);
            changeFilePermissions(updateScriptPath);
            runUpdateScript(fullNodePath);
            startNode(fullNodePath);
            checkNodeStatus(fullNodePath);
        }
    }
}