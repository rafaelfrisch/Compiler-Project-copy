flex cminus.l &&
gcc -c lex.yy.c main.c util.c &&
gcc -o cminus *.o -lfl &&
mkdir -p results &&
./cminus testfiles/mdc.c > results/mdc.txt &&
./cminus testfiles/sort.c > results/sort.txt
