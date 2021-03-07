#ifndef GKEYWORDS_MENU_H
#define GKEYWORDS_MENU_H GKEYWORDS_MENU_H
#include <stddef.h>



enum KEYWORDS{KW_AUTO,KW_DO,KW_DOUBLE,KW_INT,KW_STRUCT,KW_BREAK,KW_ELSE,KW_LONG,KW_SWITCH,KW_CASE,KW_ENUM,KW_REGISTER,KW_TYPEDEF,KW_CHAR,KW_EXTERN,KW_RETURN,KW_UNION,KW_CONST,KW_FLOAT,KW_SHORT,KW_UNSIGNED,KW_CONTINUE,KW_FOR,KW_SIGNED,KW_VOID,KW_DEFAULT,KW_GOTO,KW_SIZEOF,KW_VOLATILE,KW_IF,KW_STATIC,KW_WHILE,KW_DEFINED,KW_EXCLAMATION,KW_BACK_SLASH,KW_PERCENT,KW_AND,KW_AND_AND,KW_OPEN_NORMAL,KW_CLOSE_NORMAL,KW_STAR,KW_PLUS,KW_COMMA,KW_MINUS,KW_ARROW,KW_COLUMN,KW_SEMI_COLUMN,KW_LESS,KW_EQ,KW_EQEQ,KW_MORE,KW_QUESTION,KW_OPEN_SQUARE,KW_CLOSE_SQUARE,KW_HAT,KW_FLOOR,KW_OPEN_CURLY,KW_CLOSE_CURLY,KW_PIPE,KW_PIPE_PIPE,KW_TILDE,KW_PLUSPLUS,KW_MINUSMINUS,KW_SHIFT_RIGHT,KW_SHIFT_LEFT,KW_LESS_EQ,KW_MORE_EQ,KW_NOT_EQ,KW_PLUS_EQ,KW_MINUS_EQ,KW_STAR_EQ,KW_PERCENT_EQ,KW_SHIFT_LEFT_EQ,KW_SHIFT_RIGHT_EQ,KW_AND_EQ,KW_HAT_EQ,KW_PIPE_EQ,KW_DOT,KW_DIV_EQ,KW_FORWARD_SLASH,KW_NOTYPE,KW_HEXADECIMAL_CONSTANT,KW_DECIMAL_CONSTANT,KW_OCTAL_CONSTANT ,KW_UNSIGNED_DECIMAL_CONSTANT,KW_UNSIGNED_OCTAL_CONSTANT,KW_UNSIGNED_HEXADECIMAL_CONSTANT,KW_UNSIGNED_LONG_HEXADECIMAL_CONSTANT,KW_UNSIGNED_LONG_OCTAL_CONSTANT,KW_UNSIGNED_LONG_DECIMAL_CONSTANT,KW_UNSIGNED_LONG_LONG_DECIMAL_CONSTANT,KW_UNSIGNED_LONG_LONG_HEXADECIMAL_CONSTANT,KW_UNSIGNED_LONG_LONG_OCTAL_CONSTANT,KW_LONG_HEXADECIMAL_CONSTANT,KW_LONG_OCTAL_CONSTANT,KW_LONG_DECIMAL_CONSTANT,KW_LONG_LONG_HEXADECIMAL_CONSTANT,KW_LONG_LONG_OCTAL_CONSTANT,KW_LONG_LONG_DECIMAL_CONSTANT,KW_DOUBLE_DECIMAL_CONSTANT,KW_LONG_DOUBLE_DECIMAL_CONSTANT,KW_FLOAT_DECIMAL_CONSTANT,KW_DOUBLE_HEXADECIMAL_CONSTANT,KW_LONG_DOUBLE_HEXADECIMAL_CONSTANT,KW_FLOAT_HEXADECIMAL_CONSTANT,KW_COMMENT,KW_ID,KW_CHAR_CONSTANT,KW_WIDE_CHAR_CONSTANT,KW_STRING,KW_WIDE_STRING,PKW_IF,PKW_IFDEF,PKW_IFNDEF,PKW_ELIF,PKW_ELSE,PKW_ENDIF,PKW_INCLUDE,PKW_DEFINE,PKW_UNDEF,PKW_LINE,PKW_ERROR,PKW_PRAGMA,PKW_COMMENT,PKW_NOTYPE,KW_HASHTAG,KW_HASHTAG_HASHTAG};


struct automata_entry
{
	unsigned char is_final;
	enum KEYWORDS type;
	struct automata_entry* delta[93];
};
extern struct automata_entry chonky[];
extern int cmpr[256];
extern int uncmpr[256];

#endif
