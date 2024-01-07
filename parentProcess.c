#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "time.h"
#include "signal.h"

//function definitions
void appendToLogFile(time_t now, int i, const char *command);
void appendToLikesLogFile(int i, const char *command);
void LikesServer(int serverNumber);


int main() {
    /*
    printf("ParentProcess is alive\n"); ///////////////////////
    //seed random number generator
    srand(time(NULL));
    //create variables
    int i;
    int size = 10;

    pid_t child_pid = fork();
    //Record timestamp when the child is started
    time_t now;
    time(&now);

    //if child_pid < 0 fork failed
    if (child_pid < 0) {
        printf("Fork Failed\n");
        exit(EXIT_FAILURE);
    }
    //checks if child_pid is 0
    if (child_pid == 0) {
        for (i = 0; i < size; i++){
            //calls the likeServer and appends to the log file
            appendToLogFile(now, i, "Started");
            LikesServer(i);
            sleep(1);
            appendToLogFile(now, i, "Ended");
        }
        exit(0);
    }

    // Code executed by the parent process
    int j;
    for (j = 0; j < size; j++) {
        int status;
        wait(&status); // Wait for each child to finish

    }
     */
    printf("ParentProcess is alive\n");

    int i;
    int size = 10;
    pid_t child_pids[size];

    // Record timestamp when the parent is started
    time_t now;
    time(&now);

    for (i = 0; i < size; i++) {
        pid_t child_pid = fork();
        // seed random number generator
        srand(time(NULL) * getpid());
        if (child_pid < 0) {
            printf("Fork Failed\n");
            exit(EXIT_FAILURE);
        }

        if (child_pid == 0) {
            // Child process
            appendToLogFile(now, i, "Started");
            LikesServer(i);
            appendToLogFile(now, i, "Ended");
            exit(0);
        } else {
            // Parent process
            child_pids[i] = child_pid;
        }
    }

    // Wait for all child processes to finish
    int j;
    for (j = 0; j < size; j++) {
        int status;
        waitpid(child_pids[j], &status, 0); // Wait for each child to finish
    }
    return 0;
}


void LikesServer(int serverNumber) {
    //generate random number of likes
    int number_of_likes = rand() % 43;
    //generate random number for intervals
    //int interval = (rand() % 5) + 1;

    printf("LikesServer%d %d\n", serverNumber, number_of_likes); ////////////////////
    //create socket
    int likesServer_socket;
    //first parameter is the domain, second parameter is the type of socket, third parameter is protocol
    likesServer_socket = socket(AF_INET, SOCK_STREAM, 0);
    //specify address structure for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;    //family is AF_INET
    server_address.sin_port = htons(9002);  //port address
    server_address.sin_addr.s_addr = INADDR_ANY;

    int connect_status = connect(likesServer_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    //check if connection was successful or not
    if (connect_status == -1){
        printf("There was an error connecting to the PrimaryLikesServer\n");
        //append to parent process log if child process fails
        time_t now;
        time(&now);
        appendToLogFile(now, serverNumber, "Child Process failed with exit 1");
        exit(10);
    }

    //send data to PrimaryLikesServer
    // Format the string and send message
    char client_message[256];
    int new_size = snprintf(client_message, sizeof(client_message), "LikesServer%d %d\n", serverNumber, number_of_likes);
    send(likesServer_socket, client_message, new_size, 0);

    //create string to receive data
    char buffer[1024];
    //receive data from server
    recv(likesServer_socket, &buffer, sizeof (buffer), 0);
    //print out servers information
    printf("The PrimaryLikesServer sent the message: %s\n", buffer); //////////////////
    //add message to log file
    appendToLikesLogFile(serverNumber, buffer);
    sleep(5);

    //close the socket
    close(likesServer_socket);
}

void appendToLogFile(time_t now, int i, const char *command) {
    //open the file and write to it
    FILE *ParentLogFile = fopen("/tmp/ParentProcessStatus", "a");
    if (ParentLogFile == NULL) {
        perror("Error opening Log file");
        return;
    }
    //append to the log file
    fprintf(ParentLogFile, "%s: LikesServer%d %s\n", ctime(&now), i, command);
    fclose(ParentLogFile);
}

void appendToLikesLogFile(int i, const char *command) {
    //format the string for the filename
    char fileName[100];
    snprintf(fileName, sizeof(fileName), "/tmp/LikesServer%d", i);

    //open the file and write to it
    FILE *ChildLogFile = fopen(fileName, "a");
    if (ChildLogFile == NULL) {
        perror("Error opening Log file");
        return;
    }
    //append to the log file
    fprintf(ChildLogFile, "LikesServer%d %s\n", i, command);
    fclose(ChildLogFile);
}
