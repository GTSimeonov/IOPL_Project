#ifndef GCC_AST_C
#define GCC_AST_C GCC_AST_C
#include "ast.h"

struct AST_Error* get_error_tree(struct AST *error)
{
	struct AST_Error *ret;
	ret=malloc(sizeof(struct AST_Error));
	ret->type=ERROR;
	ret->error=error;
	return ret;
}
struct AST_Declaration_Error* get_declaration_error_tree(struct Denoted *error)
{

	struct AST_Declaration_Error *ret;
	ret=malloc(sizeof(struct AST_Declaration_Error));
	ret->type=ERROR_DECLARATION;
	ret->error=error;
	return ret;
}

struct AST_Binary_Expression* get_binary_expression_tree(struct AST *left,struct AST *right,enum AST_Type type)
{
	struct AST_Binary_Expression *ret;
	ret=malloc(sizeof(struct AST_Binary_Expression));
	ret->type=type;
	ret->left=left;
	ret->right=right;

	return ret;
}
struct AST_Conditional_Expression* get_conditional_expression_tree(struct AST *left,struct AST *center,struct AST *right)
{
	struct AST_Conditional_Expression *ret;
	ret=malloc(sizeof(struct AST_Conditional_Expression));
	ret->type=OP_COND;
	ret->left=left;
	ret->center=center;
	ret->right=right;
	return ret;
}

struct AST_Function_Expression* get_function_expression_tree(struct AST* id,struct Scope *scope)
{
	struct AST_Function_Expression *ret;
	ret=malloc(sizeof(struct AST_Function_Expression));
	ret->type=OP_FUNCTION;
	ret->id=id;
	Queue_Init(&ret->arguments);
	return ret;
}
struct AST_Unary_Expression* get_unary_expression_tree(struct AST *operand,enum AST_Type type)
{

	struct AST_Unary_Expression *ret;
	ret=malloc(sizeof(struct AST_Unary_Expression));
	ret->operand=operand;
	ret->type=type;

	return ret;
}
struct AST_Rvalue_Expression* get_rvalue_expression_tree(struct token *id)
{
	struct AST_Rvalue_Expression *ret;
	ret=malloc(sizeof(struct AST_Rvalue_Expression));
	ret->type=OP_RVALUE;
	ret->id=id;
	return ret;
}
struct AST_Lvalue_Expression* get_lvalue_expression_tree(struct token *id,struct Scope* scope)
{
	struct AST_Lvalue_Expression *ret;
	ret=malloc(sizeof(struct AST_Lvalue_Expression));
	ret->type=OP_LVALUE;
	ret->id=id;
	ret->value_type=check_ordinary(scope,id);
	if(ret->value_type==NULL)
	{
		/*TODO error*/
	}
	return ret;
}





struct AST_Labeled_Statement* get_labeled_statement_tree(struct token *label,struct AST* statement,enum AST_Type type)
{
	struct AST_Labeled_Statement *ret;
	ret=malloc(sizeof(struct AST_Labeled_Statement));
	ret->type=type;
	ret->label=label;
	ret->statement=statement;

	return ret;
}
struct AST_Compound_Statement* get_compound_statement_tree(struct Scope *parent_scope)
{
	struct AST_Compound_Statement *ret;
	ret=malloc(sizeof(struct AST_Compound_Statement));
	ret->type=ST_COMPOUND;
	Queue_Init(&ret->components);
	ret->scope=get_normal_scope(parent_scope,BLOCK_SCOPE);
	return ret;
}
struct AST_If_Statement* get_if_statement_tree()
{
	struct AST_If_Statement *ret;
	ret=malloc(sizeof(struct AST_If_Statement));
	ret->type=ST_IF;

	return ret;
}
struct AST_Switch_Statement* get_switch_statement_tree()
{
	struct AST_Switch_Statement *ret;
	ret=malloc(sizeof(struct AST_Switch_Statement));
	ret->type=ST_SWITCH;
	return ret;
}
struct AST_While_Statement* get_while_statement_tree()
{
	struct AST_While_Statement *ret;
	ret=malloc(sizeof(struct AST_While_Statement));
	ret->type=ST_WHILE;
	return ret;
}
struct AST_Do_While_Statement* get_do_while_statement_tree()
{
	struct AST_Do_While_Statement *ret;
	ret=malloc(sizeof(struct AST_Do_While_Statement));
	ret->type=ST_DO_WHILE;
	return ret;
}

struct AST_For_Statement* get_for_statement_tree()
{
	struct AST_For_Statement *ret;
	ret=malloc(sizeof(struct AST_For_Statement));
	ret->type=ST_FOR;
	return ret;
}
struct AST_Return_Statement* get_return_statement_tree(struct AST* return_expression)
{
	struct AST_Return_Statement *ret;
	ret=malloc(sizeof(struct AST_If_Statement));
	ret->type=ST_RETURN;
	ret->return_expression=return_expression;
	return ret;
}

struct AST_Goto_Statement* get_goto_statement_tree(struct token *label,struct Scope *scope)
{
	struct AST_Goto_Statement *ret;
	ret=malloc(sizeof(struct AST_Goto_Statement));
	ret->type=ST_GOTO;
	ret->label=label;
	return ret;
}


struct AST* get_nop_tree()
{
	struct AST* ret;
	ret=malloc(sizeof(struct AST*));
	ret->type=OP_NOP;
	return ret;
}



struct AST_Type_Definition* get_type_definition_tree(struct Denoted_Typedef *definition)
{
	struct AST_Type_Definition *ret;
	ret=malloc(sizeof(struct AST_Type_Definition));
	ret->type=ST_TYPE_DEFINITION;
	ret->definition=definition;

	return ret;

}
struct AST_Object_Declaration* get_object_declaration_tree(struct Denoted_Object *object,struct AST *initializer)
{
	struct AST_Object_Declaration *ret;
	ret=malloc(sizeof(struct AST_Object_Declaration));
	ret->type=ST_OBJECT_DECLARATION;
	ret->object=object;
	ret->initializer=initializer;
	
	return ret;
}

struct AST_Function_Definition* get_function_definition_tree(struct Scope *scope,struct Denoted_Function *function)
{
	struct AST_Function_Definition *ret;
	ret=malloc(sizeof(struct AST_Function_Definition));
	ret->type=ST_FUNCTION_DEFINITION;
	ret->function=function;
	return ret;
}

struct AST_Function_Declaration* get_function_declaration_tree(struct Scope *scope,struct Denoted_Function *function)
{
	struct AST_Function_Declaration *ret;
	ret=malloc(sizeof(struct AST_Function_Declaration));
	ret->type=ST_FUNCTION_DECLARATION;
	ret->function=function;
	return ret;
}
struct AST_Translation_Unit* get_translation_unit_tree(struct Scope* parent_scope)
{
	struct AST_Translation_Unit *ret;
	ret=malloc(sizeof(struct AST_Translation_Unit));
	ret->type=TRANSLATION_UNIT;
	Queue_Init(&ret->components);
	ret->scope=get_normal_scope(parent_scope,FILE_SCOPE);
	return ret;
}


void delete_ast(struct AST* ast)
{
	switch(ast->type)
	{
		case OP_COMMA:
		case OP_ADDITION:
		case OP_SUBTRACTION:
		case OP_MUL:
		case OP_DIV:
		case OP_REMAINDER:
		case OP_ASSIGN:
		case OP_ADD_ASSIGN:
		case OP_SUBTRACT_ASSIGN:
		case OP_MULTIPLY_ASSIGN:
		case OP_REMAINDER_ASSIGN:
		case OP_DIV_ASSIGN:
		case OP_SHIFT_LEFT_ASSIGN:
		case OP_AND_ASSIGN:
		case OP_SHIFT_RIGHT_ASSIGN:
		case OP_XOR_ASSIGN:
		case OP_PIPE_ASSIGN:
		case OP_LOGICAL_AND:
		case OP_LOGICAL_OR:
		case OP_BITWISE_OR:
		case OP_BITWISE_AND:
		case OP_BITWISE_XOR:
		case OP_MEMBER_TROUGH_PTR:
		case OP_MEMBER:
		case OP_ARR_SUBSCRIPT:
		case OP_SHIFT_LEFT:
		case OP_LESS_EQ:
		case OP_NOT_EQUAL:
		case OP_EQUAL:
		case OP_GREATER:
		case OP_LESS:
		case OP_GREATER_EQ:
		case OP_SHIFT_RIGHT:
			delete_ast_binary_expression((struct AST_Binary_Expression*)ast);
			break;
		case OP_COND:
			delete_ast_conditional_expression((struct AST_Conditional_Expression*)ast);
			break;
		case OP_FUNCTION:
			delete_ast_function_expression((struct AST_Function_Expression*)ast);
			break;
		case OP_NOP:
			/*it is just a ast node*/
			free(ast);
			break;
		case OP_LOGICAL_NOT:
		case OP_BITWISE_NOT:
		case OP_ADDR_OF:
		case OP_DEREFERENCE:
		case OP_POSTFIX_INC:
		case OP_POSTFIX_DEC:
		case OP_PREFIX_INC:
		case OP_PREFIX_DEC:
		case OP_UNARY_PLUS:
		case OP_UNARY_MINUS:
		case OP_CAST:
		case OP_SIZEOF:
			delete_ast_unary_expression((struct AST_Unary_Expression*)ast);
			break;
		case OP_LVALUE:
			delete_ast_lvalue_expression((struct AST_Lvalue_Expression*)ast);
			break;
		case OP_RVALUE:
			delete_ast_rvalue_expression((struct AST_Rvalue_Expression*)ast);
			break;
		case ST_COMPOUND:
			delete_ast_compound_statement((struct AST_Compound_Statement*)ast);
			break;
		case ST_SWITCH:
			delete_ast_switch_statement((struct AST_Switch_Statement*)ast);
			break;
		case ST_IF:
			delete_ast_if_statemtent((struct AST_If_Statement*)ast);
			break;
		case ST_WHILE:
			delete_ast_while_statemtent((struct AST_While_Statement*)ast);
			break;
		case ST_DO_WHILE:
			delete_ast_do_while_statement((struct AST_Do_While_Statement*)ast);
			break;
		case ST_GOTO:
			delete_ast_goto_statemtent((struct AST_Goto_Statement*)ast);
			break;
		case ST_LABEL:
		case ST_CASE:
			delete_ast_labeled_statement((struct AST_Labeled_Statement*)ast);
			break;
		case ST_DEFAULT:
		case ST_CONTINUE:
		case ST_BREAK:
			/*I think it doesnt come with anything*/
			free(ast);
			break;
		case ST_RETURN:
			delete_ast_return_statement((struct AST_Return_Statement*)ast);
			break;
		case ST_FOR:
			delete_ast_for_statement((struct AST_For_Statement*)ast);
			break;
		case ST_OBJECT_DECLARATION:
			delete_ast_object_declaration((struct AST_Object_Declaration*)ast);
			break;
		case ST_TYPE_DEFINITION:
			delete_ast_type_definition((struct AST_Type_Definition*)ast);
			break;
		case ST_FUNCTION_DEFINITION:
			delete_ast_function_definition((struct AST_Function_Definition*)ast);
			break;
		case ST_FUNCTION_DECLARATION:
			delete_ast_function_declaration((struct AST_Function_Declaration*)ast);
			break;
		case TRANSLATION_UNIT:
			delete_ast_translation_unit((struct AST_Translation_Unit*)ast);
			break;
		case ERROR:
			delete_ast_error((struct AST_Error*)ast);
			break;
		case ERROR_DECLARATION:
			delete_ast_declaration_error((struct AST_Declaration_Error*)ast);
			break;
		default:
			assert(0);
	}
}
void delete_ast_error(struct AST_Error *error)
{
	if(error->error!=NULL)
		delete_ast(error->error);
	free(error);
}
void delete_ast_declaration_error(struct AST_Declaration_Error *error)
{
	if(error->error!=NULL)
		delete_denoted_error((struct Denoted_Error*)error->error);
	free(error);
}
void delete_ast_binary_expression(struct AST_Binary_Expression *binary_expression)
{
	if(binary_expression->left!=NULL)
		delete_ast(binary_expression->left);
	if(binary_expression->right!=NULL)
		delete_ast(binary_expression->right);

	free(binary_expression);
}
void delete_ast_conditional_expression(struct AST_Conditional_Expression *cond_expression)
{
	
	if(cond_expression->left!=NULL)
		delete_ast(cond_expression->left);
	if(cond_expression->center!=NULL)
		delete_ast(cond_expression->center);
	if(cond_expression->right!=NULL)
		delete_ast(cond_expression->right);

	free(cond_expression);
}
void delete_ast_function_expression(struct AST_Function_Expression *function_expression)
{
	struct Queue_Node *it;
	if(function_expression->id!=NULL)
		delete_ast(function_expression->id);
	while(function_expression->arguments.size>0)
		delete_ast(Queue_Pop(&function_expression->arguments));

	free(function_expression);
	
}
void delete_ast_rvalue_expression(struct AST_Rvalue_Expression *rval_expression)
{
	free(rval_expression->id);
	free(rval_expression);
}
void delete_ast_lvalue_expression(struct AST_Lvalue_Expression *lval_expression)
{
	free(lval_expression->id);
	free(lval_expression);
}
void delete_ast_unary_expression(struct AST_Unary_Expression *unary_expression)
{
	if(unary_expression->operand!=NULL)
		delete_ast(unary_expression->operand);
	free(unary_expression);
}
void delete_ast_labeled_statement(struct AST_Labeled_Statement *labeled_statement)
{
	if(labeled_statement->label!=NULL)
		free(labeled_statement->label);	
	if(labeled_statement->statement!=NULL)
		delete_ast(labeled_statement->statement);
	free(labeled_statement);
}
void delete_ast_compound_statement(struct AST_Compound_Statement *compound_statement)
{
	if(compound_statement->scope!=NULL)
		delete_scope(compound_statement->scope);
	while(compound_statement->components.size>0)
		delete_ast((struct AST*)Queue_Pop(&compound_statement->components));
	free(compound_statement);
}
void delete_ast_for_statement(struct AST_For_Statement *for_statement)
{
	if(for_statement->condition!=NULL)
		delete_ast(for_statement->condition);
	if(for_statement->initialisation!=NULL)
		delete_ast(for_statement->initialisation);
	if(for_statement->update!=NULL)
		delete_ast(for_statement->update);
	if(for_statement->body_statement!=NULL)
		delete_ast(for_statement->body_statement);
	free(for_statement);
}
void delete_ast_while_statemtent(struct AST_While_Statement *while_statement)
{
	if(while_statement->condition!=NULL)
		delete_ast(while_statement->condition);
	if(while_statement->body_statement!=NULL)
		delete_ast(while_statement->body_statement);
	free(while_statement);
}
void delete_ast_do_while_statement(struct AST_Do_While_Statement *do_while_statement)
{
	if(do_while_statement->condition!=NULL)
		delete_ast(do_while_statement->condition);
	if(do_while_statement->body_statement!=NULL)
		delete_ast(do_while_statement->body_statement);
	free(do_while_statement);
}
void delete_ast_if_statemtent(struct AST_If_Statement *if_statement)
{
	if(if_statement->condition!=NULL)
		delete_ast(if_statement->condition);
	if(if_statement->body_statement!=NULL)
		delete_ast(if_statement->body_statement);
	if(if_statement->else_statement!=NULL)
		delete_ast(if_statement->else_statement);
	free(if_statement);
}
void delete_ast_goto_statemtent(struct AST_Goto_Statement *goto_statement)
{
	if(goto_statement->label!=NULL)
		free(goto_statement->label);
	free(goto_statement);
}
void delete_ast_switch_statement(struct AST_Switch_Statement *switch_statement)
{
	if(switch_statement->condition!=NULL)
		delete_ast(switch_statement->condition);
	if(switch_statement->body_statement!=NULL)
		delete_ast(switch_statement->body_statement);
	free(switch_statement);
}
void delete_ast_return_statement(struct AST_Return_Statement *return_statement)
{
	if(return_statement->return_expression!=NULL)
		delete_ast(return_statement->return_expression);
	free(return_statement);
}
void delete_ast_type_definition(struct AST_Type_Definition *type_definition)
{
	/*deleting denoted objects in scopes*/
	free(type_definition);
}
void delete_ast_object_declaration(struct AST_Object_Declaration *object_declaration)
{
	if(object_declaration->initializer!=NULL)
		delete_ast(object_declaration->initializer);
	free(object_declaration);

}
void delete_ast_function_definition(struct AST_Function_Definition *function_definition)
{
	free(function_definition);
}
void delete_ast_function_declaration(struct AST_Function_Declaration *function_declaration)
{
	free(function_declaration);
}
void delete_ast_translation_unit(struct AST_Translation_Unit *translation_unit)
{
	while(translation_unit->components.size>0)
		delete_ast((struct AST*)Queue_Pop(&translation_unit->components));
	if(translation_unit->scope!=NULL)
		delete_scope(translation_unit->scope);
	free(translation_unit);
}






#endif
