## Lex programs

```
lex program.l
gcc lex.yy.c
./a.out
```

## Yacc programs

```
bison -d program.y
gcc program5.tab.c
./a.out
```

## Combined

```
bison -d calc.y
lex calc.l
gcc calc.tab.c lex.yy.c -o calc
./calc
```
