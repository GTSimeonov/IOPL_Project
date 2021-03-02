#ifndef GCC_PRINT
#define GCC_PRINT GCC_PRINT
#include "print.h"




int indent=0;

void print_token(FILE *out,struct token *token)
{
	size_t i;
//	fprintf(out,"[");
	if(token==NULL)return;
	for(i=0;i<token->data_size;++i)
	{
		fprintf(out,"%c",token->data[i]);
	}
//	fprintf(out,"]");
}

char print_tokens_of_program(FILE *out,char **base_source_names)
{

	struct Source_File *base_file;
	struct Translation_Data *hold_translation_data;
	char *this_directory[]={"./",NULL};
	char ret;

	assert(base_source_names!=NULL);

	if(*base_source_names==NULL)
	{
		return 0;
	}
	ret=0;

	hold_translation_data=get_translation_data(NULL,get_linkage(),get_linkage());	
	do
	{
		base_file=get_source_file(*base_source_names,this_directory);

		if(base_file==NULL)
		{
			/*TODO error*/
			continue;
		}else
		{
			lex(base_file,hold_translation_data);
			if(hold_translation_data->errors->size>0)
			{
				ret=1;
				/*if we are here then the is_quiet flag has not been set*/
				print_errors(out,hold_translation_data->errors);
				delete_source_file(base_file);
				break;
			}
			fprintf(out,"\nTOKENS OF %s {\n",base_file->src_name->filename);
			print_tokens(out,hold_translation_data->tokens);
			fprintf(out,"\n} END OF TOKENS\n");


		}
	}while(*(++base_source_names));
	
	/*TODO fix memory leak*/
	free(hold_translation_data);
	

	return ret;
}
void print_tokens(FILE *out,struct Queue *tokens)
{
	struct Queue_Node *it;
	for( it=tokens->first; it!=NULL; it=it->prev)
	{
			fprintf(out,"[");
			print_keyword_enum(out,((struct token*)(it->data))->type);
			print_token(out,(struct token*)(it->data));
			fprintf(out,"] ");
	}
}

void print_ast_enum(FILE *out,enum AST_Type op)
{
	switch(op)
	{
	case OP_COMMA:
		fprintf(out,",");break;
	case OP_ADDITION:
		fprintf(out,"+");break;
	case OP_SUBTRACTION:
		fprintf(out,"-");break;
	case OP_MUL:
		fprintf(out,"*");break;
	case OP_DIV:
		fprintf(out,"/");break;
	case OP_REMAINDER:
		fprintf(out,"%");break;
	case OP_COND:
		fprintf(out,"CONDITIONAL");break;
	case OP_FUNCTION:
		fprintf(out,"FUNCTION_CALL");break;
	case OP_ASSIGN:
		fprintf(out,"=");break;
	case OP_ADD_ASSIGN:
		fprintf(out,"+=");break;
	case OP_SUBTRACT_ASSIGN:
		fprintf(out,"-=");break;
	case OP_MULTIPLY_ASSIGN:
		fprintf(out,"*=");break;
	case OP_REMAINDER_ASSIGN:
		fprintf(out,"%=");break;
	case OP_DIV_ASSIGN:
		fprintf(out,"/=");break;
	case OP_SHIFT_LEFT_ASSIGN:
		fprintf(out,">>=");break;
	case OP_SHIFT_RIGHT_ASSIGN:
		fprintf(out,"<<=");break;
	case OP_AND_ASSIGN:
		fprintf(out,"&=");break;
	case OP_XOR_ASSIGN:
		fprintf(out,"^=");break;
	case OP_PIPE_ASSIGN:
		fprintf(out,"|=");break;
	case OP_NOP:
		fprintf(out,"NOP");break;
	case OP_LOGICAL_OR:
		fprintf(out,"||");break;
	case OP_LOGICAL_AND:
		fprintf(out,"&&");break;
	case OP_LOGICAL_NOT:
		fprintf(out,"!");break;
	case OP_BITWISE_OR:
		fprintf(out,"|");break;
	case OP_BITWISE_AND:
		fprintf(out,"&");break;
	case OP_BITWISE_XOR:
		fprintf(out,"^");break;
	case OP_BITWISE_NOT:
		fprintf(out,"~");break;
	case OP_ADDR_OF:
		fprintf(out,"&");break;
	case OP_DEREFERENCE:
		fprintf(out,"*");break;
	case OP_MEMBER_TROUGH_PTR:
		fprintf(out,"->");break;
	case OP_MEMBER:
		fprintf(out,".");break;
	case OP_ARR_SUBSCRIPT:
		fprintf(out,"ARR_SUBSCRIPT");break;
	case OP_POSTFIX_INC:
		fprintf(out,"++");break;
	case OP_POSTFIX_DEC:
		fprintf(out,"--");break;
	case OP_PREFIX_INC:
		fprintf(out,"++");break;
	case OP_PREFIX_DEC:
		fprintf(out,"--");break;
	case OP_UNARY_PLUS:
		fprintf(out,"+");break;
	case OP_UNARY_MINUS:
		fprintf(out,"-");break;
	case OP_CAST:
		fprintf(out,"CAST");break;
	case OP_SIZEOF:
		fprintf(out,"sizeof");break;
	case OP_SHIFT_LEFT:
		fprintf(out,"<<");break;
	case OP_SHIFT_RIGHT:
		fprintf(out,">>");break;
	case OP_LESS_EQ:
		fprintf(out,"<=");break;
	case OP_GREATER_EQ:
		fprintf(out,">=");break;
	case OP_LESS:
		fprintf(out,"<");break;
	case OP_GREATER:
		fprintf(out,">");break;
	case OP_EQUAL:
		fprintf(out,"==");break;
	case OP_NOT_EQUAL:
		fprintf(out,"!=");break;
	case OP_LVALUE:
		fprintf(out,"LVALUE");break;
	case OP_RVALUE:
		fprintf(out,"RVALUE");break;
	case ST_COMPOUND:
		fprintf(out,"COMPOUND");break;
	case ST_EXPRESSION:
		fprintf(out,"EXPRESSION");break;
	case ST_SWITCH:
		fprintf(out,"switch");break;
	case ST_IF:
		fprintf(out,"if");break;
	case ST_WHILE:
		fprintf(out,"while");break;
	case ST_DO_WHILE:
		fprintf(out,"do_while");break;
	case ST_GOTO:
		fprintf(out,"goto");break;
	case ST_LABEL:
		fprintf(out,"LABEL");break;
	case ST_CASE:
		fprintf(out,"case");break;
	case ST_DEFAULT:
		fprintf(out,"default");break;
	case ST_CONTINUE:
		fprintf(out,"continue");break;
	case ST_BREAK:
		fprintf(out,"break");break;
	case ST_RETURN:
		fprintf(out,"return");break;
	case ST_FOR:
		fprintf(out,"for");break;
	
		/*TODO obj dec obj def func decl*/
	case ST_FUNCTION_DEFINITION:
		fprintf(out,"FUNCTION_DEFINITION");break;
	case TRANSLATION_UNIT:
		fprintf(out,"TRANSLATION_UNIT");break;
	case ERROR:
		fprintf(out,"ERROR");break;
		default:
			fprintf(out,"NOT_POSSIBLE");break;
	}
}

void print_error_tree(FILE *out,struct AST_Error *error)
{
	fprintf(out,"ERROR");
	if(error->error!=NULL)
	{
		print_ast(out,error->error);
	}
}
void print_binary_expression_tree(FILE *out,struct AST_Binary_Expression *bin)
{
	if(bin->type==OP_ARR_SUBSCRIPT)
	{
		print_ast(out,bin->left);
		fprintf(out,"[");
		print_ast(out,bin->right);
		fprintf(out,"]");
	}else
	{
		fprintf(out,"(");
		print_ast(out,bin->left);
		print_ast_enum(out,bin->type);
		print_ast(out,bin->right);
		fprintf(out,")");
	}
}
void print_conditional_expression_tree(FILE *out,struct AST_Conditional_Expression *cond)
{
	fprintf(out,"(");
	print_ast(out,cond->left);
	fprintf(out,"?");
	print_ast(out,cond->center);
	fprintf(out,":");
	print_ast(out,cond->right);
	fprintf(out,")");
}
void print_function_expression_tree(FILE *out,struct AST_Function_Expression *function_call)
{
	struct Queue_Node *it;
	print_ast(out,function_call->id);	
	fprintf(out,"(");
	if(function_call->arguments.size>0)
	{
		fprintf(out,"\n");
		for(it=function_call->arguments.first;it!=function_call->arguments.last;it=it->prev)
		{
			print_ast(out,(struct AST*)(it->data));
			fprintf(out,",\n");
		}
		if(it!=NULL)
		{
			print_ast(out,(struct AST*)(it->data));
		}

	}
	fprintf(out,")");
}
void print_unary_expression_tree(FILE *out,struct AST_Unary_Expression *unary_expression)
{
	print_ast_enum(out,unary_expression->type);
	if(unary_expression->type==OP_CAST)
	{
		fprintf(out,"(");
		print_type(out,unary_expression->value_type,1);
		fprintf(out,")");
	}
	print_ast(out,unary_expression->operand);
}
void print_rvalue_expression_tree(FILE *out,struct AST_Rvalue_Expression *rval)
{
	print_token(out,rval->id);
}
void print_lvalue_expression_tree(FILE *out,struct AST_Lvalue_Expression *lval)
{
	print_token(out,lval->id);
}
void print_labeled_statement_tree(FILE *out,struct AST_Labeled_Statement *lab)
{
	if(lab->type!=ST_LABEL)
		print_ast_enum(out,lab->type);
	if(lab->label!=NULL)
		print_token(out,lab->label);
	fprintf(out,":\n");
	print_ast(out,lab->statement);
}
void print_compound_statement_tree(FILE *out,struct AST_Compound_Statement *comp)
{
	struct Queue_Node *it;
	fprintf(out,"{\n");
		for(it=comp->components.first;it!=NULL;it=it->prev)
		{
			print_ast(out,(struct AST*)(it->data));
			fprintf(out,";\n");
		}
	fprintf(out,"}\n");
}
void print_if_statement_tree(FILE *out,struct AST_If_Statement *ifs)
{
	fprintf(out,"if(");
	print_ast(out,ifs->condition);
	fprintf(out,")\n");
	print_ast(out,ifs->body_statement);

	if(ifs->else_statement!=NULL)
	{
		fprintf(out,"\nelse");
		print_ast(out,ifs->else_statement);
	}
	
}
void print_switch_statement_tree(FILE *out,struct AST_Switch_Statement *swi)
{
	fprintf(out,"switch(");
	print_ast(out,swi->condition);
	fprintf(out,")\n");
	print_ast(out,swi->body_statement);
}
void print_while_statement_tree(FILE *out,struct AST_While_Statement *whi)
{
	fprintf(out,"while(");
	print_ast(out,whi->condition);
	fprintf(out,")\n");
	print_ast(out,whi->body_statement);
}
void print_do_while_statement_tree(FILE *out,struct AST_Do_While_Statement *whi)
{
	fprintf(out,"do\n");
	print_ast(out,whi->body_statement);
	fprintf(out,"while(");
	print_ast(out,whi->condition);
	fprintf(out,")\n");
}
void print_for_statement_tree(FILE *out,struct AST_For_Statement *fo)
{
	fprintf(out,"for(\n");
	print_ast(out,fo->initialisation);
	fprintf(out,";\n");
	print_ast(out,fo->condition);
	fprintf(out,";\n");
	print_ast(out,fo->update);
	fprintf(out,")\n");
	print_ast(out,fo->body_statement);
}
void print_return_statement_tree(FILE *out,struct AST_Return_Statement *return_expression)
{
	fprintf(out,"return ");
	print_ast(out,return_expression->return_expression);
}
void print_goto_statement_tree(FILE *out,struct AST_Goto_Statement *got)
{
	fprintf(out,"goto ");
	print_token(out,got->label);
}

void print_type(FILE *out,struct Type *type,char should_print_struct_union)
{
	print_type_qualifier(out,type);
	switch(type->specifier)
	{
		case TS_VOID:
			fprintf(out,"void");return;
		case TS_CHAR:
			fprintf(out,"char");return;
		case TS_INT:
			fprintf(out,"int");return;
		case TS_FLOAT:
			fprintf(out,"float");return;
		case TS_DOUBLE:
			fprintf(out,"double");return;
		case TS_UNION:
		case TS_STRUCT:
			if(should_print_struct_union)
			{
				print_struct_union(out,((struct Type_Struct_Union*)type)->struct_union);
			}else
			{
				fprintf(out,(type->specifier==TS_STRUCT?"struct":"union"));
			}
			return;
		case TS_ENUM:
			print_enumeration(out,((struct Type_Enum*)type)->enumeration);
			return;
		case TS_POINTER:
			fprintf(out,"pointer to ");
			print_type(out,((struct Type_Pointer*)type)->points_to,0);
			return;
		case TS_ARRAY:
			fprintf(out,"array [%zu] of ",((struct Type_Array*)type)->number_of_elements);
			print_type(out,((struct Type_Array*)type)->is_array_of,should_print_struct_union);
			return;
		case TS_FUNC:
			fprintf(out,"function taking arguments (");
			print_function_args(out,(struct Type_Function*)type);
			fprintf(out,") returning ");
			print_type(out,((struct Type_Function*)type)->return_type,should_print_struct_union);
			return;
		case TS_BITFIELD:
			fprintf(out,"%zu bits of ",((struct Type_Bit_Field*)type)->number_of_bits);
			print_type(out,((struct Type_Bit_Field*)type)->base,should_print_struct_union);
			return;
		case TS_NONE:
			fprintf(out,"NONE");return;
		case TS_ERROR:
			fprintf(out,"ERROR!");return;

	}
	assert(!"reached end of switch");
}
void print_denoted(FILE *out,struct Denoted *denoted)
{

	switch(denoted->denotation)
	{
		case DT_Macro:
			fprintf(out,"macro ");return;
		case DT_Macro_Parameter:
			fprintf(out,"macro parameter ");return;
		case DT_Label:
			fprintf(out,"label ");return;
		case DT_Object:
			switch(((struct Denoted_Object*)denoted)->linkage)
			{
				case LINKAGE_INTERNAL:
					fprintf(out,"internally linked ");
					break;
				case LINKAGE_EXTERNAL:
					fprintf(out,"externally linked ");
					break;
				case LINKAGE_NONE:
					break;
				default:
					assert(0);
			}
			fprintf(out,"denoted object ");
			print_token(out,((struct Denoted_Object*)denoted)->id);
			switch(((struct Denoted_Object*)denoted)->object->storage_class)
			{
				case SCS_NONE:
					fprintf(out," with automatic storage duration");
					break;
				case SCS_STATIC:
					fprintf(out," static storage duration");
					break;
				assert(0);
			}
			fprintf(out," is a ");
			print_type(out,((struct Denoted_Object*)denoted)->object->type,1);

			return;
		case DT_Typedef:
			fprintf(out,"typedef ");
			print_token(out,((struct Denoted_Typedef*)denoted)->id);	
			fprintf(out," to ");
			print_type(out,((struct Denoted_Typedef*)denoted)->type,0);	
			return;
		case DT_Function:
			print_token(out,((struct Denoted_Function*)denoted)->id);
			fprintf(out," is ");
			switch(((struct Denoted_Function*)denoted)->linkage)
			{
				case LINKAGE_EXTERNAL:
					fprintf(out," an externally linked ");
					break;
				case LINKAGE_INTERNAL:
					fprintf(out," an internally linked ");
					break;
				default:
					assert(0);
			}
			print_type(out,((struct Denoted_Function*)denoted)->type,1);
			return;
		case DT_Enum:
			print_token(out,((struct Denoted_Enum*)denoted)->id);
			fprintf(out," is ");
			print_enumeration(out,((struct Denoted_Enum*)denoted)->enumeration);
			return;
		case DT_Enum_Constant:
			fprintf(out,"%i ",((struct Denoted_Enum_Const*)denoted)->value);
			return;
		case DT_Struct_Union_Tag:
			print_token(out,((struct Denoted_Struct_Union*)denoted)->id);
			fprintf(out," is ");
			print_struct_union(out,((struct Denoted_Struct_Union*)denoted)->struct_union);
		case DT_Error:
			fprintf(out,"denotation error");return;
		case DT_Prototype:
			fprintf(out,"denotation prototyep");return;
		default:
			assert(0);
		
	}
}
void print_list_of_denoted(FILE *out,struct Queue *denoted)
{
	struct Queue_Node *it;
	for(it=denoted->first;it!=NULL;it=it->prev)
	{
		print_denoted(out,(struct Denoted*)it->data);
		if(it->prev!=NULL)
			fprintf(out,",");
	}
}
void print_enumeration(FILE *out,struct Enum *enumeration)
{
	fprintf(out,"enum ");
	print_list_of_denoted(out,enumeration->consts);
}
void print_struct_union(FILE *out,struct Struct_Union *struct_union)
{
	switch(struct_union->specifier)
	{
		case TS_UNION:
			fprintf(out,"union ");
			break;
		case TS_STRUCT:
			fprintf(out,"struct ");
			break;
		default:
			assert(1==0);
	}
	fprintf(out,"{");
	print_list_of_denoted(out,struct_union->members);
	fprintf(out,"}");

}
void print_translation_unit_tree(FILE *out,struct AST_Translation_Unit *unit)
{
	struct Queue_Node *it;	
	struct AST* hold;
	for(it=unit->components.first;it!=NULL;it=it->prev)
	{
		hold=(struct AST*)(it->data);
		print_ast(out,hold);
		if(hold->type!=ST_FUNCTION_DEFINITION)
		{
			fprintf(out,";\n");
		}
	}
}
void print_ast(FILE *out,struct AST* tree)
{
	if(tree==NULL)
	{
		fprintf(out,"NULL");
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
			print_binary_expression_tree(out,(struct AST_Binary_Expression*)tree);
			break;
		case OP_COND:
			print_conditional_expression_tree(out,(struct AST_Conditional_Expression*)tree);
			break;
		case OP_FUNCTION:
			print_function_expression_tree(out,(struct AST_Function_Expression*)tree);
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
			print_unary_expression_tree(out,(struct AST_Unary_Expression*)tree);
			break;
		case OP_LVALUE:
			print_lvalue_expression_tree(out,(struct AST_Lvalue_Expression*)tree);
			break;
		case OP_RVALUE:
			print_rvalue_expression_tree(out,(struct AST_Rvalue_Expression*)tree);
			break;
		case OP_NOP:
			fprintf(out,"NOP");
			break;

		case ST_SWITCH:
			print_switch_statement_tree(out,(struct AST_Switch_Statement*)tree);
			break;
		case ST_IF:
			print_if_statement_tree(out,(struct AST_If_Statement*)tree);
			break;
		case ST_WHILE:
			print_while_statement_tree(out,(struct AST_While_Statement*)tree);
			break;
		case ST_DO_WHILE:
			print_do_while_statement_tree(out,(struct AST_Do_While_Statement*)tree);
			break;
		case ST_GOTO:
			print_goto_statement_tree(out,(struct AST_Goto_Statement*)tree);
			break;
		case ST_DEFAULT:
		case ST_LABEL:
		case ST_CASE:
			print_labeled_statement_tree(out,(struct AST_Labeled_Statement*)tree);
			break;
		case ST_CONTINUE:
			fprintf(out,"continue");
			break;
		case ST_BREAK:
			fprintf(out,"break");
			break;
		case ST_RETURN:
			print_return_statement_tree(out,(struct AST_Return_Statement*)tree);
			break;
		case ST_FOR:
			print_for_statement_tree(out,(struct AST_For_Statement*)tree);
			break;
		case ST_COMPOUND:
			print_compound_statement_tree(out,(struct AST_Compound_Statement*)tree);
			break;
		case ST_OBJECT_DECLARATION:
			print_denoted(out,(struct Denoted*)((struct AST_Object_Declaration*)tree)->object);
			fprintf(out,"=");
			print_ast(out,((struct AST_Object_Declaration*)tree)->object->initializer);
			break;
		case ST_TYPE_DEFINITION:
			print_denoted(out,(struct Denoted*)((struct AST_Type_Definition*)tree)->definition);
			break;
		case ST_FUNCTION_DECLARATION:
			print_denoted(out,(struct Denoted*)((struct AST_Function_Declaration*)tree)->function);
			break;
		case ST_FUNCTION_DEFINITION:
			print_function_definition(out,((struct AST_Function_Declaration*)tree)->function);
			break;
		case TRANSLATION_UNIT:
			print_translation_unit_tree(out,(struct AST_Translation_Unit*)tree);
			break;
		case ERROR:
			print_error_tree(out,(struct AST_Error*)tree);
			break;
		default:
			fprintf(out,"NOT_POSSIBLE");break;
	}

}

void print_function_definition(FILE *out,struct Denoted_Function *function)
{
	print_token(out,function->id);
	fprintf(out," is");
	switch(function->linkage)
	{
		case LINKAGE_EXTERNAL:
			fprintf(out," an externally linked ");
			break;
		case LINKAGE_INTERNAL:
			fprintf(out," an internally linked ");
			break;
		default:
			assert(0);
	}
	print_type(out,function->type,1);
	print_ast(out,(struct AST*)function->body);
}
void print_program_ast(FILE *out,struct Program *program)
{
	size_t i;
	struct Queue_Node *it;
	for(it=program->translation_units->first;it!=NULL;it=it->prev)
	{
		fprintf(out,"TRANSLATION_UNIT {\n");
		print_ast(out,(struct AST*)it->data);
		fprintf(out,"\n} TRANSLATION_UNIT_END\n");
	}
}
void print_keyword_enum(FILE *out,enum KEYWORDS kw)
{
	switch(kw)
	{
	case  KW_AUTO :
		fprintf(out," KW_AUTO ");break;
	case  KW_DO :
		fprintf(out," KW_DO ");break;
	case  KW_DOUBLE :
		fprintf(out," KW_DOUBLE ");break;
	case  KW_INT :
		fprintf(out," KW_INT ");break;
	case  KW_STRUCT :
		fprintf(out," KW_STRUCT ");break;
	case  KW_BREAK :
		fprintf(out," KW_BREAK ");break;
	case  KW_ELSE :
		fprintf(out," KW_ELSE ");break;
	case  KW_LONG :
		fprintf(out," KW_LONG ");break;
	case  KW_SWITCH :
		fprintf(out," KW_SWITCH ");break;
	case  KW_CASE :
		fprintf(out," KW_CASE ");break;
	case  KW_ENUM :
		fprintf(out," KW_ENUM ");break;
	case  KW_REGISTER :
		fprintf(out," KW_REGISTER ");break;
	case  KW_TYPEDEF :
		fprintf(out," KW_TYPEDEF ");break;
	case  KW_CHAR :
		fprintf(out," KW_CHAR ");break;
	case  KW_EXTERN :
		fprintf(out," KW_EXTERN ");break;
	case  KW_RETURN :
		fprintf(out," KW_RETURN ");break;
	case  KW_UNION :
		fprintf(out," KW_UNION ");break;
	case  KW_CONST :
		fprintf(out," KW_CONST ");break;
	case  KW_FLOAT :
		fprintf(out," KW_FLOAT ");break;
	case  KW_SHORT :
		fprintf(out," KW_SHORT ");break;
	case  KW_UNSIGNED :
		fprintf(out," KW_UNSIGNED ");break;
	case  KW_CONTINUE :
		fprintf(out," KW_CONTINUE ");break;
	case  KW_FOR :
		fprintf(out," KW_FOR ");break;
	case  KW_SIGNED :
		fprintf(out," KW_SIGNED ");break;
	case  KW_VOID :
		fprintf(out," KW_VOID ");break;
	case  KW_DEFAULT :
		fprintf(out," KW_DEFAULT ");break;
	case  KW_GOTO :
		fprintf(out," KW_GOTO ");break;
	case  KW_SIZEOF :
		fprintf(out," KW_SIZEOF ");break;
	case  KW_VOLATILE :
		fprintf(out," KW_VOLATILE ");break;
	case  KW_IF :
		fprintf(out," KW_IF ");break;
	case  KW_STATIC :
		fprintf(out," KW_STATIC ");break;
	case  KW_WHILE :
		fprintf(out," KW_WHILE ");break;
	case  KW_EXCLAMATION :
		fprintf(out," KW_EXCLAMATION ");break;
	case  KW_BACK_SLASH :
		fprintf(out," KW_BACK_SLASH ");break;
	case  KW_PERCENT :
		fprintf(out," KW_PERCENT ");break;
	case  KW_AND :
		fprintf(out," KW_AND ");break;
	case  KW_AND_AND :
		fprintf(out," KW_AND_AND ");break;
	case  KW_QUOTE :
		fprintf(out," KW_QUOTE ");break;
	case  KW_OPEN_NORMAL :
		fprintf(out," KW_OPEN_NORMAL ");break;
	case  KW_CLOSE_NORMAL :
		fprintf(out," KW_CLOSE_NORMAL ");break;
	case  KW_STAR :
		fprintf(out," KW_STAR ");break;
	case  KW_PLUS :
		fprintf(out," KW_PLUS ");break;
	case  KW_COMMA :
		fprintf(out," KW_COMMA ");break;
	case  KW_MINUS :
		fprintf(out," KW_MINUS ");break;
	case  KW_DOT :
		fprintf(out," KW_DOT ");break;
	case  KW_ARROW :
		fprintf(out," KW_ARROW ");break;
	case  KW_COLUMN :
		fprintf(out," KW_COLUMN ");break;
	case  KW_SEMI_COLUMN :
		fprintf(out," KW_SEMI_COLUMN ");break;
	case  KW_LESS :
		fprintf(out," KW_LESS ");break;
	case  KW_EQ :
		fprintf(out," KW_EQ ");break;
	case  KW_EQEQ :
		fprintf(out," KW_EQEQ ");break;
	case  KW_MORE :
		fprintf(out," KW_MORE ");break;
	case  KW_QUESTION :
		fprintf(out," KW_QUESTION ");break;
	case  KW_OPEN_SQUARE :
		fprintf(out," KW_OPEN_SQUARE ");break;
	case  KW_CLOSE_SQUARE :
		fprintf(out," KW_CLOSE_SQUARE ");break;
	case  KW_HAT :
		fprintf(out," KW_HAT ");break;
	case  KW_FLOOR :
		fprintf(out," KW_FLOOR ");break;
	case  KW_OPEN_CURLY :
		fprintf(out," KW_OPEN_CURLY ");break;
	case  KW_CLOSE_CURLY :
		fprintf(out," KW_CLOSE_CURLY ");break;
	case  KW_PIPE :
		fprintf(out," KW_PIPE ");break;
	case  KW_PIPE_PIPE :
		fprintf(out," KW_PIPE_PIPE ");break;
	case  KW_TILDE :
		fprintf(out," KW_TILDE ");break;
	case  KW_PLUSPLUS :
		fprintf(out," KW_PLUSPLUS ");break;
	case  KW_MINUSMINUS :
		fprintf(out," KW_MINUSMINUS ");break;
	case  KW_SHIFT_RIGHT :
		fprintf(out," KW_SHIFT_RIGHT ");break;
	case  KW_SHIFT_LEFT :
		fprintf(out," KW_SHIFT_LEFT ");break;
	case  KW_LESS_EQ :
		fprintf(out," KW_LESS_EQ ");break;
	case  KW_MORE_EQ :
		fprintf(out," KW_MORE_EQ ");break;
	case  KW_NOT_EQ :
		fprintf(out," KW_NOT_EQ ");break;
	case  KW_PLUS_EQ :
		fprintf(out," KW_PLUS_EQ ");break;
	case  KW_MINUS_EQ :
		fprintf(out," KW_MINUS_EQ ");break;
	case  KW_STAR_EQ :
		fprintf(out," KW_STAR_EQ ");break;
	case  KW_PERCENT_EQ :
		fprintf(out," KW_PERCENT_EQ ");break;
	case  KW_SHIFT_LEFT_EQ :
		fprintf(out," KW_SHIFT_LEFT_EQ ");break;
	case  KW_SHIFT_RIGHT_EQ :
		fprintf(out," KW_SHIFT_RIGHT_EQ ");break;
	case  KW_AND_EQ :
		fprintf(out," KW_AND_EQ ");break;
	case  KW_HAT_EQ :
		fprintf(out," KW_HAT_EQ ");break;
	case  KW_PIPE_EQ :
		fprintf(out," KW_PIPE_EQ ");break;
	case  KW_DIV_EQ :
		fprintf(out," KW_DIV_EQ ");break;
	case  KW_FORWARD_SLASH :
		fprintf(out," KW_FORWARD_SLASH ");break;
	case  KW_NOTYPE :
		fprintf(out," KW_NOTYPE ");break;
	case  KW_NUMBER :
		fprintf(out," KW_NUMBER ");break;
	case  KW_COMMENT :
		fprintf(out," KW_COMMENT ");break;
	case  KW_ID :
		fprintf(out," KW_ID ");break;
	case  KW_STRING :
		fprintf(out," KW_STRING ");break;
	default:
		fprintf(out," KW_ERROR ");break;
	}
}
void print_errors(FILE *out,struct Queue *errors)
{
	struct Queue_Node *it;
	for(it=errors->first;it!=NULL;it=it->prev)
	{
		print_translation_error(out,(struct Translation_Error*)it->data);
	}
}
void print_function_args(FILE *out,struct Type_Function *func)
{
	size_t i;
	if(func->number_of_arguments==0)
		return;

	print_denoted(out,(struct Denoted*)func->arguments[0]);
	for(i=1;i<func->number_of_arguments;++i)
	{
		fprintf(out,", ");
		print_denoted(out,(struct Denoted*)func->arguments[i]);
	}	
}
void print_type_qualifier(FILE *out,struct Type *type)
{

	switch(type->specifier)
	{
		case TS_VOID:
		case TS_CHAR:
		case TS_INT:
		case TS_FLOAT:
		case TS_DOUBLE:
			fprintf(out,"%s %s",
				(AS_BASIC_TYPE_PTR(type)->is_const?"constant ":""),
				(AS_BASIC_TYPE_PTR(type)->is_volatile ?"volatile ":"")
			       );
			break;	
		case TS_STRUCT:
		case TS_UNION:
			fprintf(out,"%s %s",
				(AS_STRUCT_UNION_PTR(type)->is_const?"constant ":""),
				(AS_STRUCT_UNION_PTR(type)->is_volatile ?"volatile ":"")
			       );
			break;
		case TS_ENUM:
			fprintf(out,"%s %s",
				(AS_STRUCT_UNION_PTR(type)->is_const?"constant ":""),
				(AS_STRUCT_UNION_PTR(type)->is_volatile ?"volatile ":"")
			       );
			break;
		case TS_POINTER:
			fprintf(out,"%s %s",
				(AS_TYPE_PTR_PTR(type)->is_const?"constant ":""),
				(AS_TYPE_PTR_PTR(type)->is_volatile ?"volatile ":"")
			       );
			break;
		case TS_ARRAY:
		case TS_FUNC:
		case TS_BITFIELD:
			break;
		case TS_NONE:
		case TS_ERROR:
		default:
			fprintf(out,"error");
	}
}
#undef TOK
#undef INDENT

#endif
