bison -d cminus.y &&
flex cminus.l &&
gcc -c *.c &&
gcc -o cminus *.o -ll &&
mkdir -p results &&
for file in testfiles/*
do
  name=${file##*/}
  base=${name%.c}
  ./cminus "$file" > results/"$base".txt
done &&
./clearfiles.sh
