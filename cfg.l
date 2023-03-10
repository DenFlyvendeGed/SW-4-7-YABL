
%{
#include "cfg.tab.h"
%}

%%
\/\/.*\n {/*Comment*/ }
\/\*.*\*\/ { /* Comment */ }

"!" {/*exclamation mark*/}
"board"  {/*board preamble*/}
[0-9]+x[0-9] {/*board declaration*/}
"tile"   {/*tile preamble*/}
"player" {/*player preamble*/}


"setup" {/*setup event*/}
"turn" {/*turn event*/}
"close" {/*close event*/}

"list"   {/*list type*/}
"text"   {/*text type*/}
"bool"   {/*bool type*/}
"point"  {/*point type*/}
"number" { /*number type */ }

"function" { /* functionkeyword */} 
"return" {/*return*/}
"returns" {/*returns*/}

"do" {/* Scope begin */}
"end" {/* Scope End*/}
\n   {/* End of statement*/}

"repeat" {/*repeat*/}
"for" {/*for*/}
"in" {/*in*/}
"while" {/*while*/}
"times" {/*times*/}

"if" {/*if*/}
"else" {/*else*/}

"+" {/*addition*/}
"-" {/*subtraction*/}
"*" {/*multiplication*/}
"/" {/*division*/}
"remains" {/*modulo*/}
"not" {/*not*/}
"!="  {/*neq*/}
"=="  {/*eq*/}
">" {/*gt*/}
"<" {/*lt*/}
">=" {/*gteq*/}
"<=" {/*lteq*/}
"=" {/*assign operator*/}
"and" {/*and*/}
"or" {/*or*/}

"true" {/*true bool*/}
"false" {/*false bool*/}
\".*\" {/*text*/}
[0-9]+ { /*number*/  }
[a-zA-Z][0-9a-zA-Z]* { /* id */ }

%%



