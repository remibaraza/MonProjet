//#pragma once

#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <err.h>

/**
 * \page Lexer
 *
 * The lexer cuts some input text into block called tokens.

 * This process is done **on demand**: the lexer doesn't read the
 * input more than it needs, and only creates tokens when lexer_peek
 * or lexer_pop is called, and no token is available.
 *
 * "2 + 3" will produce 3 tokens:
 *   - TOKEN_NUMBER { .value = 2 }
 *   - TOKEN_PLUS
 *   - TOKEN_NUMBER { .value = 3 }
 */
ssize_t reverse(ssize_t a)
{
    ssize_t res = 0;
    while (a != 0)
    {
        res = res * 10 + a % 10;
        a /= 10;
    }
    return res;
}
ssize_t numberlen(ssize_t n)
{
    ssize_t res = 1;
    while (n /10 > 0)
    {
        res ++;
        n/=10;
    }
    return res;
}

struct lexer *lexer_new(const char *input)
{

    struct lexer *new = xmalloc(sizeof(struct lexer));
    new->input = input;
    new->pos = 0;
    /*struct token *newtoken = xmalloc(sizeof(struct token));
    newtoken->type = input[new->pos];
    newtoken->value = 0;
    new->current_tok = newtoken;*/
    return new;

}
void lexer_free(struct lexer *lexer)
{
    //if (lexer->current_tok != NULL)
    free(lexer->current_tok);
    free(lexer);

}
struct token *lexer_peek(struct lexer *lexer)
{
    struct token *tokenpeek = zalloc(sizeof(struct token));

    if (lexer->pos >= strlen(lexer->input))
    {
        tokenpeek->type = TOKEN_EOF;
        return tokenpeek;
    }

    if (lexer->input[lexer->pos] >= '0' && lexer->input[lexer->pos] <= '9')
    {
        ssize_t res = 0;
        ssize_t inter = lexer->pos;
        ssize_t resf;
        int zero = 0;
        int mult = 1;
        while (lexer->input[inter] >= '0' && lexer->input[inter] <= '9')
        {
            if (!(lexer->input[inter +1] >= '0' && lexer->input[inter+1] <= '9') && lexer->input[inter] == '0')
            {
                res *= 10;
                zero = 1;
                break;
            }
            res += (ssize_t)(lexer->input[inter] - '0')* mult;
            mult *= 10;
            inter += 1;
        }
        resf = reverse(res);
        if (zero)
            resf *= 10;

        tokenpeek->type = TOKEN_NUMBER;
        tokenpeek->value = resf;
        return tokenpeek;
    }
    if (lexer->input[lexer->pos] == '(')
    {
        tokenpeek->type = TOKEN_LEFT_PAR;

    }
    else if (lexer->input[lexer->pos] == ')')
    {
        tokenpeek->type = TOKEN_RIGHT_PAR;

    }
    else  if (lexer->input[lexer->pos] == '+')
    {
        tokenpeek->type = TOKEN_PLUS;

    }
    else  if (lexer->input[lexer->pos] == '-')
    {
        tokenpeek->type = TOKEN_MINUS;

    }
    else if (lexer->input[lexer->pos] == '/')
    {
        tokenpeek->type = TOKEN_DIV;

    }
    else if (lexer->input[lexer->pos] == '*')
    {
        tokenpeek->type = TOKEN_MUL;

    }
    else
    {
        err(0,"Token not valid");
    }

    return tokenpeek;

}
struct token *lexer_pop(struct lexer *lexer)
{
    if (lexer->input[lexer->pos] == ' ')
        lexer->pos += 1;
    struct token *toremove = lexer_peek(lexer);
    if (toremove->type == TOKEN_NUMBER)
    {
        lexer->pos += numberlen(toremove->value);
    }
    else
    {
        lexer->pos += 1;
    }
    
    lexer->current_tok = NULL;
    return toremove;
}
/*int main()
{
    struct lexer *new = lexer_new("3ey");
    struct token *token = lexer_pop(new);
    printf("%p\n",(void*)token);
    lexer_free(new);
    
}*/
