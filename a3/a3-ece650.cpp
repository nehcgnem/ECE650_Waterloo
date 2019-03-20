#include <iostream>
#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <csignal>


using namespace std;


int main (int argc, char **argv) {

    pid_t pid[4];
    string line;
    int pipe1[2]; // pipe between regen and a1-ece650
    pipe (pipe1);
 
    int pipe2[2]; // pipe 2 
    pipe (pipe2);



/*regen to A1*/
    // child one 
    if ((pid[0] = fork())<0){
    	perror("Error: fork");
    	abort(); 
    }
    else if (pid[0] == 0){

    	/* connect Regen to A1*/
    	// dup2(pipe1[0], STDIN_FILENO);    	
    	dup2(pipe1[1], STDOUT_FILENO);
    	close(pipe1[0]);
    	close(pipe1[1]);
    	// return execl("./rgen", "rgen", (char *) NULL); 
        return execv("./rgen", argv);

    }

/*A1 to A2*/ 
// child two    
    if ((pid[1] = fork())<0){
    	perror("Error: fork");
    	abort(); 
    }
    else if (pid[1] == 0){
    	/* connect A1 to A2*/
    	dup2(pipe1[0], STDIN_FILENO);    	
    	dup2(pipe2[1], STDOUT_FILENO);
    	close(pipe1[0]);
    	close(pipe1[1]);
    	close(pipe2[0]);
    	close(pipe2[1]);
        char *pythonArgs[] = {(char *) "python", (char *) "./a1-ece650.py", nullptr};
        return execvp("python", pythonArgs);    
    }
 // child three    
    if ((pid[2] = fork())<0){
    	perror("Error: fork");
    	abort(); 
    }
    else if (pid[2] == 0){
    	/* connect A1 to A2*/
    	dup2(pipe2[0], STDIN_FILENO);    	
    	close(pipe2[0]);
    	close(pipe2[1]);
    	return execl("./a2-ece650", "a2-ece650", (char *) NULL); 
    }

// child four or parent 
    // if ((pid[3] = fork())<0){
    // 	perror("Error: fork");
    // 	abort(); 
    // }
     else if (pid[3] == 0){
    	/* connect A1 to A2*/
    	dup2(pipe2[1], STDOUT_FILENO);  
    	close(pipe1[0]);
    	close(pipe1[1]);  	
    	close(pipe2[0]);
    	close(pipe2[1]);
    	/* print commend input */
    	while (getline(cin, line))
    	{
    		cout << line << endl;
    	}
     }

    // send kill signal to all children
      for (auto k : pid) {
        int status;
        kill(k, SIGTERM);
        waitpid(k, &status, 0);
    }

    return 0;
    }

