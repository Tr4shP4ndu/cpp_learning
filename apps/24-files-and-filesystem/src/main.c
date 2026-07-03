/* 24-files-and-filesystem (C) — the same with FILE*, fopen/fprintf/fgets.
   C has no std::filesystem; paths are just strings and you check return codes. */
#include <stdio.h>

int main(void) {
    const char *path = "c_learning_demo.txt";

    /* fopen returns a FILE* handle, or NULL on failure — always check it.
       "w" = write (truncate/create). You must fclose to flush and release. */
    FILE *out = fopen(path, "w");
    if (!out) { perror("fopen(write)"); return 1; }
    fprintf(out, "line one\n");
    fprintf(out, "line two\n");
    fclose(out);   /* no RAII in C — forgetting this can lose data */

    /* "r" = read. Read back line by line with fgets. */
    FILE *in = fopen(path, "r");
    if (!in) { perror("fopen(read)"); return 1; }
    printf("reading %s:\n", path);
    char line[128];
    while (fgets(line, sizeof line, in) != NULL) {
        printf("  %s", line);   /* line already ends in '\n' */
    }
    fclose(in);

    remove(path);  /* delete the file */
    return 0;
}
