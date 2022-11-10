# Compiler-Project

## Lexical Analyser
Run this command:

```
./lexicalanalyser.sh
```

Results will be stored on results folder


Or run each compiled file separately:
### Generate Flex

```
flex cminus.l
```

### Generate .o files

```
gcc -c lex.yy.c main.c util.c
```

### Create Lexical Analyser:

```
gcc -o cminus *.o -lfl
```

### Run test Programs

```
mkdir -p results
./cminus testfiles/mdc.c > results/mdc.txt
./cminus testfiles/sort.c > results/sort.txt
```
Results will be stored on results folder