#ifndef _SEMANTIC_H
#define _SEMANTIC_H

#include "ast.h"
#include "symbol.h"
#include "common.h"

#define ERROR -1

int semantic_check( node *ast);

#endif