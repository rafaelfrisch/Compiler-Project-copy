bison -d cminus.y &&
flex cminus.l &&
gcc -c *.c &&
gcc -o cminus *.o -ll &&
mkdir -p results &&
./cminus testfiles/mdc.c > results/mdc.txt &&
./cminus testfiles/sort.c > results/sort.txt &&
./cminus testfiles/simple.c > results/simple.txt