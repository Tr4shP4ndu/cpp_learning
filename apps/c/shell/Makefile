# shell — builds with the shared rules at the repo root.
#   make        build it        make run     build & run     make clean
APP := shell
# Find rules.mk at the repo root, however deep this app sits.
include $(firstword $(wildcard ../../rules.mk ../../../rules.mk ../../../../rules.mk ../../../../../rules.mk))
