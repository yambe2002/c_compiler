#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// token types
enum {
  TK_NUM = 256, // integer
  TK_EOF,       // end of input
};

// token
typedef struct {
  int ty;      // token type
  int val;     // numeric value if ty == TK_NUM
  char *input; // token string (for error message)
} Token;

Token tokens[100];

void tokenize(char *p) {
  int i = 0;
  while (*p) {
    // skip empty char
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (*p == '+' || *p == '-') {
      tokens[i].ty = *p;
      tokens[i].input = p;
      i++;
      p++;
      continue;
    }

    if (isdigit(*p)) {
      tokens[i].ty = TK_NUM;
      tokens[i].input = p;
      tokens[i].val = strtol(p, &p, 10);
      i++;
      continue;
    }

    fprintf(stderr, "failed to tokenize: %s\n", p);
    exit(1);
  }

  tokens[i].ty = TK_EOF;
  tokens[i].input = p;
}

void error(int i) {
  fprintf(stderr, "unexpected token: %s\n", tokens[i].input);
  exit(1);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Number of arguments is not correct");
    return 1;
  }

  tokenize(argv[1]);

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // the first token should be numeric
  if (tokens[0].ty != TK_NUM)
    error(0);
  printf("  mov rax, %d\n", tokens[0].val);

  // output assembly
  int i = 1;
  while (tokens[i].ty != TK_EOF) {
    if (tokens[i].ty == '+' || tokens[i].ty == '-') {
      i++;
      if (tokens[i].ty != TK_NUM)
        error(i);
      if (tokens[i - 1].ty == '+')
        printf("  add");
      else
        printf("  sub");
      printf(" rax, %d\n", tokens[i].val);
      i++;
      continue;
    }

    error(i);
  }

  printf("  ret\n");
  return 0;
}
