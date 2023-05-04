#ifndef YABL_CONFIGURATION_H
#define YABL_CONFIGURATION_H

#include <stdlib.h>
#include <stdio.h>

typedef enum {
	co_null, co_term, co_gcc, co_file
} ConfigurationOutput;


typedef struct {
	ConfigurationOutput output;
	char* outputFilename;
} Configuration;

Configuration createConfiguration(int argv, char** args);
FILE* openConfiguration(Configuration c);

#endif // !YABL_CONFIGURATION_H

