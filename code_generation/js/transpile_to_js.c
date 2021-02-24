#ifndef GCC_TRANSPILE_TO_JS_C
#define GCC_TRANSPILE_TO_JS_C GCC_TRANSPILE_TO_JS_C
#include <transpile_to_js.h>


void transpile_to_javascript(FILE* out,struct Program *program,struct Command_Arguments *command_arguments)
{
	struct Queue_Node *it;

	if(command_arguments->insert_html)
	{
		fprintf(out,"<DOCTYPE! html> <html> <head> <title>asdf</title> <script>");
	}
	to_js_print_externs(out,program,command_arguments);
	
	for(it=program->translation_units->first;it!=NULL;it=it->prev)
	{
		fprintf(out,"{");
		to_js_print_translation_unit_tree(out,(struct AST_Translation_Unit*)it->data,program);
		fprintf(out,"\n}\n");
	}

	if(command_arguments->insert_html)
	{
		fprintf(out,"</script> </head> <body> </body> </html>");
	}

}

void _to_js_print_externs(void *denoted,void *args)
{
#define ARGS(x) ((struct Command_Arguments*)x)
#define DENOTED(x) ((struct Denoted*)x)
#define DFUNC(x) ((struct Denoted_Function*)x)
#define DOBJ(x) ((struct Denoted_Object*)x)
	if(DENOTED(denoted)->denotation==DT_Function)
	{
		fprintf(ARGS(args)->output_file,"var ");
		print_token(ARGS(args)->output_file,DFUNC(denoted)->id);
		fprintf(ARGS(args)->output_file,"; /*FUNCTION*/ ");
		if(DFUNC(denoted)->body==NULL)
		{
			fprintf(ARGS(args)->output_file,"/*UNDEFINED*/");
		}
		fprintf(ARGS(args)->output_file,"\n");

	}else if(DENOTED(denoted)->denotation==DT_Object)
	{
		fprintf(ARGS(args)->output_file,"var ");
		print_token(ARGS(args)->output_file,DFUNC(denoted)->id);
		fprintf(ARGS(args)->output_file,"; /*VARIABLE*/");
	}

#undef DOBJ
#undef DFUNC
#undef ARGS
#undef DENOTED
}
void to_js_print_externs(FILE* out,struct Program *program,struct Command_Arguments *command_arguments)
{
	fprintf(out,"\n/*EXTERNS START*/\n");
	Map_Map_Extended(
			&((struct Normal_Scope*)program->externs)->ordinary
			,_to_js_print_externs
			,command_arguments);
	fprintf(out,"\n/*EXTERNS END*/\n");
}
void to_js_print_ast(FILE* out,struct AST *tree,struct Program *program)
{
	if(tree==NULL)
	{
		return ;
	}
	switch(tree->type)
	{
		case OP_MEMBER_TROUGH_PTR:
		case OP_MEMBER:
		case OP_BITWISE_AND:
		case OP_BITWISE_XOR:
		case OP_BITWISE_NOT:
		case OP_LOGICAL_AND:
		case OP_LOGICAL_OR:
		case OP_XOR_ASSIGN:
		case OP_PIPE_ASSIGN:
		case OP_SHIFT_RIGHT_ASSIGN:
		case OP_ADD_ASSIGN:
		case OP_SUBTRACT_ASSIGN:
		case OP_MULTIPLY_ASSIGN:
		case OP_REMAINDER_ASSIGN:
		case OP_DIV_ASSIGN:
		case OP_SUBTRACTION:
		case OP_MUL:
		case OP_DIV:
		case OP_REMAINDER:
		case OP_EQUAL:
		case OP_LESS:
		case OP_LESS_EQ:
		case OP_SHIFT_LEFT:
		case OP_BITWISE_OR:
		case OP_AND_ASSIGN:
		case OP_ARR_SUBSCRIPT:
		case OP_SHIFT_LEFT_ASSIGN:
		case OP_ASSIGN:
		case OP_ADDITION:
		case OP_COMMA:
		case OP_SHIFT_RIGHT:
		case OP_GREATER_EQ:
		case OP_GREATER:
		case OP_NOT_EQUAL:
			to_js_print_binary_expression_tree(out,(struct AST_Binary_Expression*)tree,program);
			break;
		case OP_COND:
			to_js_print_conditional_expression_tree(out,(struct AST_Conditional_Expression*)tree,program);
			break;
		case OP_FUNCTION:
			to_js_print_function_expression_tree(out,(struct AST_Function_Expression*)tree,program);
			break;
		case OP_LOGICAL_NOT:
		case OP_UNARY_MINUS:
		case OP_SIZEOF:
		case OP_ADDR_OF:
		case OP_DEREFERENCE:
		case OP_POSTFIX_INC:
		case OP_PREFIX_INC:
		case OP_UNARY_PLUS:
		case OP_POSTFIX_DEC:
		case OP_PREFIX_DEC:
		case OP_CAST:
			to_js_print_unary_expression_tree(out,(struct AST_Unary_Expression*)tree,program);
			break;
		case OP_LVALUE:
			to_js_print_lvalue_expression_tree(out,(struct AST_Lvalue_Expression*)tree,program);
			break;
		case OP_RVALUE:
			to_js_print_rvalue_expression_tree(out,(struct AST_Rvalue_Expression*)tree,program);
			break;
		case OP_NOP:
			fprintf(out,"NOP");
			break;

		case ST_SWITCH:
			to_js_print_switch_statement_tree(out,(struct AST_Switch_Statement*)tree,program);
			break;
		case ST_IF:
			to_js_print_if_statement_tree(out,(struct AST_If_Statement*)tree,program);
			break;
		case ST_WHILE:
			to_js_print_while_statement_tree(out,(struct AST_While_Statement*)tree,program);
			break;
		case ST_DO_WHILE:
			to_js_print_do_while_statement_tree(out,(struct AST_Do_While_Statement*)tree,program);
			break;
		case ST_GOTO:
			to_js_print_goto_statement_tree(out,(struct AST_Goto_Statement*)tree,program);
			break;
		case ST_DEFAULT:
		case ST_LABEL:
		case ST_CASE:
			to_js_print_labeled_statement_tree(out,(struct AST_Labeled_Statement*)tree,program);
			break;
		case ST_CONTINUE:
			fprintf(out,"continue");
			break;
		case ST_BREAK:
			fprintf(out,"break");
			break;
		case ST_RETURN:
			to_js_print_return_statement_tree(out,(struct AST_Return_Statement*)tree,program);
			break;
		case ST_FOR:
			to_js_print_for_statement_tree(out,(struct AST_For_Statement*)tree,program);
			break;
		case ST_COMPOUND:
			to_js_print_compound_statement_tree(out,(struct AST_Compound_Statement*)tree,program);
			break;
		case ST_OBJECT_DECLARATION:
			to_js_print_object_declaration_tree(out,((struct AST_Object_Declaration*)tree),program);
			if(((struct AST_Object_Declaration*)tree)->initializer!=NULL)
			{
			fprintf(out,"=");
			to_js_print_ast(out,((struct AST_Object_Declaration*)tree)->initializer,program);
			}
			break;
		case ST_FUNCTION_DECLARATION:
			to_js_print_function_declaration_tree(out,((struct AST_Function_Declaration*)tree),program);
			break;
		case ST_FUNCTION_DEFINITION:
			to_js_print_function_definition(out,((struct AST_Function_Definition*)tree),program);
			break;
		case TRANSLATION_UNIT:
			to_js_print_translation_unit_tree(out,(struct AST_Translation_Unit*)tree,program);
			break;
		case ERROR:
		default:
			/*TODO error*/
			return;
	}
}
void to_js_print_translation_unit_tree(FILE* out,struct AST_Translation_Unit *translation_unit,struct Program *program)
{
	struct Queue_Node *it;	
	struct AST* hold;
	for(it=translation_unit->components.first;it!=NULL;it=it->prev)
	{
		hold=(struct AST*)(it->data);
		to_js_print_ast(out,hold,program);

		fprintf(out,";\n");
	}
}
void to_js_print_binary_expression_tree(FILE* out,struct AST_Binary_Expression *bin,struct Program *program)
{
	if(bin->type==OP_ARR_SUBSCRIPT)
	{
		to_js_print_ast(out,bin->left,program);
		fprintf(out,"[");
		to_js_print_ast(out,bin->right,program);
		fprintf(out,"]");
	}else
	{
		fprintf(out,"(");
		to_js_print_ast(out,bin->left,program);
		print_ast_enum(out,bin->type);
		to_js_print_ast(out,bin->right,program);
		fprintf(out,")");
	}
}

void to_js_print_conditional_expression_tree(FILE* out,struct AST_Conditional_Expression *cond,struct Program *program)
{
	fprintf(out,"(");
	to_js_print_ast(out,cond->left,program);
	fprintf(out,"?");
	to_js_print_ast(out,cond->center,program);
	fprintf(out,":");
	to_js_print_ast(out,cond->right,program);
	fprintf(out,")");
}
void to_js_print_function_expression_tree(FILE* out,struct AST_Function_Expression *function_call,struct Program *program)
{
	struct Queue_Node *it;
	to_js_print_ast(out,function_call->id,program);	
	fprintf(out,"(");
	if(function_call->arguments.size>0)
	{
		fprintf(out,"\n");
		for(it=function_call->arguments.first;it!=function_call->arguments.last;it=it->prev)
		{
			to_js_print_ast(out,(struct AST*)(it->data),program);
			fprintf(out,",\n");
		}
		if(it!=NULL)
		{
			to_js_print_ast(out,(struct AST*)(it->data),program);
		}

	}
	fprintf(out,")");
}
void to_js_print_rvalue_expression_tree(FILE* out,struct AST_Rvalue_Expression *rval,struct Program *program)
{
	print_token(out,rval->id);
}
void to_js_print_lvalue_expression_tree(FILE* out,struct AST_Lvalue_Expression *lval,struct Program *program)
{
	print_token(out,lval->id);
}
void to_js_print_unary_expression_tree(FILE* out,struct AST_Unary_Expression *unary,struct Program *program)
{
	print_ast_enum(out,unary->type);
	to_js_print_ast(out,unary->operand,program);
}
void to_js_print_labeled_statement_tree(FILE* out,struct AST_Labeled_Statement *label,struct Program *program)
{
	if(label->type!=ST_LABEL)
		print_ast_enum(out,label->type);
	if(label->label!=NULL)
	{
		fprintf(out,"case");
		print_token(out,label->label);
	}
	fprintf(out,":\n");
	to_js_print_ast(out,label->statement,program);
}
void to_js_print_compound_statement_tree(FILE* out,struct AST_Compound_Statement *compound,struct Program *program)
{
	struct Queue_Node *it;
	fprintf(out,"{");
	for(it=compound->components.first;it!=NULL;it=it->prev)
	{
		fprintf(out,"\n");
		to_js_print_ast(out,(struct AST*)it->data,program);
		fprintf(out,";");
	}
	fprintf(out,"\n}");
}
void to_js_print_for_statement_tree(FILE* out,struct AST_For_Statement *for_statement,struct Program *program)
{
	fprintf(out,"for(\n");
	if(for_statement->initialisation!=NULL)
		to_js_print_ast(out,for_statement->initialisation,program);
	fprintf(out,";\n");
	if(for_statement->condition!=NULL)
		to_js_print_ast(out,for_statement->condition,program);
	fprintf(out,";\n");
	if(for_statement->update!=NULL)
		to_js_print_ast(out,for_statement->update,program);
	fprintf(out,")\n");
	print_ast(out,for_statement->body_statement);
}
void to_js_print_while_statement_tree(FILE* out,struct AST_While_Statement *while_statement,struct Program *program)
{
	fprintf(out,"while(");
	to_js_print_ast(out,while_statement->condition,program);
	fprintf(out,")\n");
	to_js_print_ast(out,while_statement->body_statement,program);
}
void to_js_print_do_while_statement_tree(FILE* out,struct AST_Do_While_Statement *do_while_statement,struct Program *program)
{
	fprintf(out,"do\n");
	to_js_print_ast(out,do_while_statement->body_statement,program);
	fprintf(out,"while(");
	to_js_print_ast(out,do_while_statement->condition,program);
	fprintf(out,")\n");
}
void to_js_print_if_statement_tree(FILE* out,struct AST_If_Statement *if_statement,struct Program *program)
{
	fprintf(out,"if(");
	to_js_print_ast(out,if_statement->condition,program);
	fprintf(out,")\n");
	to_js_print_ast(out,if_statement->body_statement,program);

	if(if_statement->else_statement!=NULL)
	{
		fprintf(out,"\nelse");
		to_js_print_ast(out,if_statement->else_statement,program);
	}
}
void to_js_print_goto_statement_tree(FILE* out,struct AST_Goto_Statement *goto_statement,struct Program *program)
{
	/*TODO something here, be it error or not*/
}
void to_js_print_switch_statement_tree(FILE* out,struct AST_Switch_Statement *switch_statement,struct Program *program)
{
	fprintf(out,"switch(");
	to_js_print_ast(out,switch_statement->condition,program);
	fprintf(out,")\n");
	to_js_print_ast(out,switch_statement->body_statement,program);
}
void to_js_print_return_statement_tree(FILE* out,struct AST_Return_Statement *return_statement,struct Program *program)
{
	fprintf(out,"return ");
	to_js_print_ast(out,return_statement->return_expression,program);
}
void to_js_print_object_declaration_tree(FILE* out,struct AST_Object_Declaration *object_declaration,struct Program *program)
{
	if(object_declaration->object->object->storage_class!=SC_EXTERN)
	{
		fprintf(out,"let ");
		print_token(out,object_declaration->object->id);
	}

}
void to_js_print_function_definition(FILE* out,struct AST_Function_Definition *function_definition,struct Program *program)
{
	size_t i;
	struct Type_Function *cache_type;

	cache_type=(struct Type_Function*)function_definition->function->type;

	if((struct Type_Function*)function_definition->function->storage_class==SC_EXTERN)
	{
	//	fprintf(out,"var ");
		print_token(out,function_definition->function->id);
		fprintf(out,"= function");
	}else
	{
		fprintf(out,"function ");
		print_token(out,function_definition->function->id);
	}

	/*print parameters*/
	fprintf(out,"(");
	print_token(out,cache_type->arguments[0]->id);
	for(i=1;i<cache_type->number_of_arguments;++i)
	{
		fprintf(out,",");
		print_token(out,cache_type->arguments[i]->id);
	}

	fprintf(out,")");

	/*print body*/
	to_js_print_compound_statement_tree(out,function_definition->function->body,program);


}
void to_js_print_function_declaration_tree(FILE* out,struct AST_Function_Declaration *function_declaration,struct Program *program)
{
	/*TODO probably leave it empty*/
}



#endif
