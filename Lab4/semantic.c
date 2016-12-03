#include "semantic.h"
#include <cstddef>
#include <stdio.h>
#include <string.h>
/*
int semantic_check( node *ast) {
  return 0; // failed checks
}*/

int isInIf=0;

int checkPredefined(char* name) {
	if (!strcmp(name, "gl_FragColor") || !strcmp(name, "gl_FragDepth") || !strcmp(name, "gl_FragCoord") || !strcmp(name, "gl_TexCoord") || !strcmp(name, " gl_Color")  || 
		!strcmp(name, "gl_Secondary") || !strcmp(name, "gl_FogFragCoord") || !strcmp(name, "gl_Light_Half") || !strcmp(name, " gl_Light_Ambient") || 
		!strcmp(name, "gl_Material_Shininess") || !strcmp(name, "env1") || !strcmp(name, "env2") || !strcmp(name, "env3"))
                
		return ERROR;

	return 1;
}

bool isAttribute(char* name){
	if (!strcmp(name, "gl_TexCoord") || !strcmp(name, "gl_Color") || !strcmp(name, "gl_Secondary") || !strcmp(name, "gl_FogFragCoord"))
		return true;
	return false;
}

bool isUniform(char* name){
	if (!strcmp(name, "gl_Light_Half") || !strcmp(name, "gl_Light_Ambient") || !strcmp(name, "gl_Material_Shininess") || !strcmp(name, "env1") 
		|| !strcmp(name, "env2") || !strcmp(name, "env3"))
		return true;
	return false;
}

bool isResult(char* name){
	if (!strcmp(name, "gl_FragColor") || !strcmp(name, "gl_FragDepth") || !strcmp(name, "gl_FragCoord"))
		return true;
	return false;
}

int getBaseForTypeCode(int type_code) {
	if (type_code < 4)
		return 0;
	else if (type_code < 8)
		return 4;
	else if (type_code < 12)
		return 8;
	else
		return ERROR;
}

int checkDepth( node *ast) {

	if(ast==NULL)
		return 0;

	int kind;
	kind = ast->kind;

	switch(kind){
            case NESTED_EXPRESSION_NODE:
                    return 1;
                    break;
            case UNARY_EXPRESION_NODE:
                    return 1;
                    break;
            case BINARY_EXPRESSION_NODE:
                    return 1;
                    break;
            case INT_NODE:
                    return 1;
                    break;
            case BOOL_NODE:
                    return 1;
                    break;
            case FLOAT_NODE:
                    return 1;
                    break;
            case VARIABLE_NODE:
                    return 1;
                    break;
            case ARRAY_INDEX_NODE:
                    return 1;
                    break;
            case FUNCTION_NODE:
                    return 1;
                    break;
            case CONSTRUCTOR_NODE:
                    return 1;
                    break;
            case EXPRESSION_VARIABLE_NODE:
                    return 1;
                    break;
            case ARGUMENTS_NODE:
                    if(ast->arguments.args == NULL) return 0;
                    return 1 + checkDepth(ast->arguments.args);
                    break;
            default :
                    errorOccurred = 1; fprintf(errorFile,"check depth failed: %d\n", kind);
                    return ERROR;
                    break;
	}

	return 0;
}

int semantic_check(node *ast) {
        
	if(ast==NULL) {
        fprintf(errorFile,"Semantic function visited a NULL node\n");
        return ERROR;
    }

	int kind;
	int type;
	int depth;
	int exp1, exp2;
	char * name;
	int index;
	kind = ast->kind;
    int isDecl=0;
    symbol_table_entry *entry;
    node *var;

	switch(kind){
		case ENTER_SCOPE_NODE:
			return exp1 = semantic_check(ast->enter_scope);			
			break;

		case SCOPE_NODE:
            scope_enter();

			if (ast->scope.declarations)
                exp1 = semantic_check(ast->scope.declarations);
            else 
                exp1 = 1;
            if(ast->scope.statements) 
                exp2 = semantic_check(ast->scope.statements);
            else
                exp2 = 1;

			if(exp1==ERROR || exp2 == ERROR)
                return ERROR;

            scope_exit();
			return 0;
			break;

		case DECLARATIONS_NODE:
            if(ast->declarations.declarations)
                exp1 = semantic_check(ast->declarations.declarations);
            else 
                exp1 = 1;

            exp2 = semantic_check(ast->declarations.declaration);

			if(exp1==ERROR || exp2 == ERROR)
                return ERROR;

			return exp2;
			break;

		case STATEMENTS_NODE:
			if (ast->statements.statements)
                exp1 = semantic_check(ast->statements.statements);
            else
                exp1 = 1;
            if(ast->statements.statement)
                exp2 = semantic_check(ast->statements.statement);
            else
                exp2 = 1;
                    
			if(exp1==ERROR || exp2 == ERROR)
				return ERROR;

			return exp2;
			break;
                        
		case NESTED_EXPRESSION_NODE:
			return semantic_check(ast->nested_expression);
			break;

		case UNARY_EXPRESION_NODE:
			exp1 = semantic_check(ast->unary.right);

			if(exp1==ERROR)
				return ERROR;

			switch (ast->unary.op){
				case MINUS_OP:
					if(exp1 == BOOL || exp1==BVEC2|| exp1==BVEC3|| exp1==BVEC4){                    
						errorOccurred = 1; 
						fprintf(errorFile,"Line: %d: error: found BOOL, expecting INT or FLOAT\n",ast->line_num);						
						return ERROR;
					} else{
						return exp1;
					}
					break;

				case NOT_OP:
					if(exp1!=BOOL || exp1!=BVEC2 || exp1!=BVEC3 || exp1!=BVEC4){
						if(exp1 == INT || exp1==IVEC2 || exp1==IVEC3|| exp1==IVEC4){
							errorOccurred = 1; 
							fprintf(errorFile,"Line: %d: error: found INTEGER, expecting BOOL\n",ast->line_num);						
						}
						else if(exp1 == FLOAT || exp1==VEC2 || exp1==VEC3|| exp1==VEC4){
							errorOccurred = 1; 
							fprintf(errorFile,"Line: %d: error: found FLOAT, expecting BOOL\n",ast->line_num);						
						}
						else {
							errorOccurred = 1; 
							fprintf(errorFile,"Line: %d: error: found UNKNOWN TYPE, expecting BOOL\n",ast->line_num);						
						}
						return ERROR;

					} else{
						return exp1;
					}
					break;
			}
			break;
                        
		case BINARY_EXPRESSION_NODE:
			exp2 = semantic_check(ast->binary.left);
			exp1 = semantic_check(ast->binary.right);

			if(exp1 == ERROR || exp2 == ERROR)
				return ERROR;

			//Logical operators
			if(ast->binary.op==AND_OP || ast->binary.op==OR_OP){
				if(exp2==BOOL && exp1==BOOL){
					return BOOL;
				}else if(exp2==BVEC2 && exp1==BVEC2){
					return BVEC2;
				}else if(exp2==BVEC3 && exp1==BVEC3){
					return BVEC3;
				}else if(exp2==BVEC4 && exp1==BVEC4){
					return BVEC4;
				}else if(exp2 == INT || exp2 == IVEC2 || exp2 == IVEC3 || exp2 == IVEC4){
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, found INT, expecting BOOL\n",ast->line_num);						
					return ERROR;
				}else if(exp1 == INT || exp1 == IVEC2 || exp1 == IVEC3 || exp1 == IVEC4){
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, found INT, expecting BOOL\n",ast->line_num);						
					return ERROR;
				}else if(exp2 == FLOAT || exp2 == VEC2 || exp2 == VEC3 || exp2 == VEC4){
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, found FLOAT, expecting BOOL\n",ast->line_num);						
					return ERROR;
				}else if(exp1 == FLOAT || exp1 == VEC2 || exp1 == VEC3 || exp1 == VEC4){
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, found FLOAT, expecting BOOL\n",ast->line_num);						
					return ERROR;
				}else if(exp2 != exp1){
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, operation can only be performed on vectors of same size\n",ast->line_num);						
					return ERROR;
				}
			}

			//comparison operators
			if(ast->binary.op==LT_OP ||ast->binary.op==LEQ_OP || ast->binary.op==GT_OP || ast->binary.op==GEQ_OP){
				if(exp2==INT && exp1==INT){
					return INT;
				}else if(exp2==FLOAT && exp1==FLOAT){
					return FLOAT;
				}else if(exp2==BOOL || exp1==BOOL){
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, found BOOL, expecting INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp2== IVEC2 || exp2==IVEC3 || exp2==IVEC4){	
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, found INT VECTOR, expecting SCALAR INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp2== VEC2 || exp2==VEC3 || exp2==VEC4){
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, found FLOAT VECTOR, expecting SCALAR INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp2== BVEC2 || exp2==BVEC3 || exp2==BVEC4){
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, found BOOL VECTOR, expecting SCALAR INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp1== IVEC2 || exp1==IVEC3 || exp1==IVEC4){
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, found INT VECTOR, expecting SCALAR INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp1== VEC2 || exp1==VEC3 || exp1==VEC4){
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, found FLOAT VECTOR, expecting SCALAR INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp1== BVEC2 || exp1==BVEC3 || exp1==BVEC4){
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, found BOOL VECTOR, expecting SCALAR INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp2!=exp1){
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, expecting operands of same type on both sides\n",ast->line_num);						
					return ERROR;
				}
			}

			if(ast->binary.op==EQ_OP || ast->binary.op==NEQ_OP){
				if(exp2==exp1){
					if(exp2==BOOL || exp2 == BVEC2 || exp2 == BVEC3 || exp2 == BVEC4){
						errorOccurred = 1; 
						fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, found BOOL, expecting INT or FLOAT\n",ast->line_num);						
					}
				}else{
						errorOccurred = 1; 
						fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, expecting operands of same type on both sides\n",ast->line_num);						
					return ERROR;
				}
			}

			//arithmetic operators add and sub
			if(ast->binary.op==ADD_OP || ast->binary.op==SUB_OP){
				if(exp2==INT && exp1==INT){
					return INT;
				}else if(exp2==FLOAT && exp1==FLOAT){
					return FLOAT;
				}else if(exp2==IVEC2 && exp1==IVEC2){
					return IVEC2;
				}else if(exp2==IVEC3 && exp1==IVEC3){
					return IVEC3;
				}else if(exp2==IVEC4 && exp1==IVEC4){
					return IVEC4;
				}else if(exp2==VEC2 && exp1==VEC2){
					return VEC2;
				}else if(exp2==VEC3 && exp1==VEC3){
					return VEC3;
				}else if(exp2==VEC4 && exp1==VEC4){
					return VEC4;
				}else if(exp2==BOOL || exp2 == BVEC2 || exp2 == BVEC3 || exp2 == BVEC4){
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, found BOOL, expecting INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp1==BOOL || exp1 == BVEC2 || exp1 == BVEC3 || exp1 == BVEC4){
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, found BOOL, expecting INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp2!=exp1){
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, expecting operands of same type on both sides\n",ast->line_num);						
					return ERROR;
				}
			}

			//arithmetic operator mult
			if(ast->binary.op==MULT_OP){
				if(exp2 == INT && (exp1==INT || exp1==IVEC2 || exp1==IVEC3 || exp1==IVEC4)){
					return exp1;
				}else if(exp1 == INT && (exp2==INT || exp2==IVEC2 || exp2==IVEC3 || exp2==IVEC4)){
					return exp2;
				}else if(exp2 == FLOAT && (exp1==FLOAT || exp1==VEC2 || exp1==VEC3 || exp1==VEC4)){
					return exp1;
				}else if(exp1 == FLOAT && (exp2==FLOAT || exp2==VEC2 || exp2==VEC3 || exp2==VEC4)){
					return exp2;
				}else if(exp2 == BOOL || exp2 == BVEC2 || exp2 == BVEC3 || exp2 == BVEC4){
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, found BOOL, expecting INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp1==BOOL || exp1 == BVEC2 || exp1 == BVEC3 || exp1 == BVEC4){
					errorOccurred = 1;
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, found BOOL, expecting INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp2==exp1){
					return exp2;
				}else if(exp2!=exp1){
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, expecting operands of same type on both sides\n",ast->line_num);						
					return ERROR;
				}
			}

			//arithmetic operator /,^
			if(ast->binary.op==DIV_OP || ast->binary.op==POW_OP){
				if(exp2==INT && exp1==INT){
					return INT;
				}else if(exp2==FLOAT && exp1==FLOAT){
					return FLOAT;
				}else if(exp2 == BOOL || exp2 == BVEC2 || exp2 == BVEC3 || exp2 == BVEC4){
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, found BOOL, expecting INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp1==BOOL || exp1 == BVEC2 || exp1 == BVEC3 || exp1 == BVEC4){
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, found BOOL, expecting INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp2!=exp1){
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, expecting operands of same type on both sides\n",ast->line_num);						
					return ERROR;
				}else{
					errorOccurred = 1; 
					fprintf(errorFile,"Line: %d: error: TYPE MISMATCH, expecting operands of SCALAR type\n",ast->line_num);						
					return ERROR;
				}
			}
			break;
                        
		case INT_NODE:
			return INT;
			break;
		case FLOAT_NODE:
			return FLOAT;
			break;
		case BOOL_NODE:
			return BOOL;
			break;
                        
		case VARIABLE_NODE:
            entry = symbol_find(ast->variable.id);
            
            if (!entry) {
				errorOccurred = 1; 
				fprintf(errorFile,"Line: %d: error: UNDEFINED VARIABLE, %s not defined in scope before it is used.\n",ast->line_num, ast->variable.id);						
                return ERROR;
            } else if (!entry->is_init){
                errorOccurred = 1; 
                fprintf(errorFile,"Line: %d: error: UNINITIALISED VARIABLE, %s not initialised in scope before it is used.\n",ast->line_num, ast->variable.id);
                return ERROR;
            } else {
                return entry->type_code;
            }
			break;

		case ARRAY_INDEX_NODE:
            entry = symbol_find(ast->array_index.id);
            if (!entry) {
				errorOccurred = 1; 
				fprintf(errorFile,"Line: %d: error: UNDEFINED VARIABLE, %s not defined in scope before it is used.\n",ast->line_num, ast->variable.id);						
                return ERROR;

            } else if (!entry->is_init){
                errorOccurred = 1; fprintf(errorFile,"Line: %d: error: UNINITIALISED VARIABLE, %s not initialised in scope before it is used.\n",ast->line_num, ast->variable.id);
                return ERROR;
            }
            
            type = entry->type_code;
            ast->array_index.type = (type_code) type;

			index = ast->array_index.index;
			switch(type){
                case BVEC2:
                case VEC2:
                case IVEC2:
                    if(index>=2 || index < 0){
                        errorOccurred = 1; 
                        fprintf(errorFile,"Line: %d: error: Index out of bounds\n",ast->line_num);
                        return ERROR;
                    }
                    break;
                case BVEC3:
                case VEC3:
                case IVEC3:
                    if(index>=3 || index < 0){
                        errorOccurred = 1; 
                        fprintf(errorFile,"Line: %d: error: Index out of bounds\n",ast->line_num);
                        return ERROR;
                    }
                    break;
                case BVEC4:
                case VEC4:
                case IVEC4:
                    if(index>=4 || index < 0){
	                    errorOccurred = 1; 
	                    fprintf(errorFile,"Line: %d: error: Index out of bounds\n",ast->line_num);
	                    return ERROR;
                    }
                    break;
                default:
                    errorOccurred = 1; 
                    fprintf(errorFile,"Line: %d: error: Cannot read index of scalar data types\n",ast->line_num);
                    return ERROR;
			}

			break;

		case FUNCTION_NODE:
			type = semantic_check(ast->function.args);
			if(type==ERROR)
				return ERROR;

			if(ast->function.func_code == 2){ //RSQ
                if(type==FLOAT){
                        return FLOAT;
                }
                if(type==INT){
                        return FLOAT;
                }
                else {
                    errorOccurred = 1; 
                    fprintf(errorFile,"Line: %d error: expecting int or float to function rsq, got %s", ast->line_num, getTypeString(type));
                    return ERROR;
                }
			} else if(ast->function.func_code == 0) { //DP3
				if(type==VEC4 || type == VEC3){
					return FLOAT;
				}
				if(type==IVEC4 || type == IVEC3){
					return INT;
				} else {
	                errorOccurred = 1; 
	                fprintf(errorFile,"Line: %d error: expecting vec4, vec3, ivec4, or ivec3 to function dp3, got %s",ast->line_num, getTypeString(type));
	                return ERROR;
	            }
			} else if (ast->function.func_code == 1 ){  //LIT
                if(type != VEC4)  {
                    errorOccurred = 1; 
                    fprintf(errorFile,"Line: %d error: expecting vec4 to function lit, got %s",ast->line_num, getTypeString(type));
                    return ERROR;
                }
                return FLOAT;
			}

			errorOccurred = 1; 
			fprintf(errorFile,"Line: %d: error: UNRECOGNIZED function %s\n",ast->line_num, getFuncString(ast->function.func_code));
			return ERROR;

			break;

		case CONSTRUCTOR_NODE:
			exp2 = semantic_check(ast->constructor.type);
			if (ast->constructor.args)
                exp1 = semantic_check(ast->constructor.args);
            else
                exp1 = 1;

			if(exp1==ERROR || exp2 == ERROR)
				return ERROR;

			depth = checkDepth(ast->constructor.args);

			switch(exp2){
                case FLOAT:
                case BOOL:
                case INT:
                    if(depth>1){
						errorOccurred = 1; 
						fprintf(errorFile,"Line: %d: error: TOO MANY ARGUMENTS, expecting 1, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					else if (depth<1){
						errorOccurred = 1; 
						fprintf(errorFile,"Line: %d: error: TOO FEW ARGUMENTS, expecting 1, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					break;
                case BVEC2:
                case VEC2:
				case IVEC2:
					if(depth>2){
						errorOccurred = 1; 
						fprintf(errorFile,"Line: %d: error: TOO MANY ARGUMENTS, expecting 2, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					else if (depth<2){
						errorOccurred = 1; 
						fprintf(errorFile,"Line: %d: error: TOO FEW ARGUMENTS, expecting 2, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					break;
                case BVEC3:
                case VEC3:
				case IVEC3:
					if(depth>3){
						errorOccurred = 1; 
						fprintf(errorFile,"Line: %d: error: TOO MANY ARGUMENTS, expecting 3, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					else if (depth<3){
						errorOccurred = 1; 
						fprintf(errorFile,"Line: %d: error: TOO FEW ARGUMENTS, expecting 3, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					break;
                                case VEC4:
                                case BVEC4:
				case IVEC4:
					if(depth>4){
						errorOccurred = 1; 
						fprintf(errorFile,"Line: %d: error: TOO MANY ARGUMENTS, expecting 4, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					else if (depth<4){
						errorOccurred = 1; 
						fprintf(errorFile,"Line: %d: error: TOO FEW ARGUMENTS, expecting 4, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					break;
				default:
					if(depth>1){
						errorOccurred = 1; 
						fprintf(errorFile,"Line: %d: error: UNKNOWN CONSTRUCTOR\n",ast->line_num);
					}
			}

			if(exp2 == INT || exp2==IVEC2 || exp2==IVEC3 || exp2==IVEC4){
				if(exp1==INT){
					return exp2;
				}
			} else if(exp2 == BOOL || exp2==BVEC2 || exp2==BVEC3 || exp2==BVEC4){
				if(exp1==BOOL){
					return exp2;
				}
			} else if(exp2 == FLOAT || exp2==VEC2 || exp2==VEC3 || exp2==VEC4){
				if(exp1==FLOAT){
					//fprintf(errorFile,"CONS EXP1: %d: \nEXP2: %d:\n",exp1, exp2);								
					return exp2;
				}
			} else {
				errorOccurred = 1; 
				fprintf(errorFile,"Line: %d: error: TYPE MISMATCH\n",ast->line_num);
				return ERROR;
            }
			break;
                            
		case TYPE_NODE:
			return ast->type_node.type;
			break;
                        
		case IF_STATEMENT_NODE:
			isInIf++;
			exp2 = semantic_check(ast->if_statement.cond);

			if(exp2 == ERROR)
				return ERROR;

			if(exp2!=BOOL){
				errorOccurred = 1; 
				fprintf(errorFile,"Line: %d: error: expecting boolean expression.\n",ast->line_num);
				
				return ERROR;
			}
			if(ast->if_statement.else_statement != NULL) 
                            semantic_check(ast->if_statement.else_statement);
			if (ast->if_statement.then_statement)
                            semantic_check(ast->if_statement.then_statement);
			isInIf--;
			break;
                        
		case ASSIGNMENT_NODE:
			var = ast->assignment.var;
            if(var->kind == VARIABLE_NODE)
                name = var->variable.id;
            else
                name = var->array_index.id;

            entry = symbol_find(name);
            if(isAttribute(name) || isUniform(name)){
				errorOccurred = 1; 
				fprintf(errorFile,"Line: %d: error: INVALID ASSIGNMENT, trying to write to a read-only variable\n",ast->line_num);
				return ERROR;
			}
                        
            if(isResult(name) && isInIf!=0){
                errorOccurred = 1; 
                fprintf(errorFile,"Line: %d: error: INVALID ASSIGNMENT, trying to write to a Uniform Pre-Defined variable inside an if/else statement\n",ast->line_num);
				return ERROR;
            }
            
            if(entry == NULL) {
                errorOccurred = 1; 
                fprintf(errorFile,"Line: %d: error: INVALID ASSIGNMENT, variable %s not defined\n", ast->line_num, name);
                return ERROR;
            }
            exp2 = entry->type_code;
            ast->assignment.type = (type_code) entry->type_code;
                          
			exp1 = semantic_check(ast->assignment.expression);

			if(exp1==ERROR || exp2 == ERROR)
				return ERROR;

			var = ast->assignment.expression;

                        
            if (entry->is_const) {
				errorOccurred = 1; 
				fprintf(errorFile,"Line: %d: error: cannot assign a value to a constant variable.\n",ast->line_num);
				return ERROR;
			}
                        
            if(exp2!=exp1){
				errorOccurred = 1; 
				fprintf(errorFile,"Line: %d: error: TYPE MISMATCH\n",ast->line_num);
				return ERROR;
			}
                        
			if (var->kind == VARIABLE_NODE)
				name = var->variable.id;
			else if (var->kind == ARRAY_INDEX_NODE)
				name = var->array_index.id;	
            else
                return exp2;
			
			if(isResult(name)){
				errorOccurred = 1; 
				fprintf(errorFile,"Line: %d: error: INVALID ASSIGNMENT, trying to read from a write-only variable\n",ast->line_num);
				return ERROR;
			}
                        
	        entry = symbol_find(name);
	        entry->is_init = 1;

			return exp2;
			break;
                        
		case NESTED_SCOPE_NODE:
            return semantic_check(ast->nested_scope);
			break;
                        
		case DECLARATION_NODE:
			isDecl = symbol_exists_in_this_scope(ast->declaration.id);

			if (checkPredefined(ast->declaration.id) == ERROR) {
				errorOccurred = 1; 
				fprintf(errorFile,"line: %d: error: Cannot declare Pre-defined variables\n", ast->line_num);
				return ERROR;
			}

			if(isDecl!= -1){
				errorOccurred = 1; 
               	fprintf(errorFile,"Line: %d: error: Variable already declared.\n",ast->line_num);
				return ERROR;
			}else{
				symbol_table_entry new_entry;
				new_entry.id = ast->declaration.id;
		  		new_entry.is_const = ast->declaration.is_const;
		  		new_entry.type_code = ast->declaration.type->type_node.type;
		  		new_entry.vec = getBaseForTypeCode(new_entry.type_code);
		  		new_entry.is_init = ast->declaration.expression == NULL ? 0 : 1;
		  		symbol_add(new_entry);
			}
                        
            exp2 = semantic_check(ast->declaration.type);
            if(ast->declaration.expression != NULL) {
                exp1 = semantic_check(ast->declaration.expression);
                if(exp1 == exp2)
                    return exp1;
                else {
                    errorOccurred = 1; 
                    fprintf(errorFile,"Line: %d: error: Type mismatch.\n",ast->line_num); 
                    return ERROR;
                }
            }
            else
                return exp2;
                        
			break;
                        
		case ARGUMENTS_NODE:
			if (ast->arguments.args && ast->arguments.expression) {
                exp1 = semantic_check(ast->arguments.args);
                exp2 = semantic_check(ast->arguments.expression);
            } else if (ast->arguments.args)
                exp2 = exp1 = semantic_check(ast->arguments.args);
            else 
                exp1 = exp2 = semantic_check(ast->arguments.expression);
                        
			if(exp1==ERROR || exp2 == ERROR)
				return ERROR;

			if(exp1==exp2){
				return exp1;
			}else{
				errorOccurred = 1; 
                fprintf(errorFile,"Line: %d: error: Type mismatch. All arguments are not of the same type.\n",ast->line_num); 
				return ERROR;
			}
			break;
                        
		case EXPRESSION_VARIABLE_NODE:
			return semantic_check(ast->expression_variable);
			break;
                        
		case UNKNOWN:
			errorOccurred = 1; 
			fprintf(errorFile,"Line: %d: error: UNKNOWN ERROR. You have successfully voted for Donald Trump!\n",ast->line_num);
			return ERROR;
			break;
                        
		default:
			errorOccurred = 1; 
			fprintf(errorFile,"Line: %d: error: SEGMENTATION FAULT. You have successfully crashed the compiler!\n",ast->line_num);
			return ERROR;
			break;

	}

  return 0; // failed checks
}
