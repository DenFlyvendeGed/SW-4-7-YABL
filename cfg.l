%{
#include "../cfg.h"
#include "cfg.tab.h"
#include <stdio.h>
#include <string.h>
#define YYERROR_VERBOSE 1
#define PRINT(X) (printf("[%s] -- \"%s\"\n", X, yytext), YABL_TOKEN = yytext)
char* YABL_TOKEN;
int yywrap();
%}

%option yylineno

%%
!board  { PRINT("board"); return board; }
[1-9][0-9]*x[1-9][0-9]* { PRINT("board-size");yylval.text = copystringalloc(yytext); return boardsize; }
!tile   { PRINT("tile"); return tile; }
!player { PRINT("player"); return player; }
setup { PRINT("setup"); return setupevent; }
turn { PRINT("turn"); return turnevent; }
close { PRINT("close"); return closeevent; }
list   { PRINT("list"); return listdcl; }
text   { PRINT("textdcl"); return textdcl; }
logic   { PRINT("logicdcl"); return logicdcl; }
point  PRINT("point");                  
number { PRINT("numberdcl"); return numberdcl;   }
function { PRINT("function"); return funckeyword; } 
return { PRINT("return"); return returnkeyword; }
returns { PRINT("returns"); return returnskeyword; }
do { PRINT("do"); return scopebegin; }
end { PRINT("end"); return scopeend; }
break { PRINT("break"); return breakkeyword; }
[;] { PRINT("end of statement"); return endofstatement; }
repeat { PRINT("repeat"); return repeat; }
for { PRINT("for"); return forkeyword; }
in { PRINT("in"); return in; }
while { PRINT("while"); return whilekeyword; }
times { PRINT("times"); return times;  }
if { PRINT("if"); return ifkeyword; }
else { PRINT("else"); return elsekeyword; }
\+ { PRINT("plus"); return addition; }
\- { PRINT("minus"); return subtraction; }
\* { PRINT("times"); return multiplication; }
\/ { PRINT("division"); return division; }
remains { PRINT("remains"); return modulus; }
not { PRINT("not"); return not; }
!=  { PRINT("neq"); return neq; }
==  { PRINT("eq"); return eq; }
\> { PRINT("gt"); return gt; }
\< { PRINT("lt"); return lt; }
\>= { PRINT("gteq"); return gteq; }
\<= { PRINT("lteq"); return lteq; }
= { PRINT("assign"); return assignoperator; }
and { PRINT("and"); return and; }
or { PRINT("or"); return or; }
true { PRINT("true"); yylval.text= 1; return logic; }
false { PRINT("false"); yylval.text= 0; return logic; }
\"([^\"]|\\\")*\" { PRINT("string"); yylval.text = copystringalloc(yytext); return text; }
[0-9]+ { PRINT("number"); yylval.text = copystringalloc(yytext); return number;   }
[a-zA-Z][0-9a-zA-Z]* { PRINT("id"); yylval.text = copystringalloc(yytext); return id; }
\( {PRINT("L NORMAL Paren"); return lparen; }
\) {PRINT("R NORMAL Paren"); return rparen; }
\[ {PRINT("L SQUARE Paren"); return lsparen; }
\] {PRINT("R SQUARE Paren"); return rsparen; }
\{ {PRINT("L CURLY Paren"); return lcparen; }
\} {PRINT("R CURLY Paren"); return rcparen; }
\. {PRINT("dot"); return dot;}

\/\/.*\n     PRINT("end of line comment");
\/\*.*\*\/   PRINT("multi line comment");

<<EOF>> { PRINT("EOF"); return YYEOF; }

[ \t\r] ;
\n  ;
.   { PRINT("ERROR"); return '\0';}
%%

int yywrap() {return 1;}
void yyerror(char * msg){
	fprintf(stderr, "[error line %d, unexpected token \"%s\"] %s\n", yylineno, YABL_TOKEN,  msg);
}

