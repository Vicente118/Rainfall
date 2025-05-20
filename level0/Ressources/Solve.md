- We have a 32 bit ELF binary called level0 with SUID bit set, let's decompile the code:
```bash
int32_t main(void* arg1)
{
    if (atoi(*(uint32_t*)((char*)arg1 + 4)) != 423)
        _IO_fwrite("No !\n", 1, 5, _IO_stderr);
    else
    {
        char* var_20 = __strdup("/bin/sh");
        int32_t var_1c_1 = 0;
        int32_t eax_5 = __getegid();
        int32_t eax_6 = __geteuid();
        __setresgid(eax_5, eax_5, eax_5);
        __setresuid(eax_6, eax_6, eax_6);
        execv("/bin/sh", &var_20);
    }
    
    return 0;
}
```

----

- This code verify that the first argument passed to the program then passed to atoi() is different than 423.
- To go through this condition we just have to run the program like this:

```bash
> ./level0 "423"
We are level1 !

> cat /home/user/level1/.pass
1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
```
