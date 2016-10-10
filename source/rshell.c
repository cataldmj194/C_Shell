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

/* function declaration */
char **parseCmd(char cmd[]);

int main(int argc, char * argv[]){
#ifdef DEBUG_COMMANDLINE
  for(int a = 0; a < argc; ++a)
    printf("argv[%d] = %s\n", a, argv[a]);
#endif

  int retval = -2;
  char cwd[1024];
  char cmd[1024];
  char *command;
  char **parsed;

  //Swap this to be just while
  do{
    getcwd(cwd, sizeof(cwd));
    printf("%s $ ",cwd);
    fgets(cmd,sizeof(cmd),stdin);
    command = cmd;
    parsed = parseCmd(command);

    strtok(parsed[0],"\n");
    if(strcmp("cd",parsed[0]) == 0){
      strtok(parsed[1], "\n");
      if(chdir(parsed[1]) != 0){
        perror("Error changing directories");
      }
    }
 
    if(strcmp("exit",parsed[0]) == 0){
      exit(0);
    }

     

    //TODO: fork/exec command and args in another function

    while(*parsed != NULL){
      printf("testing parsed: %s\n", *parsed);
      parsed++;
    }
  }while(retval);
   
  return 0;
}

char **parseCmd (char *command){
  //execv
  //char **parsed = malloc(strlen(command)+1);
  char **parsed[1024];
  char *cmd = strtok(command, " ");
  int count = 0;
  while(cmd != NULL){
    printf("entry %d: %s\n",count,cmd);
    parsed[count] = cmd;
    cmd = strtok(NULL, " ");
    count++;
  }
  return parsed;
}

