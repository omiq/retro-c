# retro-c examples
Install cl65 then compile and run with 
```cl65 hello.c -o hellopet.prg -t pet && xpet hellopet.prg```

Simple CP/M examples could be compiled with Hi-tech C (CP/M-80) V3.09 but easier to use z88dk

### Cross-Compile and create Z80 Pack compatible disk:
```zcc +cpm -lm -o cpm-keyb.com -subtype=z80pack -create-app cpm-keyb.c```
