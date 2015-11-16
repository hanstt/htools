#ifndef HUTILS_LEXER_H
#define HUTILS_LEXER_H

#include <stdlib.h>
#include <hutils/macros.h>

enum LexerError {
	LEXER_ERROR_NONE,
	LEXER_ERROR_INVALID_HEX,
	LEXER_ERROR_INVALID_NUMBER,
	LEXER_ERROR_UNTERMINATED_LITERAL
};
enum LexerType {
	LEXER_ALNUM,
	LEXER_EOF,
	LEXER_ERROR,
	LEXER_HEX,
	LEXER_LITERAL,
	LEXER_NUMBER,
	LEXER_SYMBOL
};

typedef size_t (*LexerCallback)(void *, char *, size_t);

struct Lexer;
struct LexerToken {
	enum	LexerType type;
	char	*str;
};

struct Lexer	*lexer_create(LexerCallback, void *) FUNC_RETURNS;
void		lexer_free(struct Lexer **);
int		lexer_get_col_no(struct Lexer const *) FUNC_RETURNS;
enum LexerError	lexer_get_error(struct Lexer const *) FUNC_RETURNS;
int		lexer_get_line_no(struct Lexer const *) FUNC_RETURNS;
int		lexer_token_get(struct Lexer *, struct LexerToken *)
	FUNC_RETURNS;

#endif
