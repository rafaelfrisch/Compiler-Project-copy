bison -d cminus.y &&
flex cminus.l &&
gcc -c *.c &&
gcc -o cminus *.o -ll &&
mkdir -p results &&
for file in testfiles/*
do
  if [[ $file == *.c ]]
    then
      name=${file##*/}
      base=${name%.c}
      ./cminus "$file" > results/"$base".txt
    fi
done &&
for file in testfiles/*
do
  if [[ $file == *.tm ]]
    then
      name=${file##*/}
      base=${name%.tm}
      mv "$file" results/"$base".tm
    fi
done &&
./clearfiles.sh
