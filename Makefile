all: delete compile run

delete:
    rm -rf main

compile:
    g++ main.cpp -lncurses -o main

run:
    ./main