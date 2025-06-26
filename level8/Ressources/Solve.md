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
        if (*(auth + 8))
            system("/bin/sh");
        else
            fwrite("Password:\n", 1, 0xAu, stdout);
    }
  }
  return 0;
}
```