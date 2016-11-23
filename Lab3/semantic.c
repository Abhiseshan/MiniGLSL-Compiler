#include "semantic.h"

int semantic_check( node *ast) {
  return 0; // failed checks
}

int checkPredefined(char* name) {
	if strcmp(name, "gl_FragColor") || strcmp(name, "gl_FragDepth") || strcmp(name, "gl_FragCoord") || strcmp(name, "gl_TexCoord") || strcmp(name, " gl_Color")  || 
		strcmp(name, "gl_Secondary") || strcmp(name, "gl_FogFragCoord") || strcmp(name, "gl_Light_Half") || strcmp(name, " gl_Light_Ambient") || 
		strcmp(name, "gl_Material_Shininess") || strcmp(name, "env1") || strcmp(name, "env2") || strcmp(name, "env3")

		return ERROR;

	return 1;
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

	int kind, depth = 0;
	kind = ast->kind;

	switch(kind){
	case EXPRESSION_NODE:
		return 1;
		break;
	case PREN_EXPRESSION_NODE:
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
	case FLOAT_NODE:
		return 1;
		break;
	case BOOL_NODE:
		return 1;
		break;
	case IDENT_NODE:
		return 1;
		break;
	case VAR_NODE:
		return 1;
		break;
	case ARRAY_NODE:
		return 1;
		break;
	case FUNCTION_NODE:
		return 1;
		break;
	case CONSTRUCTOR_NODE:
		return 1;
		break;
	case ARGUMENTS_NODE:
                if(ast->arguments.args == NULL) return 1;
		return 1 + checkDepth(ast->arguments.args);
		break;
	default :
		printf("check depth failed: %d\n", kind);
		return ERROR;
		break;
	}

	return 0;
}

int in_scope=0;


int semantic_check(node *ast) {

	if(ast==NULL) {
            fprintf(errorFile,"Semantic function visited a NULL node\n");
            return ERROR;
        }

	int kind;
	int type;
	int depth;
	int tmp;
	int exp1, exp2;
	char * name;
	int index;
	kind = ast->kind;
	int isDecl=0;

	switch(kind){
		case ENTER_SCOPE_NODE:
			in_scope++;
			scope_enter();
			exp1 = semantic_check(ast->enter_scope.scope);
			scope_exit();
			in_scope--;
			return exp1;
			break;

		case SCOPE_NODE:
			exp1 = semantic_check(ast->scope.declarations);
			exp2 = semantic_check(ast->scope.statements);

			if(exp1==ERROR || exp2 == ERROR)
                            return ERROR;

			return 0;
			break;

		case DECLARATIONS_NODE:
			exp1 = semantic_check(ast->declarations.declarations);
			exp2 = semantic_check(ast->declarations.declaration);

			if(exp1==ERROR || exp2 == ERROR)
                            return ERROR;

			return exp2;
			break;

		case STATEMENTS_NODE:
			exp1 = semantic_check(ast->statements.statements);
			exp2 = semantic_check(ast->statements.statement);

			if(exp1==ERROR || exp2 == ERROR)
				return ERROR;

			return exp2;
			break;
                        
		case NESTED_EXPRESSION:
			return semantic_check(ast->nested_expression);
			break;

		case UNARY_EXPRESION_NODE:
			exp1 = semantic_check(ast->unary.right);

			if(exp1==ERROR)
				return ERROR;

			switch (ast->unary.op){
				case MINUS_OP:
					if(exp1 == BOOL || exp1==BVEC2|| exp1==BVEC3|| exp1==BVEC4){
						printf("Line: %d: error: found BOOL, expecting INT or FLOAT\n",ast->line_num);						
						eturn ERROR;
					} else{
						return exp1;
					}
					break;

				case NOT_OP:
					if(exp1!=BOOL || exp1!=BVEC2 || exp1!=BVEC3 || exp1!=BVEC4){
						if(exp1 == INT || exp1==IVEC2 || exp1==IVEC3|| exp1==IVEC4){
							printf("Line: %d: error: found INTEGER, expecting BOOL\n",ast->line_num);						
						}
						else if(exp1 == FLOAT || exp1==VEC2 || exp1==VEC3|| exp1==VEC4){
							printf("Line: %d: error: found FLOAT, expecting BOOL\n",ast->line_num);						
						}
						else {
							printf("Line: %d: error: found UNKNOWN TYPE, expecting BOOL\n",ast->line_num);						
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
					printf("Line: %d: error: TYPE MISMATCH, found INT, expecting BOOL\n",ast->line_num);						
					return ERROR;
				}else if(exp1 == INT || exp1 == IVEC2 || exp1 == IVEC3 || exp1 == IVEC4){
					printf("Line: %d: error: TYPE MISMATCH, found INT, expecting BOOL\n",ast->line_num);						
					return ERROR;
				}else if(exp2 == FLOAT || exp2 == VEC2 || exp2 == VEC3 || exp2 == VEC4){
					printf("Line: %d: error: TYPE MISMATCH, found FLOAT, expecting BOOL\n",ast->line_num);						
					return ERROR;
				}else if(exp1 == FLOAT || exp1 == VEC2 || exp1 == VEC3 || exp1 == VEC4){
					printf("Line: %d: error: TYPE MISMATCH, found FLOAT, expecting BOOL\n",ast->line_num);						
					return ERROR;
				}else if(exp2 != exp1){
					printf("Line: %d: error: TYPE MISMATCH, operation can only be performed on vectors of same size\n",ast->line_num);						
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
					printf("Line: %d: error: TYPE MISMATCH, found BOOL, expecting INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp2== IVEC2 || exp2==IVEC3 || exp2==IVEC4){	
					printf("Line: %d: error: TYPE MISMATCH, found INT VECTOR, expecting SCALAR INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp2== VEC2 || exp2==VEC3 || exp2==VEC4){
					printf("Line: %d: error: TYPE MISMATCH, found FLOAT VECTOR, expecting SCALAR INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp2== BVEC2 || exp2==BVEC3 || exp2==BVEC4){
					printf("Line: %d: error: TYPE MISMATCH, found BOOL VECTOR, expecting SCALAR INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp1== IVEC2 || exp1==IVEC3 || exp1==IVEC4){
					printf("Line: %d: error: TYPE MISMATCH, found INT VECTOR, expecting SCALAR INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp1== VEC2 || exp1==VEC3 || exp1==VEC4){
					printf("Line: %d: error: TYPE MISMATCH, found FLOAT VECTOR, expecting SCALAR INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp1== BVEC2 || exp1==BVEC3 || exp1==BVEC4){
					printf("Line: %d: error: TYPE MISMATCH, found BOOL VECTOR, expecting SCALAR INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp2!=exp1){
					printf("Line: %d: error: TYPE MISMATCH, expecting operands of same type on both sides\n",ast->line_num);						
					return ERROR;
				}
			}

			if(ast->binary.op==EQ_OP || ast->binary.op==NEQ_OP){
				if(exp2==exp1){
					if(exp2==BOOL || exp2 == BVEC2 || exp2 == BVEC3 || exp2 == BVEC4){
						printf("Line: %d: error: TYPE MISMATCH, found BOOL, expecting INT or FLOAT\n",ast->line_num);						
					}
				}else{
						printf("Line: %d: error: TYPE MISMATCH, expecting operands of same type on both sides\n",ast->line_num);						
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
					printf("Line: %d: error: TYPE MISMATCH, found BOOL, expecting INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp1==BOOL || exp1 == BVEC2 || exp1 == BVEC3 || exp1 == BVEC4){
					printf("Line: %d: error: TYPE MISMATCH, found BOOL, expecting INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp2!=exp1){
					printf("Line: %d: error: TYPE MISMATCH, expecting operands of same type on both sides\n",ast->line_num);						
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
					printf("Line: %d: error: TYPE MISMATCH, found BOOL, expecting INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp1==BOOL || exp1 == BVEC2 || exp1 == BVEC3 || exp1 == BVEC4){
					printf("Line: %d: error: TYPE MISMATCH, found BOOL, expecting INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp2==exp1){
					return exp2;
				}else if(exp2!=exp1){
					printf("Line: %d: error: TYPE MISMATCH, expecting operands of same type on both sides\n",ast->line_num);						
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
					printf("Line: %d: error: TYPE MISMATCH, found BOOL, expecting INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp1==BOOL || exp1 == BVEC2 || exp1 == BVEC3 || exp1 == BVEC4){
					printf("Line: %d: error: TYPE MISMATCH, found BOOL, expecting INT or FLOAT\n",ast->line_num);						
					return ERROR;
				}else if(exp2!=exp1){
					printf("Line: %d: error: TYPE MISMATCH, expecting operands of same type on both sides\n",ast->line_num);						
					return ERROR;
				}else{
					printf("Line: %d: error: TYPE MISMATCH, expecting operands of SCALAR type\n",ast->line_num);						
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
			int type;
			type = checkDeclaredInScope(ast->variable.id);
			if(type==ERROR){
				printf("Line: %d: error: UNDEFINED VARIABLE, %s not defined in scope before it is used.\n",ast->line_num, ast->variable.id);						
				return ERROR;
			}else{
				return type;
			}
			break;

		case ARRAY_INDEX_NODE:
                        symbol_table_entry *entry = symbol_find(ast->array_index.id);
                        int type_code = entry->type_code;
                        ast->array_index.type = type_code;

			index = ast->array_index.index;
			switch(type_code){
			case IVEC2:
				if(index>=2 || index < 0){
					printf("Line: %d: error: Index out of bounds\n",ast->line_num);
					return ERROR;
				}
				break;
			case IVEC3:
				if(index>=3 || index < 0){
					printf("Line: %d: error: Index out of bounds\n",ast->line_num);
					return ERROR;
				}
				break;
			case IVEC4:
				if(index>=4 || index < 0){
					printf("Line: %d: error: Index out of bounds\n",ast->line_num);
					return ERROR;
				}
				break;
			case BVEC2:
				if(index>=2 || index < 0){
					printf("Line: %d: error: Index out of bounds\n",ast->line_num);
					return ERROR;
				}
				break;
			case BVEC3:
				if(index>=3 || index < 0){
					printf("Line: %d: error: Index out of bounds\n",ast->line_num);
					return ERROR;
				}
				break;
			case BVEC4:
				if(index>=4 || index < 0){
					printf("Line: %d: error: Index out of bounds\n",ast->line_num);
					return ERROR;
				}
				break;
			case VEC2:
				if(index>=2 || index < 0){
					printf("Line: %d: error: Index out of bounds\n",ast->line_num);
					return ERROR;
				}
				break;
			case VEC3:
				if(index>=3 || index < 0){
					printf("Line: %d: error: Index out of bounds\n",ast->line_num);
					return ERROR;
				}
				break;
			case VEC4:
				if(index>=4 || index < 0){
					printf("Line: %d: error: Index out of bounds\n",ast->line_num);
					return ERROR;
				}
				break;
			default:
					printf("Line: %d: error: Cannot read index of scalar data types\n",ast->line_num);
				return ERROR;
			}

			break;

		case FUNCTION_NODE:
			type = semantic_check(ast->function.args); //Need to figre out if it is printing if the argeuments passed into the function is right.
			if(type==ERROR)
				return ERROR;

			if(ast->function.func_code == 2){ //rsq
                            if(type==FLOAT){
                                    return FLOAT;
                            }
                            if(type==INT){
                                    return INT;
                            }
                            else {
                                printf("Line: %d error: expecting int or float to function rsq, got %s", getTypeString(type));
                                return ERROR;
                            }
			}else if(ast->function.func_code == 0){ //dp3
				if(type==VEC4 || type == VEC3){
					return FLOAT;
				}
				if(type==IVEC4 || type == IVEC3){
					return INT;
				}
                                else {
                                    printf("Line: %d error: expecting vec4, vec3, ivec4, or ivec3 to function dp3, got %s", getTypeString(type));
                                    return ERROR;
                                }
			}else if (ast->function.func_code == 1){ //lit
                            if(type != VEC4)  {
                                printf("Line: %d error: expecting vec4 to function lit, got %s", getTypeString(type));
                                return ERROR;
                            }
                            return FLOAT;
			}

			printf("Line: %d: error: UNRECOGNIZED function %s\n",ast->line_num, getFuncString(ast->function.func_code));
			return ERROR;

			break;

		case CONSTRUCTOR_NODE:
			exp2 = semantic_check(ast->constructor.type);
			exp1 = semantic_check(ast->constructor.args);

			if(exp1==ERROR || exp2 == ERROR)
				return ERROR;

			depth = checkDepth(ast->constructor.arguments);

			switch(exp2){
				case IVEC2:
					if(depth>2){
						printf("Line: %d: error: TOO MANY ARGUMENTS, expecting 2, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					else if (depth<2){
						printf("Line: %d: error: TOO FEW ARGUMENTS, expecting 2, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					break;
				case IVEC3:
					if(depth>3){
						printf("Line: %d: error: TOO MANY ARGUMENTS, expecting 3, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					else if (depth<3){
						printf("Line: %d: error: TOO FEW ARGUMENTS, expecting 3, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					break;
				case IVEC4:
					if(depth>4){
						printf("Line: %d: error: TOO MANY ARGUMENTS, expecting 4, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					else if (depth<4){
						printf("Line: %d: error: TOO FEW ARGUMENTS, expecting 4, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					break;
				case BVEC2:
					if(depth>2){
						printf("Line: %d: error: TOO MANY ARGUMENTS, expecting 2, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					else if (depth<2){
						printf("Line: %d: error: TOO FEW ARGUMENTS, expecting 2, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					break;
				case BVEC3:
					if(depth>3){
						printf("Line: %d: error: TOO MANY ARGUMENTS, expecting 3, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					else if (depth<3){
						printf("Line: %d: error: TOO FEW ARGUMENTS, expecting 3, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					break;
				case BVEC4:
					if(depth>4){
						printf("Line: %d: error: TOO MANY ARGUMENTS, expecting 4, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					else if (depth<4){
						printf("Line: %d: error: TOO FEW ARGUMENTS, expecting 4, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					break;	
				case VEC2:
					if(depth>2){
						printf("Line: %d: error: TOO MANY ARGUMENTS, expecting 2, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					else if (depth<2){
						printf("Line: %d: error: TOO FEW ARGUMENTS, expecting 2, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					break;
				case VEC3:
					if(depth>3){
						printf("Line: %d: error: TOO MANY ARGUMENTS, expecting 3, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					else if (depth<3){
						printf("Line: %d: error: TOO FEW ARGUMENTS, expecting 3, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					break;
				case VEC4:
					if(depth>4){
						printf("Line: %d: error: TOO MANY ARGUMENTS, expecting 4, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					else if (depth<4){
						printf("Line: %d: error: TOO FEW ARGUMENTS, expecting 4, got %d\n",ast->line_num, depth);
						return ERROR;
					}
					break;
				default:
					if(depth>1){
						printf("Line: %d: error: UNKNOWN CONSTRUCTOR\n",ast->line_num);
					}
			}

			if(exp2==IVEC2 || exp2==IVEC3 || exp2==IVEC4){
				if(exp1==INT){
					return exp2;
				}
			}

			else if(exp2==BVEC2 || exp2==BVEC3 || exp2==BVEC4){
				if(exp1==BOOL){
					return exp2;
				}
			}

			else if(exp2==VEC2 || exp2==VEC3 || exp2==VEC4){
				if(exp1==FLOAT){
					return exp2;
				}
			}

			else {
				printf("Line: %d: error: TYPE MISMATCH\n",ast->line_num);
				return ERROR;
                        }
			break;
                            
		case TYPE_NODE:
			return ast->type.type_code;
			break;
                        
		case IF_STATEMENT_NODE:
			exp2 = semantic_check(ast->if_statement.condition);

			if(exp2 == ERROR)
				return ERROR;

			if(exp2!=BOOL){
				printf("ERROR: Expression must evaluate to bool line: %d\n", ast->line_num);
				return ERROR;
			}
			if(ast->if_statement.else_statement != NULL) 
                            semantic_check(ast->if_statement.else_statement);
			semantic_check(ast->if_statement.then_statement);
			break;
                        
		case ASSIGNMENT_NODE:

			node *var = ast->assignment.var;
            if(var->kind == VARIABLE_NODE)
                name = var->variable.id;
            else
                name = var->array_index.id;
            
            symbol_table_entry *entry = symbol_find(name);
            if(entry == NULL) {
                printf("Line: %d: error: INVALID ASSIGNMENT, variable %s not defined %d\n", name, ast->line_num);
                return ERROR;
            }
            exp2 = entry->vec;
            ast->assignment.type = entry->type_code;
                        
			exp1 = semantic_check(ast->assignment.expression);

			if(exp1==ERROR || exp2 == ERROR)
				return ERROR;

			if(checkPredefined(ast->assignment.expression.variable.id)==1){
				printf("Line: %d: error: INVALID ASSIGNMENT, trying to assign to a const or read-only variable %d\n",ast->line_num);
				return ERROR;
			}

			if(checkPredefined(ast->assignment.var.variable.id)==1){
				printf("ERROR Cannot Read from RESULT modified pre-defined variable line: %d \n",ast->assignment.line);
				return ERROR;
			}

			//We have to modify this to our own approach. Releated to predefined variableds 
			/*if(ast->assignment.left->kind == VAR_NODE){
				type = getState(ast->assignment.left->variable_exp.identifier);
				if(type == ATTRIBUTE || type == UNIFORM || type==CONST_S){
					printf("Line: %d: error: INVALID ASSIGNMENT, trying to assign to a const or read-only variable %d\n",ast->line_num);
					return ERROR;
				}
			}

			//We have to modify this to our own approach. Releated to predefined variableds 
			if(ast->assignment.right->kind == VAR_NODE){
				type = getState(ast->assignment.right->variable_exp.identifier);

				if(type == RESULT){
					printf("ERROR Cannot Read from RESULT modified pre-defined variable line: %d \n",ast->assignment.line);
					return ERROR;
				}
			}*/

			if(exp2!=exp1){
				printf("Line: %d: error: TYPE MISMATCH\n",ast->line_num);
				return ERROR;
			}

			return exp2;
			break;
                        
		case NESTED_SCOPE_NODE:
            return semantic_check(ast->nested_scope);
			break;
                        
		case DECLARATION_NODE:
			isDecl = symbol_exists_in_scope(ast->declaration.id);

			if (checkPredefined(ast->declaration.id) == ERROR) {
				printf("line: %d: error: Cannot declare Pre-defined variables\n", );
				return ERROR;
			}

			if(isDecl==ERROR){
				printf("Line: %d: error: Variable already declared.\n",ast->line_num);
				return ERROR;
			}else{
				symbol_table_entry new_entry;
				new_entry.id = ast->declaration.id;
		  		new_entry.is_const = ast->declaration.is_const;
		  		new_entry.type_code = ast->declaration.type->type_node.type;
		  		new_entry.vec = getBaseForTypeCode(new_entry.type_code);
		  		new_entry.is_init = ast->declaration.expression == NULL ? 0 : 1;
		  		symbol_add(new_entry);

				return semantic_check(ast->declaration.type);
			}
			break;
                        
		case ASSIGNMENT_NODE:
			exp2 = semantic_check(ast->declaration_assignment.type);
			exp1 = semantic_check(ast->declaration_assignment.value);

			if(exp1==ERROR || exp2 == ERROR)
				return ERROR;

			isDecl = symbol_exists_in_this_scope(ast->declaration_assignment.iden);

			if(isDecl==ERROR){
				printf("Line: %d: error: Variable not declared in scope.\n",ast->line_num);
				return ERROR;
			}
			
			if (is_symobl_const(ast->declaration_assignment.iden)) {
				printf("Line: %d: error: cannot assign a value to a constant variable.\n",ast->line_num);
				return ERROR;
			}



			//We have to modify this to our own approach. Releated to predefined variableds 
			if(ast->const_declaration_assignment.type->kind == VAR_NODE){
				type = getState(ast->const_declaration_assignment.type->variable_exp.identifier);

				if(type == ATTRIBUTE || type == UNIFORM){
					printf("ERROR Cannot assign to pre defined read-only type line: %d\n", ast->declaration_assignment.line);
					return ERROR;
				}
			}

			//We have to modify this to our own approach. Releated to predefined variableds 
			if(ast->declaration_assignment.value->kind == VAR_NODE){
				type = getState(ast->declaration_assignment.value->variable_exp.identifier);

				if(type == RESULT){
					printf("ERROR Cannot Read from RESULT modified pre-defined variable line: %d\n", ast->declaration_assignment.line);
					return ERROR;
				}
			}

			if(exp2==exp1){
				return exp2;
			}

			if(exp2==IVEC2 || exp2==IVEC3 || exp2==IVEC4){
				if(exp1==INT){
					return INT;
				}
			}

			if(exp2==BVEC2 || exp2==BVEC3 || exp2==BVEC4){
				if(exp1==BOOL){
					return BOOL;
				}
			}

			if(exp2==VEC2 || exp2==VEC3 || exp2==VEC4){
				if(exp1==FLOAT){
					return FLOAT;
				}
			}


			if(exp2!=exp1){
				printf("Line: %d: error: TYPE MISMATCH\n",ast->line_num);
				return ERROR;
			}

			if(exp2!=exp1){
				printf("Line: %d: error: TYPE MISMATCH\n",ast->line_num);
				return ERROR;
			}

			break;
		//Eric necessary?
		//Added constant to the prev case. 
		case 25:
			//printf("CONST_DECLARATION_ASSIGNMENT_NODE %d\n", kind);
			exp2 = semantic_check(ast->const_declaration_assignment.type);
			exp1 = semantic_check(ast->const_declaration_assignment.value);

			if(exp1==ERROR || exp2 == ERROR)
				return ERROR;

			if(ast->const_declaration_assignment.type->kind == VAR_NODE){
				type = getState(ast->const_declaration_assignment.type->variable_exp.identifier);
				if(type == ATTRIBUTE || type == UNIFORM){
					printf("ERROR Cannot assign to pre defined read-only type line: %d\n", ast->const_declaration_assignment.line);
					return ERROR;
				}
			}

			if(ast->const_declaration_assignment.value->kind == VAR_NODE){
				type = getState(ast->const_declaration_assignment.value->variable_exp.identifier);

				if(type == RESULT){
					printf("ERROR Cannot Read from RESULT modified pre-defined variable line: %d.\n",ast->const_declaration_assignment.line);
					return ERROR;
				}
			}

			if(ast->const_declaration_assignment.value->kind == INT_NODE ||
					ast->const_declaration_assignment.value->kind == BOOL_NODE ||
					ast->const_declaration_assignment.value->kind == FLOAT_NODE ||
					type == CONST_S || type == UNIFORM ) {
				;
			}else{
				printf("ERROR const var must be initialized with a literal value or uniform variable line:%d\n",ast->const_declaration_assignment.line);
				return ERROR;
			}

			if(exp2==exp1){
				return exp2;
			}

			if(exp2==IVEC2 || exp2==IVEC3 || exp2==IVEC4){
				if(exp1==INT){
					return INT;
				}
			}

			if(exp2==BVEC2 || exp2==BVEC3 || exp2==BVEC4){
				if(exp1==BOOL){
					return BOOL;
				}
			}

			if(exp2==VEC2 || exp2==VEC3 || exp2==VEC4){
				if(exp1==FLOAT){
					return FLOAT;
				}
			}

			if(exp2!=exp1){
				printf("ERROR types must match for assignement line:%d\n", ast->const_declaration_assignment.line);
				return ERROR;
			}

			break;
                        
		case ARGUMENTS_NODE:
			exp1 = semantic_check(ast->arguments.args);
			exp2 = semantic_check(ast->arguments.expression);
                        
			if(exp1==ERROR || exp2 == ERROR)
				return ERROR;

			if(exp1==exp2){
				return exp1;
			}else{
				printf("ERROR ARGUMENTS_COMMA_NODE line:%d\n", ast->line_num);
				return ERROR;
			}
			break;
		case EXPRESSION_VARIABLE_NODE:
			return semantic_check(ast->expression_variable);
			break;
		case UNKNOWN:
			printf("Line: %d: error: UNKNOWN ERROR. You have successfully crashed the compiler!\n",ast->line_num);
			return ERROR;
			break;
		default:
			printf("Line: %d: error: DEFAULT ERROR. You have successfully crashed the compiler!\n",ast->line_num);
			return ERROR;
			break;

	}



  return 0; // failed checks
}
