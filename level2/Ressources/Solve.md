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

