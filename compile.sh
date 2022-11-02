flex cminus.l &&
gcc -c lex.yy.c main.c util.c &&
gcc -o cminus *.o -lfl &&
./cminus mdc.c > mdc.txt &&
./cminus sort.c > sort.txt