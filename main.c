#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef enum TokenKind {
    TK_PUNCT,
    TK_NUM,
    TK_EOF,
}TokenKind;

typedef struct Token Token;
struct Token
{
    TokenKind kind;
    Token* next;
    int val;
    char* loc;
    int len;
};

static Token* newToken(TokenKind kind, char* loc) {
    Token* token = calloc(1, sizeof(Token));
    token->kind = kind;
    token->next = NULL;
    token->loc = loc;

    return token;
}


static Token* toToken(char* p) {
    Token head = {};
    Token* cur = &head;

    while (*p) {
        if (isspace(*p)) {
            p++;
        } else if (isdigit(*p)) {
            cur->next = newToken(TK_NUM, p);
            cur = cur->next;
            char* olerPtr = p;
            cur->val = strtol(p, &p, 10);
            cur->len = p - olerPtr;
        } else if (*p == '+' || *p == '-') {
            cur->next = newToken(TK_PUNCT, p);
            cur = cur->next;
            cur->len = 1;
            p++;
        } else {
            fprintf(stderr, "invalid token: %c", *p);
            exit(1);
        }
    }

    cur->next = newToken(TK_EOF, p);
    return head.next;
}

static bool equal(Token *tok, char *str) {
  return memcmp(tok->loc, str, tok->len) == 0 && str[tok->len] == '\0';
}

int getNumber(Token* tok) {
    if (tok->kind != TK_NUM) {
        fprintf(stderr ,"expect a number");
        exit(1);
    }
    return tok->val;
}

static Token *skip(Token *tok, char *str) {
  if (!equal(tok, str)) {
    fprintf(stderr, "expect '%s'", str);
  }
  return tok->next;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "%s: invalid number of arguments\n", argv[0]);
        exit(1);
    }
    Token* tok = toToken(argv[1]);
    printf("  .globl main\n");
    printf("main:\n");

    // 这里将输入解析为： num (op num) (op num) ... 格式，(op num) 可选
    printf("  li a0, %d\n", getNumber(tok));
    tok = tok->next;

    while (tok->kind != TK_EOF) {
        if (equal(tok, "+")) {
            tok = tok->next;
            printf("  addi a0, a0, %d\n", getNumber(tok));
        } else if (equal(tok, "-")) {
            tok = skip(tok, "-");
            printf("  addi a0, a0, -%d\n", getNumber(tok));
        } else {
            fprintf(stderr, "invalid string");
            exit(1);
        }
        tok = tok->next;
    }
    printf("  ret\n");

    exit(0);
}