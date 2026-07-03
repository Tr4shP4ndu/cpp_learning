/* 38-structs (C) — the struct is where C and C++ start to diverge.
   In C a struct holds DATA only: no member functions, and (pre-C23) you often
   wrote `struct Point` everywhere or used a typedef. */
#include <stdio.h>

struct Point {
    double x;
    double y;
};

/* typedef lets you write `Person` instead of `struct Person`. */
typedef struct {
    char name[32];
    int age;
} Person;

int main(void) {
    struct Point p = {3.0, 4.0};   /* note: `struct` keyword needed in C */
    printf("point (%.1f, %.1f)\n", p.x, p.y);

    Person alice = {"Alice", 30};
    alice.age += 1;
    printf("%s is %d\n", alice.name, alice.age);
    return 0;
}
