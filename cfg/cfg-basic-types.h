#ifndef YABL_BASIC_TYPES_H
#define YABL_BASIC_TYPES_H
typedef enum {bt_NULL, bt_number, bt_text, bt_logic, bt_list, bt_unset} BasicTypes;

typedef struct { 
	struct Typedcl* list;
	BasicTypes type;
} TypeValue;

#endif // !YABL_BASIC_TYPES_H



