#include <stdio.h>
#include <stdlib.h>
#include "configuration.h"
#include <string.h>
#include <unistd.h>

#define IF_CASE(str) if(strcmp(args[i], str) == 0)
#define FD_WRITE 1
#define FD_READ 0

Configuration createConfiguration(int argv, char** args){
	Configuration c = {.output = co_term, .outputFilename = NULL};
	for(int i = 0; i < argv; i++){
		IF_CASE("-c"){
			if(++i > argv)
				goto c_error;
			IF_CASE("null")
				c.output = co_null;
			else IF_CASE("gcc")
				c.output = co_gcc;
			else IF_CASE("file")
				c.output = co_file;
			else IF_CASE("term")
				c.output = co_term;
			else{
				c_error:
				perror("[ERROR] after -c \"null\", \"gcc\", \"file\" or \"term\" must come");
				exit(EXIT_FAILURE);
			}
		}
		else IF_CASE("-o"){
			if(++i > argv){
				perror("[ERROR] after -o a filename must come");
				exit(EXIT_FAILURE);
			}

			c.outputFilename = args[i];
		}
	}
	return c;
}

FILE* openConfiguration(Configuration c){
	FILE* f;
	int fd[2];
	switch (c.output) {
		case co_null:	
			f = fopen("/dev/null", "w");
			break;
		case co_file: 
			f = fopen(c.outputFilename == NULL ? "yabl.c" : c.outputFilename, "w");
			break;
		case co_gcc:
			pipe(fd);
			if(!fork()){
				close(fd[FD_WRITE]);
				dup2(fd[FD_READ],STDIN_FILENO);
				char command[100];
				sprintf(command, "gcc -o %s -g -x c -", c.outputFilename == NULL ? "yabl.out" : c.outputFilename);
				system(command);
				close(fd[FD_READ]);
				exit(EXIT_SUCCESS);
			}else{
				close(fd[FD_READ]);
				dup2(fd[FD_WRITE],STDOUT_FILENO);
				f = fdopen(STDOUT_FILENO, "w");
			}
			
		case co_term:
			f = stdout;
			break;
	}
	return f;
}


