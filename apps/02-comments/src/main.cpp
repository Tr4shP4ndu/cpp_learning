// 02-comments — notes for humans that the compiler ignores.
#include <iostream>

int main() {
    // This is a single-line comment. Everything after // is ignored.

    /* This is a block comment.
       It can span multiple lines.
       Use it for longer explanations. */

    int speed = 60;  // comments can sit at the end of a line, too

    // Good comments explain *why*, not *what*. The line below is obvious;
    // a comment like "add 5 to speed" would just be noise.
    speed += 5;  // nudge above the limit to test the warning logic later

    std::cout << "speed = " << speed << "\n";
    return 0;
}
