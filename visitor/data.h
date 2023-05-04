#ifndef YABL_DATA_H

#define YABL_DATA_H

typedef struct {
    // Nonterminals nonterminal;
    BasicTypes type;
    ErrorCode errorCode;
    void* list; //Data*
    void* value; 
} Data;

#endif