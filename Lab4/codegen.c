#include "codegen.h"

#define print(...) fprintf(fragFile,__VA_ARGS__)

int tmpCount = 1;
int prmCount = 1;
int condCount = 1;
int if_state = 0;
int maxTmpCount = 0;
int initFlag = 0;

int ifStack[256];
int ifStackTop, ifStackElem;

FILE * fragFile;

enum {
	IN_NONE, IN_THEN, IN_ELSE, IN_COND
} if_states;

int isTempRes(int kind) {
	if (kind == UNARY_EXPRESION_NODE || kind == BINARY_EXPRESSION_NODE
			|| kind == FUNCTION_NODE) {
		return 1;
	}
	return 0;
}

char toChar(int n) {
	switch (n) {
	case 0:
		return 'x';
	case 1:
		return 'y';
	case 2:
		return 'z';
	case 3:
		return 'w';
	default:
		return '0';
	}
}

int printVar(node *ast) {
	char *name = ast->variable.id;

	if (strcmp(name, "gl_FragColor") == 0) {
		print("result.color");
		return 0;

	} else if (strcmp(name, "gl_FragDepth") == 0) {
		print("result.depth");
		return 0;

	} else if (strcmp(name, "gl_FragCoord") == 0) {
		print("fragment.position");
		return 0;

	} else if (strcmp(name, "gl_TexCoord") == 0) {
		print("fragment.texcoord");
		return 0;

	} else if (strcmp(name, "gl_Color") == 0) {
		print("fragment.color");
		return 0;

	} else if (strcmp(name, "gl_Secondary") == 0) {
		print("fragment.color.secondary");
		return 0;

	} else if (strcmp(name, "gl_FogFragCoord") == 0) {
		print("fragment.fogcoord");
		return 0;

	} else if (strcmp(name, "gl_Light_Half") == 0) {
		print("state.light[0].half");
		return 0;

	} else if (strcmp(name, "gl_Light_Ambient") == 0) {
		print("state.lightmodel.ambient");
		return 0;

	} else if (strcmp(name, "gl_Material_Shininess") == 0) {
		print("state.material.shininess");
		return 0;

	} else if (strcmp(name, "env1") == 0) {
		print("program.env[1]");
		return 0;

	} else if (strcmp(name, "env2") == 0) {
		print("program.env[2]");
		return 0;

	} else if (strcmp(name, "env3") == 0) {
		print("program.env[3]");
		return 0;

	} else {
		print("%s", name);
		return 0;
	}
	return 0;
}

int printArray(node *ast) {
	char *name = ast->array_index.id;
	int index = ast->array_index.index;

	if (strcmp(name, "gl_FragColor") == 0) {
		print("result.color.%c", toChar(index));
		return 0;

	} else if (strcmp(name, "gl_FragDepth") == 0) {
		print("result.depth.%c", toChar(index));
		return 0;

	} else if (strcmp(name, "gl_FragCoord") == 0) {
		print("fragment.position.%c", toChar(index));
		return 0;

	} else if (strcmp(name, "gl_TexCoord") == 0) {
		print("fragment.texcoord.%c", toChar(index));
		return 0;

	} else if (strcmp(name, "gl_Color") == 0) {
		print("fragment.color.%c", toChar(index));
		return 0;

	} else if (strcmp(name, "gl_Secondary") == 0) {
		print("fragment.color.secondary.%c", toChar(index));
		return 0;

	} else if (strcmp(name, "gl_FogFragCoord") == 0) {
		print("fragment.fogcoord.%c", toChar(index));
		return 0;

	} else if (strcmp(name, "gl_Light_Half") == 0) {
		print("state.light[0].half.%c", toChar(index));
		return 0;

	} else if (strcmp(name, "gl_Light_Ambient") == 0) {
		print("state.lightmodel.ambient.%c", toChar(index));
		return 0;

	} else if (strcmp(name, "gl_Material_Shininess") == 0) {
		print("state.material.shininess.%c", toChar(index));
		return 0;

	} else if (strcmp(name, "env1") == 0) {
		print("program.env[1].%c", toChar(index));
		return 0;

	} else if (strcmp(name, "env2") == 0) {
		print("program.env[2].%c", toChar(index));
		return 0;

	} else if (strcmp(name, "env3") == 0) {
		print("program.env[3].%c", toChar(index));
		return 0;

	} else {
		print("%s.%c", name, toChar(index));
		return 0;
	}

}

int initAll(node *ast){
	if(initFlag==0){
		s_init(&ifStackTop);

		fragFile = fopen ("frag.txt","w");
	}
	initFlag = 1;
	
	print("!!ARBfp1.0\n");
	int tmp = genCode(ast);
	print("END\n"); 
	return tmp;
}

int genCode(node *ast) {

	if (ast == NULL) {
		//printf("-1 empty tree\n");
		return 0;
	}

	int kind;
	int type;
	int exp1, exp2;
	kind = ast->kind;
	int val;


	switch (kind) {
	case ENTER_SCOPE_NODE:
		//printf("ENTER_SCOPE_NODE %d\n", kind);

		exp1 = genCode(ast->enter_scope);
		return exp1;

	case SCOPE_NODE:
		//printf("SCOPE_NODE %d\n", kind);
		if (ast->scope.declarations)
            exp1 = genCode(ast->scope.declarations);
        else 
            exp1 = 1;
        if(ast->scope.statements) 
            exp2 = genCode(ast->scope.statements);
        else
            exp2 = 1;

		if(exp1==-1 || exp2 == -1)
            return -1;

		return 0;

	case DECLARATIONS_NODE:
		//printf("DECLARATIONS_NODE %d\n", kind);
		if(ast->declarations.declarations)
            exp1 = genCode(ast->declarations.declarations);
        else 
            exp1 = 1;

        exp2 = genCode(ast->declarations.declaration);

		if(exp1==-1 || exp2 == -1)
            return -1;

		return exp2;

	case STATEMENTS_NODE:
		//printf("STATEMENTS_NODE %d\n", kind);
		if (ast->statements.statements)
            exp1 = genCode(ast->statements.statements);
        else
            exp1 = 1;
        if(ast->statements.statement)
            exp2 = genCode(ast->statements.statement);
        else
            exp2 = 1;
                
		if(exp1==-1 || exp2 == -1)
			return -1;

		return exp2;

	case NESTED_EXPRESSION_NODE:
		//printf("PREN_EXPRESSION_NODE %d\n", kind);
		return genCode(ast->nested_expression);

	case UNARY_EXPRESION_NODE:
		//printf("UNARY_EXPRESION_NODE %d\n", kind);
		//printf("Operator: %d\n", ast->unary_expr.op);
		val = tmpCount++;
		print("TEMP tmpVar%d;\n", val);

		if (isTempRes(ast->unary.right->kind)) {
			exp1 = genCode(ast->unary.right);
			switch (ast->unary.op) {
				case MINUS_OP:
					print("SUB tmpVar%d, 0.0, tmpVar%d;\n", val, exp1);
					return val;
				case NOT_OP:
					print("NOT tmpVar%d, tmpVar%d;\n", val, exp1);
					return val;
			}
		} else {
			switch (ast->unary.op) {
				case MINUS_OP:
					print("MUL tmpVar%d, ", val);
					exp1 = genCode(ast->unary.right);
					print(" , -1.0;\n");
					return val;
				case NOT_OP:
					print("NOT tmpVar%d, ");
					exp1 = genCode(ast->unary.right);
					print(";\n", val);
					return val;
				}
		}

	case BINARY_EXPRESSION_NODE:
		//printf("BINARY_EXPRESSION_NODE %d\n", kind);
		//printf("Operator: %d\n", ast->binary_expr.op);

		int t1, t2, t3;
		exp2 = 0;
		exp1 = 0;
		val = tmpCount++;
		if (val > maxTmpCount) {
			print("TEMP tmpVar%d;\n", val);
			maxTmpCount++;
		}

		if (isTempRes(ast->binary.left->kind)) {
			exp2 = genCode(ast->binary.left);
		}

		if (isTempRes(ast->binary.right->kind)) {
			exp1 = genCode(ast->binary.right);
		}

		if (ast->binary.op == AND_OP) {
			print("ADD ");
			print("tmpVar%d, ", val);
			if (exp2 == 0) {
				exp2 = genCode(ast->binary.left);
			} else {
				print("tmpVar%d ", exp2);
			}
			print(",");

			if (exp1 == 0) {
				exp1 = genCode(ast->binary.right);
			} else {
				print("tmpVar%d ", exp1);
			}
			print(";\n", val);
			print("SUB tmpVar%d, tmpVar%d, 1.0;\n", val, val);
			val = tmpCount++;
			if (val > maxTmpCount) {
				print("TEMP tmpVar%d;\n", val);
				maxTmpCount++;
			}

			print("CMP tmpVar%d, tmpVar%d, -1.0, 1.0;\n", val, val-1);
			print("MOV tmpVar%d, tmpVar%d;\n", val-1, val);
			tmpCount--;
			return val - 1;
		} else if (ast->binary.op == OR_OP) {
			print("ADD ");
			//
			print("tmpVar%d, ", val);
			if (exp2 == 0) {
				exp2 = genCode(ast->binary.left);
			} else {
				print("tmpVar%d ", exp2);
			}
			print(",");

			if (exp1 == 0) {
				exp1 = genCode(ast->binary.right);
			} else {
				print("tmpVar%d ", exp1);
			}
			print(";\n", val);
			print("ADD tmpVar%d, tmpVar%d, 1.0;\n", val, val);
			val = tmpCount++;
			if (val > maxTmpCount) {
				print("TEMP tmpVar%d;\n", val);
				maxTmpCount++;
			}
			print("CMP tmpVar%d, tmpVar%d, -1.0, 1.0;\n", val, val-1);
			print("MOV tmpVar%d, tmpVar%d;\n", val-1, val);
			tmpCount--;
			return val - 1;
		} else if (ast->binary.op == LT_OP) {
			print("SLT ");
		} else if (ast->binary.op == LEQ_OP) {
			print("SGE ");
			//
			print("tmpVar%d, ", val);
			if (exp2 == 0) {
				exp2 = genCode(ast->binary.left);
			} else {
				print("tmpVar%d ", exp2);
			}
			print(",");

			if (exp1 == 0) {
				exp1 = genCode(ast->binary.right);
			} else {
				print("tmpVar%d ", exp1);
			}
			print(";\n", val);
			t1 = val;
			t2 = t1 + 1;
			if (t2 > maxTmpCount) {
				print("TEMP tmpVar%d;\n", t2);
				maxTmpCount++;
			}

			print("SGE tmpVar%d,", t2);
			if (exp1 == 0) {
				exp1 = genCode(ast->binary.right);
			} else {
				print("tmpVar%d ", exp1);
			}
			print(",");

			if (exp2 == 0) {
				exp2 = genCode(ast->binary.left);
			} else {
				print("tmpVar%d ", exp2);
			}
			print(";\n", val);

			print("ADD tmpVar%d, tmpVar%d, tmpVar%d;\n", t2, t2, t1);
			print("SUB tmpVar%d, tmpVar%d, 2.0;\n", t2, t2);
			print("CMP tmpVar%d, tmpVar%d, -1.0, 1.0;\n", t1, t2);
			t3 = t2 + 1;
			print("SUB tmpVar%d,", t2);
			if (exp2 == 0) {
				exp2 = genCode(ast->binary.left);
			} else {
				print("tmpVar%d ", exp2);
			}
			print(",");

			if (exp1 == 0) {
				exp1 = genCode(ast->binary.right);
			} else {
				print("tmpVar%d ", exp1);
			}
			print(";\n", val);
			if (t3 > maxTmpCount) {
				print("TEMP tmpVar%d;\n", t3);
				maxTmpCount++;
			}
			print("SLT tmpVar%d, tmpVar%d, 0.0;\n", t3, t2);
			print("ADD tmpVar%d, tmpVar%d, tmpVar%d;\n", t3, t3, t1);
			print("SUB tmpVar%d, tmpVar%d,1.0;\n", t3, t3);
			print("CMP tmpVar%d, tmpVar%d, -1.0, 1.0;\n", t1, t3);
			tmpCount = t1 + 1;
			return t1;
		} else if (ast->binary.op == GT_OP) {
			print("SGE ");
			//
			print("tmpVar%d, ", val);
			if (exp2 == 0) {
				exp2 = genCode(ast->binary.left);
			} else {
				print("tmpVar%d ", exp2);
			}
			print(",");

			if (exp1 == 0) {
				exp1 = genCode(ast->binary.right);
			} else {
				print("tmpVar%d ", exp1);
			}
			print(";\n", val);
			t1 = val;
			t2 = t1 + 1;

			if (t2 > maxTmpCount) {
				print("TEMP tmpVar%d;\n", t2);
				maxTmpCount++;
			}

			print("SGE tmpVar%d,", t2);
			if (exp1 == 0) {
				exp1 = genCode(ast->binary.right);
			} else {
				print("tmpVar%d ", exp1);
			}
			print(",");

			if (exp2 == 0) {
				exp2 = genCode(ast->binary.left);
			} else {
				print("tmpVar%d ", exp2);
			}
			print(";\n", val);

			print("ADD tmpVar%d, tmpVar%d, tmpVar%d;\n", t2, t2, t1);
			print("SUB tmpVar%d, tmpVar%d, 2.0;\n", t2, t2);
			print("CMP tmpVar%d, tmpVar%d, 1.0, 0;\n", t1, t2);
			t3 = t2 + 1;

			if (t3 > maxTmpCount) {
				print("TEMP tmpVar%d;\n", t3);
				maxTmpCount++;
			}
			print("SGE tmpVar%d,", t3);
			if (exp2 == 0) {
				exp2 = genCode(ast->binary.left);
			} else {
				print("tmpVar%d ", exp2);
			}
			print(",");

			if (exp1 == 0) {
				exp1 = genCode(ast->binary.right);
			} else {
				print("tmpVar%d ", exp1);
			}
			print(";\n", val);

			print("SUB tmpVar%d, tmpVar%d, 2.0;\n",t3,t3);
			print("CMP tmpVar%d, tmpVar%d, -1.0,1.0;\n", t1,t3);
			tmpCount = t1 + 1;
			return t1;

			//------//

		} else if (ast->binary.op == GEQ_OP) {
			print("SGE ");
		} else if (ast->binary.op == EQ_OP) {
			print("SGE ");
			//
			print("tmpVar%d, ", val);
			if (exp2 == 0) {
				exp2 = genCode(ast->binary.left);
			} else {
				print("tmpVar%d ", exp2);
			}
			print(",");

			if (exp1 == 0) {
				exp1 = genCode(ast->binary.right);
			} else {
				print("tmpVar%d ", exp1);
			}
			print(";\n", val);
			t1 = val;
			t2 = t1 + 1;
			if (t2 > maxTmpCount) {
				print("TEMP tmpVar%d;\n", t2);
				maxTmpCount++;
			}

			print("SGE tmpVar%d,", t2);
			if (exp1 == 0) {
				exp1 = genCode(ast->binary.right);
			} else {
				print("tmpVar%d ", exp1);
			}
			print(",");

			if (exp2 == 0) {
				exp2 = genCode(ast->binary.left);
			} else {
				print("tmpVar%d ", exp2);
			}
			print(";\n", val);

			print("ADD tmpVar%d, tmpVar%d, tmpVar%d;\n", t2, t2, t1);
			print("SUB tmpVar%d, tmpVar%d, 2.0;\n", t2, t2);
			print("CMP tmpVar%d, tmpVar%d, -1.0, 1.0;\n", t1, t2);
			tmpCount = t1 + 1;
			return t1;
		} else if (ast->binary.op == NEQ_OP) {
			print("SGE ");
			//
			print("tmpVar%d, ", val);
			if (exp2 == 0) {
				exp2 = genCode(ast->binary.left);
			} else {
				print("tmpVar%d ", exp2);
			}
			print(",");

			if (exp1 == 0) {
				exp1 = genCode(ast->binary.right);
			} else {
				print("tmpVar%d ", exp1);
			}
			print(";\n", val);
			t1 = val;
			t2 = t1 + 1;

			if (t2 > maxTmpCount) {
				print("TEMP tmpVar%d;\n", t2);
				maxTmpCount++;
			}

			print("SGE tmpVar%d,", t2);
			if (exp1 == 0) {
				exp1 = genCode(ast->binary.right);
			} else {
				print("tmpVar%d ", exp1);
			}
			print(",");

			if (exp2 == 0) {
				exp2 = genCode(ast->binary.left);
			} else {
				print("tmpVar%d ", exp2);
			}
			print(";\n", val);

			print("ADD tmpVar%d, tmpVar%d, tmpVar%d;\n", t2, t2, t1);
			print("SUB tmpVar%d, tmpVar%d, 2.0;\n", t2, t2);
			print("CMP tmpVar%d, tmpVar%d, 1.0, -1.0;\n", t1, t2);
			tmpCount = t1 + 1;
			return t1;

		} else if (ast->binary.op == ADD_OP) {
			print("ADD ");
		} else if (ast->binary.op == SUB_OP) {
			print("SUB ");
		} else if (ast->binary.op == MULT_OP) {
			print("MUL ");
		} else if (ast->binary.op == DIV_OP) {
			print("RCP ");
			//
			print("tmpVar%d, ", val);
			if (exp1 == 0) {
				exp1 = genCode(ast->binary.right);
			} else {
				print("tmpVar%d ", exp1);
			}
			print(";\n", val);

			print("MUL tmpVar%d,", val);
			if (exp2 == 0) {
				exp2 = genCode(ast->binary.left);
			} else {
				print("tmpVar%d ", exp2);
			}
			print(",");
			print("tmpVar%d;\n", val);
			//tmpVar%d, tmpVar%d;\n", t1,t1,val);
			tmpCount = val + 1;
			return val;
		} else if (ast->binary.op == POW_OP) {
			print("POW ");
		}

		print("tmpVar%d, ", val);
		if (exp2 == 0) {
			exp2 = genCode(ast->binary.left);
		} else {
			print("tmpVar%d ", exp2);
		}
		print(",");

		if (exp1 == 0) {
			exp1 = genCode(ast->binary.right);
		} else {
			print("tmpVar%d ", exp1);
		}

		//if (exp1 == 0 || exp2 == 0) {
		print(";\n", val);
		//} else {
		//	print("tmpVar%d , tmpVar%d, tmpVar%d;\n", exp2, exp1, tmpCount);
		//}

		return val;

	case INT_NODE:
		//printf("INT_NODE %d\n", kind);
		//printf("Integer: %d\n",ast->int_literal.right);
		//print("MOV tmpVar%d, %d.0;\n", tmpCount, ast->int_literal.right);
		//val = tmpCount++;
		print("%d.0", ast->int_val);
		return 0;

	case FLOAT_NODE:
		//printf("FLOAT_NODE %d\n", kind);
		//printf("Float: %f", ast->float_literal.right);
		//print("MOV tmpVar%d, %d;\n", tmpCount, ast->float_literal.right);
		//val = tmpCount++;

		print("%f", ast->float_val);
		return 0;

	case BOOL_NODE:
		//printf("BOOL_NODE %d\n", kind);
		//printf("Bool: %d", ast->bool_literal.right);
		if (ast->bool_val == 1) {
			//print("MOV tmpVar%d, %f;\n", tmpCount, 1.0);
			print("1.0");
		} else if (ast->bool_val == 0) {
			//print("MOV tmpVar%d, %f;\n", tmpCount, -1.0);
			print("-1.0");
		}
		//val = tmpCount++;
		//return val;
		return 0;

	case VARIABLE_NODE:
		//print("VAR_NODE %d\n", kind);
		return printVar(ast);

	case ARRAY_INDEX_NODE:
		//print("ARRAY_NODE %d\n",kind);
		return printArray(ast);

	case FUNCTION_NODE:
		//printf("FUNCTION_NODE %d\n", kind);
		if (type == -1)
			return -1;
		//type = genCode(ast->function_exp.arguments);

		val = tmpCount++;

		if (val > maxTmpCount) {
			print("TEMP tmpVar%d;\n", val);
			maxTmpCount++;
		}

		if (ast->function.func_code == 2) { //rsq
			print("RSQ tmpVar%d, ", val);
		} else if (ast->function.func_code == 0) { //dp3
			print("DP3 tmpVar%d, ", val);
		} else if (ast->function.func_code == 1) { //lit
			print("LIT tmpVar%d, ", val);
		}

		genCode(ast->function.args);
		print(";\n");
		return val;

	case CONSTRUCTOR_NODE:
		//printf("CONSTRUCTOR_NODE %d\n", kind);
		exp2 = genCode(ast->constructor.type);
		print("{", val);
		exp1 = genCode(ast->constructor.args);
		print("}");
		return 0;

	case TYPE_NODE:
		//printf("TYPE_NODE %d\n", kind);
		return 0; //ast->type.type_name;

	case IF_STATEMENT_NODE:
		//printf("#IF_ELSE_STATEMENT_NODE %d\n", kind);
		val = ++condCount;
		print("TEMP condVar%d;\n", val);

		if (isTempRes(ast->if_statement.cond->kind)) {
			exp2 = genCode(ast->if_statement.cond);
			print("MOV condVar%d, tmpVar%d;\n", val, exp2);
		} else {
			print("MOV condVar%d, ", val);
			genCode(ast->if_statement.cond);
			print(";\n");
		}

		if (ast->if_statement.else_statement) {
			print("#else\n");
			if_state = IN_ELSE;
			s_push(ifStack, &ifStackTop, if_state);
			exp1 = genCode(ast->if_statement.else_statement);
			s_pop(ifStack, &ifStackTop);
		}

		if (ast->if_statement.then_statement) {
			print("#then\n");
			if_state = IN_THEN;
			s_push(ifStack,&ifStackTop,if_state);
			genCode(ast->if_statement.then_statement);
			s_pop(ifStack, &ifStackTop);
		}	

		print("#endif\n");
		if_state = IN_NONE;

		condCount--;

		return 0;

	case ASSIGNMENT_NODE:
		//print("#ASSIGNMENT_NODE %d\n", kind);

		if_state = s_peak(ifStack,&ifStackTop);

		if (if_state == IN_THEN) {
			if (isTempRes(ast->assignment.expression->kind)) {
				exp1 = genCode(ast->assignment.expression);
				print("CMP ");
				exp2 = genCode(ast->assignment.var);
				print(", ");
				print("condVar%d ,", condCount);
				exp2 = genCode(ast->assignment.var);
				print(", tmpVar%d", exp1);
				print(";\n");
			} else {
				print("CMP ");
				exp2 = genCode(ast->assignment.var);
				print(", ");
				print("condVar%d ,", condCount);
				exp2 = genCode(ast->assignment.var);
				print(", ");
				exp1 = genCode(ast->assignment.expression);
				print(";\n");
			}

		} else if (if_state == IN_ELSE) {
			if (isTempRes(ast->assignment.expression->kind)) {
				exp1 = genCode(ast->assignment.expression);
				print("CMP ");
				exp2 = genCode(ast->assignment.var);
				print(", ");
				print("condVar%d ,", condCount);
				print("tmpVar%d ,", exp1);
				exp2 = genCode(ast->assignment.var);
				//double
				print(";\n");
			} else {
				print("CMP ");
				exp2 = genCode(ast->assignment.var);
				print(", ");
				print("condVar%d ,", condCount);
				exp1 = genCode(ast->assignment.expression);
				print(", ");
				exp2 = genCode(ast->assignment.var);
				print(";\n");
			}

		} else {

			if (isTempRes(ast->assignment.expression->kind)) {
				exp1 = genCode(ast->assignment.expression);
				print("MOV ");
				exp2 = genCode(ast->assignment.var);
				print(", ");
				print("tmpVar%d", exp1);
				print(";\n");
			} else {
				print("MOV ");
				exp2 = genCode(ast->assignment.var);
				print(", ");
				exp1 = genCode(ast->assignment.expression);
				print(";\n");
			}
		}

		return 0;

	case NESTED_SCOPE_NODE:
		return genCode(ast->nested_scope);

	case DECLARATION_NODE:
		//printf("DECLARATION_NODE %d\n", kind);
		if(!ast->declaration.expression) {	// Declaration without assignment
			print("TEMP %s;\n", ast->declaration.id);
		}
		else if(ast->declaration.is_const) {	// Constant value declaration
			exp2 = genCode(ast->declaration.type);
			if (isTempRes(ast->declaration.expression->kind)) {
				exp1 = genCode(ast->declaration.expression);
				print("PARAM %s = tmpVar%d;\n", ast->declaration.id, exp1);

			} else {
				print("PARAM %s = ", ast->declaration.id);
				exp1 = genCode(ast->declaration.expression);
				print(";\n");
			}
		}	
		else {
			exp2 = genCode(ast->declaration.type);
			if_state = s_peak(ifStack,&ifStackTop);

			if (if_state == IN_THEN) {
				if (isTempRes(ast->declaration.expression->kind)) {
					exp1 = genCode(ast->declaration.expression);
					print("TEMP %s;\n", ast->declaration.id);
					print("CMP ");
					print("%s ,", ast->declaration.id);
					print("condVar%d ,", condCount);
					print("%s ,", ast->declaration.id);
					print("tmpVar%d ;\n", exp1);

				} else {
					print("TEMP %s;\n", ast->declaration.id);
					print("CMP ");
					print("%s ,", ast->declaration.id);
					print("condVar%d ,", condCount);
					print(" %s ,", ast->declaration.id);
					exp1 = genCode(ast->declaration.expression);
					print(";\n");
				}
			} else if (if_state == IN_ELSE) {
				if (isTempRes(ast->declaration.expression->kind)) {
					exp1 = genCode(ast->declaration.expression);
					print("TEMP %s;\n", ast->declaration.id);
					print("CMP ");
					print("%s ,", ast->declaration.id);
					print("condVar%d ,", condCount);
					print("tmpVar%d ,", exp1);
					print("%s;\n", ast->declaration.id);

				} else {
					print("TEMP %s;\n", ast->declaration.id);
					print("CMP ");
					print("%s ,", ast->declaration.id);
					print("condVar%d ,", condCount);
					exp1 = genCode(ast->declaration.expression);
					print(", %s", ast->declaration.id);
					print(";\n");
				}
			} else {

				if (isTempRes(ast->declaration.expression->kind)) {
					exp1 = genCode(ast->declaration.expression);
					print("TEMP %s;\n", ast->declaration.id);
					print("MOV %s, tmpVar%d;\n", ast->declaration.id, exp1);
				} else {
					print("TEMP %s;\n", ast->declaration.id);
					print("MOV %s, ", ast->declaration.id);
					exp1 = genCode(ast->declaration.expression);
					print(";\n");
				}
			}
		}
		return 0;	

	case ARGUMENTS_NODE:
		//print("#ARGUMENTS_COMMA_NODE %d\n", kind);

		exp1 = genCode(ast->arguments.args);
		print(", ");
		exp2 = genCode(ast->arguments.expression);
		return 0;

	case EXPRESSION_VARIABLE_NODE:
		//printf("ARGUMENTS_EXPRESSION_NODE %d\n", kind);
		return genCode(ast->expression_variable);

	case UNKNOWN:
		errorOccurred = 1; 
		fprintf(errorFile,"UNKNOW ERROR! NUCLEAR MELTDOWN IN 5. 4. 3. 2. 1. 0.!\n",ast->line_num);
		return ERROR;
	default:
		errorOccurred = 1; 
		fprintf(errorFile,"ENCRYPTING HARDDRIVE! PLEASE PAY 1 Million $ to Nigerian Prince to get files back.\n",ast->line_num);
		return ERROR;
	}

	return 0; // failed checks
}
