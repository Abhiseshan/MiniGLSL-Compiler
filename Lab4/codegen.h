#ifndef CODEGEN_H
#define CODEGEN_H 1

#include <stdio.h>
#include "ast.h"
#include "string.h"
#include "stack.h"

#include "semantic.h"

int genCode(node * ast);
int initAll(node * ast);

#endif
