# Compiler-Project

## Lexical Analyser
Run this command:

```
./compile.sh
```

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
./cminus mdc.c
./cminus sort.c
```
