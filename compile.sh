flex cminus.l &&
gcc -c lex.yy.c main.c util.c &&
gcc -o cminus *.o -lfl &&
./cminus mdc.c > teste.txt