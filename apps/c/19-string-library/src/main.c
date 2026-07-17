/* 19-string-library (C) — <string.h> and <ctype.h> over C strings. */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void) {
    const char *src = "Ada Lovelace";

    char buf[32];
    strcpy(buf, src);                  /* copy; buf must hold src + '\0' */
    printf("copy   : %s (len %zu)\n", buf, strlen(buf));

    /* strncpy is bounded: it writes at most n bytes, but may NOT add a
       '\0' if src is too long — so terminate the buffer yourself. */
    char safe[8];
    strncpy(safe, src, sizeof safe - 1);
    safe[sizeof safe - 1] = '\0';
    printf("bounded: %s\n", safe);     /* "Ada Lov" */

    strcat(buf, "!");                  /* append; buf still big enough */
    printf("concat : %s\n", buf);

    printf("cmp    : %d\n", strcmp("abc", "abd"));  /* <0: 'c' < 'd' */

    char *sp = strchr(buf, ' ');       /* first ' '; pointer or NULL */
    if (sp) printf("space  : index %ld\n", (long)(sp - buf));

    char *sub = strstr(buf, "Love");   /* substring search */
    if (sub) printf("found  : %s\n", sub);

    /* <ctype.h>: classify/convert one char at a time. */
    char up[32];
    size_t i = 0;
    for (; src[i] != '\0'; i++) up[i] = (char)toupper((unsigned char)src[i]);
    up[i] = '\0';
    printf("upper  : %s\n", up);
    printf("isdigit('7') = %d\n", isdigit((unsigned char)'7'));
    return 0;
}
