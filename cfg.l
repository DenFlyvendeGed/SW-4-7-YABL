
%{
#include "cfg.tab.h"
#include<stdio.h>
int yywrap();
%}

%%
\/\/.*\n   { /* Comment */ }
\/\*.*\*\/ { /* Comment */ }

"!" {return setpreamble; /*exclamation mark*/}
"board"  {return board;/*board preamble*/}
[0-9]+x[0-9] {return boardsize;/*board declaration*/}
"tile"   {return tile;/*tile preamble*/}
"player" {return player;/*player preamble*/}

"setup" {return setupevent;/*setup event*/}
"turn" {return turnevent;/*turn event*/}
"close" {return closeevent;/*close event*/}

"list"   {return listdcl;/*list type*/}
"text"   {return textdcl;/*text type*/}
"bool"   {return logicdcl;/*bool type*/}
"point"  {/*point type*/}
"number" {return numberdcl; /*number type */ }

"function" {return funckeyword; /* functionkeyword */} 
"return" {return returnkeyword;/*return*/}
"returns" {return returnskeyword;/*returns*/}

"do" {return scopebegin;/* Scope begin */}
"end" {return scopeend;/* Scope End*/}
[\n;] {return endofstatement;/* End of statement*/}

"repeat" {return repeat;/*repeat*/}
"for" {return forkeyword;/*for*/}
"in" {return in;/*in*/}
"while" {return whilekeyword;/*while*/}
"times" {return times; /*times*/}

"if" {return ifkeyword;/*if*/}
"else" {return elsekeyword;/*else*/}

"+" {return addition;/*addition*/}
"-" {return subtraction;/*subtraction*/}
"*" {return multiplication;/*multiplication*/}
"/" {return division;/*division*/}
"remains" {return modulus;/*modulo*/}
"not" {return not;/*not*/}
"!="  {return neq;/*neq*/}
"=="  {return eq;/*eq*/}
">" {return gt;/*gt*/}
"<" {return lt;/*lt*/}
">=" {return gteq;/*gteq*/}
"<=" {return lteq;/*lteq*/}
"=" {return assignoperator;/*assign operator*/}
"and" {return and;/*and*/}
"or" {return or;/*or*/}

"true" {return logic;/*true bool*/}
"false" {return logic;/*false bool*/}
\".*\" {return text;/*text*/}
[0-9]+ {return number;/*number*/  }
[a-zA-Z][0-9a-zA-Z]* {return id; /* id */ }


%%

int yywrap() {return 0;}
void yyerror(char * msg){printf("error %s", msg);}


