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

/* function declaration */
int execute(char **parsed);
char **cleanUp(char **parsed);
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

  putenv("PATH = :/bin:/usr/bin:/usr/local/bin/");

  do{
    getcwd(cwd, sizeof(cwd));
    printf("%s $ ",cwd);
    fgets(cmd,sizeof(cmd),stdin);
    command = cmd;
    strtok(command, "\n");
    parsed = parseCmd(command);

    if(strcmp("cd",parsed[0]) == 0){
      strtok(parsed[1], "\n");
      if(chdir(parsed[1]) != 0){
        perror("Error changing directories");
      }
      parsed = cleanUp(parsed);
      continue;
    }
 
    if(strcmp("exit",parsed[0]) == 0){
      exit(0);
    }

    retval = execute(parsed);
    parsed = cleanUp(parsed);

  }while(retval);
   
  return 0;
}

char **cleanUp(char **parsed){

  int i = 0;
  while(parsed[i] != NULL){
    parsed[i] = NULL;
    i++;
  }
  return parsed;
}

int execute(char **parsed){
  int rc = fork();
  int retval = 1;
 
  if(rc < 0){
    fprintf(stderr, "fork failed");
    exit(1);
  }else if (rc == 0){
    if(execvp(parsed[0],parsed) != 0){
      perror("Error executing program");
      retval = -1;
      cleanUp(parsed);
      _Exit(-1);
    }
  }else {
    int wc = wait(NULL);
  }

  return retval;
}

char **parseCmd (char *command){
  char **parsed[1024];
  char *cmd = strtok(command, " ");
  int count = 0;

  while(cmd != NULL){
    //printf("entry %d: %s\n",count,cmd);
    parsed[count] = cmd;
    cmd = strtok(NULL, " ");
    count++;
  }

  return parsed;
}

