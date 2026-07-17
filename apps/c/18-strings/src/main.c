/* 18-strings (C) — a C string is a raw char array ending in a '\0' byte.
   Everything std::string does for you, you do by hand with <string.h>. */
#include <stdio.h>
#include <string.h>

int main(void) {
    const char *first = "Ada";
    const char *last  = "Lovelace";

    /* You must provide a buffer big enough for the result + the '\0'. */
    char full[32];

    /* strcpy/strcat build the string manually. Sizes are your responsibility;
       an overflow here is a classic security bug. */
    strcpy(full, first);   /* full = "Ada"          */
    strcat(full, " ");     /* full = "Ada "         */
    strcat(full, last);    /* full = "Ada Lovelace" */

    printf("full   : %s\n", full);
    printf("length : %zu\n", strlen(full));  /* strlen counts up to the '\0' */
    printf("first  : %c\n", full[0]);

    /* strchr finds a character; returns a pointer, not an index. */
    char *p = strchr(full, 'L');
    if (p) printf("find L : index %ld\n", (long)(p - full));
    return 0;
}
