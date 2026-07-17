/* 27-bitfields (C) — pack booleans with struct bit-fields and flip bits with masks. */
#include <stdio.h>

/* Each field is given an exact width in bits. */
struct Flags {
    unsigned bold : 1;
    unsigned italic : 1;
    unsigned level : 3;   /* 0..7 */
};

/* Flag masks for a plain int: distinct single bits via shifting. */
#define READ  (1u << 0)   /* 0b001 */
#define WRITE (1u << 1)   /* 0b010 */
#define EXEC  (1u << 2)   /* 0b100 */

int main(void) {
    struct Flags f = {0};
    f.bold = 1;
    f.level = 5;
    printf("bold=%u italic=%u level=%u\n", f.bold, f.italic, f.level);

    unsigned mask = 0;
    mask |= READ;          /* set   */
    mask |= WRITE;
    mask &= ~WRITE;        /* clear */
    printf("READ set?  %d\n", (mask & READ) != 0);   /* test */
    printf("WRITE set? %d\n", (mask & WRITE) != 0);
    printf("EXEC set?  %d\n", (mask & EXEC) != 0);
    return 0;
}
