bison -d cminus.y &&
flex cminus.l &&
gcc -c *.c &&
gcc -o cminus *.o -ll &&
./cminus testfiles/mdc.c > results/mdc.txt &&
./cminus testfiles/sort.c > results/sort.txt