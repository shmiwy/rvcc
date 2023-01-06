#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "%s: invalid number of arguments\n", argv[0]);
        return 1;
    }
    char *p = argv[1];
    printf("  .globl main\n");
    printf("main:\n");

    // 这里将输入解析为： num (op num) (op num) ... 格式，(op num) 可选
    printf("  li a0, %ld\n", strtol(p, &p, 10));

    while (*p) {
        if (*p == '+') {
            p++;
            printf("  addi a0, a0, %ld\n", strtol(p, &p, 10));
        } else if (*p == '-') {
            p++;
            printf("  addi a0, a0, -%ld\n", strtol(p, &p, 10));
        } else {
            fprintf(stderr, "unexpected character: '%c'\n", *p);
            return 1;
        }
    }
    printf("  ret\n");

    return 0;
}