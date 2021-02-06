#ifndef GCC_PARSE_EXPR_H
#define GCC_PARSE_EXPR_H GCC_PARSE_EXPR_H
#include"chonky.h"
#include"queue.h"
#include"lexer.h"
#include"parse_declaration.h"
#include"ast.h"
#include<limits.h>


struct AST* parse_expression(struct Queue *tokens,struct Scope *scope);
struct AST* parse_const_expression(struct Queue *tokens,struct Scope *scope);
struct AST* parse_primary_expression(struct Queue *tokens,struct Scope *scope);


struct AST_Function_Expression* parse_arglist(struct Queue *tokens,struct Scope *scope,struct AST* id);
struct AST* parse_postfix_expression(struct Queue *tokens,struct Scope *scope);

struct AST* parse_cast_expression(struct Queue *tokens,struct Scope *scope);
struct AST* parse_unary_expression(struct Queue *tokens,struct Scope *scope);

struct AST* parse_multiplicative_expression(struct Queue *tokens,struct Scope *scope);

struct AST* parse_additive_expression(struct Queue *tokens,struct Scope *scope);

struct AST* parse_shift_expression(struct Queue *tokens,struct Scope *scope);


struct AST* parse_relational_expression(struct Queue *tokens,struct Scope *scope);
struct AST* parse_equality_expression(struct Queue *tokens,struct Scope *scope);
struct AST* parse_and_expression(struct Queue *tokens,struct Scope *scope);
struct AST* parse_exclusive_or_expression(struct Queue *tokens,struct Scope *scope);
struct AST* parse_inclusive_or_expression(struct Queue *tokens,struct Scope *scope);
struct AST* parse_logical_and_expression(struct Queue *tokens,struct Scope *scope);
struct AST* parse_logical_or_expression(struct Queue *tokens,struct Scope *scope);
struct AST* parse_conditional_expression(struct Queue *tokens,struct Scope *scope);
struct AST* parse_assignment_expression(struct Queue *tokens,struct Scope *scope);
struct AST* parse_comma_expression(struct Queue *tokens,struct Scope *scope);





/*

expression:
	e15
comma-expression:
	assignment-expression(,assignment-expression)*

assignment-expression:
	unary-expression ( ( = | += | -= | %= | /= | *= | >>= | <<= | &= | |= | ^= ) assignment-expression

conditional-expression:
	logical-or-expression
	logical-or-expression?expression:conditional-expression
logical-or-expression:
	logical-and-expression ( || logical-and-expression )*
logical-and-expression:
	inclusive-or-expression(&&inclusive-or-expression)*
inclusive-or-expression:
	exclusive-or-expression (|exclusive-or-expression)*
exclusive-or-expression:
	and-expression (^ and-expression)*
and-expression:
	equality-expression ( & equality-expression ) *
equality-expression:
	realtional-expression ( ( == | != ) relational-expression )*
relational-expression:
	shift-expression ( ( < | > | <= | >= ) shift-expression )*

shift-expression:
	additive-expression ( ( << | >> ) additive-expression)*

additive-expression:
	multiplicative-expression ( ( + | - ) multiplicative-expression )*

multiplicative-expression:
	cast-expression ( ( * | / | % ) cast-expression )*
	
unary-expression:
	++unary-expression
	--unary-expression
	+unary-expression
	-unary-expression
	!cast-expression
	~cast-expression
	*cast-expression
	&cast-expression
	sizeof ( typename )
	sizeof unary-expression
	postfix-expression

	cast-expression:
		unary-expression
		(type)cast-expression


arglist:
	epsilon
	assignment-expression-list(,assignment-expression-list)*
postfix_expression:
	postfix_expression ++
	postfix_expression --
	postfix_expression [ expression ]
	postfix_expression.id
	postfix_expression->id
	postfix_expression ( arglist )


	primary_expression

primary-expression:
	number
	string
	id
	(e)

*/


#endif
