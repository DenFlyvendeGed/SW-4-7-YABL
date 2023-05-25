#include"../test.h"
#include<stdio.h>
#include<stdlib.h>
#include"../cfg/cfg.h"
#include"code-generation.h"
#include<stdlib.h>

int cgTypeCastingTest(){
    char buffer[100];
    FILE* w = fmemopen(buffer, 100, "w+");
    Type* type = createType(createTypeValue(bt_text, NULL));
    Expr* expr = createExpr(et_constant, createConstant(td_number, "2"));
    TypeCast* typecastTest = createExpr(et_typecast, createTypeCast(type, expr));
    cgExpr(typecastTest, w);

    printf("%s", w);
    return 1;
}

void cgTests(){
    testHeader("Code genneration");
    doTest("Typecast", cgTypeCastingTest());
}


