bison -d cminus.y &&
flex cminus.l &&
gcc -c *.c &&
gcc -o cminus *.o -ll &&
./cminus testfiles/erick.c
./clearfiles.sh
