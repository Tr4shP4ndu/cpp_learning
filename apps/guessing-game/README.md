# guessing-game

A number-guessing game: the program picks 1–100 and tells you "higher" or
"lower" until you get it. A first taste of putting the basics together into
something interactive.

**Builds on:** io-basics (05), conditionals (07), while-loop (14), plus
`<random>`.

## Run
```sh
make project name=guessing-game                 # interactive
echo "50 25 37 31 34" | make project name=guessing-game ARGS=42   # scripted, fixed seed
```
Pass a seed (`ARGS=42`) to get the same secret every time — handy for practice.

## Try extending it
- Cap the number of tries and print "you lose".
- Let the player choose the range (read `argv`).
- Track a best score across games (persist it to a file — see `cli-todo`).
