#ifndef YABL_DATA_H
#define YABL_DATA_H

#include "../cfg/cfg-basic-types.h"

typedef enum  {ECnoError, ECempty, ECargumentExeption, ECtypeExeption, ECmissingChild, ECoutOfRange, ECoutOfNamespace, ECnameSpaceClash} ErrorCode;
typedef struct {
    // Nonterminals nonterminal;
    BasicTypes type;
    ErrorCode errorCode;
    void* list; //Data*
    void* value; 
} Data;

#endif
