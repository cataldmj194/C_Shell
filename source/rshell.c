/** 
   Name : Michael Cataldo
   Class: CIS 310 
   Assignment: Really Anemic Shell
   Due: 10/7/2016
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

/* function prototype declarations */
int execute(char **parsed);
char **cleanUp(char **parsed);
char **parseCmd(char cmd[]);

int main(int argc, char * argv[]){
#ifdef DEBUG_COMMANDLINE
  for(int a = 0; a < argc; ++a)
    printf("argv[%d] = %s\n", a, argv[a]);
#endif

  int retval = -2; //return value
  char cwd[1024];
  char cmd[1024];
  char *command;
  char **parsed;

  //Sets our Path Environment Variable to the directories specified.
  //Allows execvp() to automatically search in those directories
  putenv("PATH = :/bin:/usr/bin:/usr/local/bin/");


  //Our run loop. Could possibly scoop out into it's own method.
  do{
    getcwd(cwd, sizeof(cwd));       //Gets current working directory
    printf("%s $ ",cwd);            //Prints cwd to console
    fgets(cmd,sizeof(cmd),stdin);   //Gets our next line from user, store in cmd
    command = cmd;                  //Sets *command to point to cmd
    strtok(command, "\n");          //Used to break command around newline
    parsed = parseCmd(command);     //calls parseCmd with command
    if(strcmp("cd",parsed[0]) == 0){ //if command entered is cd 
      strtok(parsed[1], "\n");
      if(chdir(parsed[1]) != 0){  //try & change directory to what user provided
        perror("Error changing directories"); //print error if chdir failed
      }
      parsed = cleanUp(parsed);   //Calls cleanUP on parsed (NULLs out contents)
      continue;                   //continue to next iteration of run loop
    }
 
    if(strcmp("exit",parsed[0]) == 0){ //if command entered is exit, we exit
      exit(0);
    }

    retval = execute(parsed);     //calls execute with user provided command
    parsed = cleanUp(parsed);     //Null out contents of parsed

  }while(retval);                 //do while retval != 0
   
  return 0;
}

//Cleanup function, NULLs out contents of a double char pointer provided
//Parameters: char **parsed - a char double pointer
//Returns: a nulled out version of our double pointer
char **cleanUp(char **parsed){

  int i = 0;
  while(parsed[i] != NULL){
    parsed[i] = NULL;
    i++;
  }
  return parsed;
}

//Execute function, forks parent process and calls exec in child process
//Using what was entered from the user. If the user only typed enter, return 1
//Parameters: **parsed - a char double pointer
//Returns: int retval
int execute(char **parsed){

  if(strcmp("\n",parsed[0]) == 0)
    return 1;

  int rc = fork(); 
  int retval = 1; 

  if(rc < 0){
    fprintf(stderr, "fork failed");
    exit(1);
  }else if (rc == 0){                   //We are the child process
    if(execvp(parsed[0],parsed) != 0){  //call execvp w/ user command + args
      perror("Error executing program");
      retval = -1;
      cleanUp(parsed);
      _Exit(-1);
    }
  }else {
    int wc = wait(NULL);    //We are the parent, wait for status change in child
  }

  return retval;
}

//Parses the user command and arguments
//Learned that char** let us kind of use Strings in c
//Parameters: command - a char pointer containing user commands to be parsed
//Returns: parsed - a char double pointer containing parsed user commands
char **parseCmd (char *command){
  char **parsed[1024];
  char *cmd = strtok(command, " ");
  int count = 0;

  while(cmd != NULL){     //trudge through elements of cmd
    //printf("entry %d: %s\n",count,cmd);
    parsed[count] = cmd;  
    cmd = strtok(NULL, " ");
    count++;
  }

  return parsed;
}

