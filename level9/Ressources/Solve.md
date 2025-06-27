```cpp
#include <unistd.h>
#include <cstring>

class N {
	public:
		int nb;					//   4 bytes
		int (N::*func)(N &);	//   4 bytes
		char annotation[108];   // 108 bytes

		N(int val) : nb(val)
		{
			this->func = &N::operator+;
		}
		int operator+(N &right)
		{
			return this->nb + right.nb;
		}
		int operator-(N &right)
		{
			return this->nb - right.nb;
		}
		void setAnnotation(char *str)
		{
			memcpy(this->annotation, str, strlen(str));
		}
};

int		main(int ac, char **av)
{
	if (ac < 1)
		exit(1);

	N *a = new N(5);
	N *b = new N(6);

	a->setAnnotation(av[1]);
	return (b->*(b->func))(*a);
}
```

- Doing new N() allocate the class in the head and it's data are contiguous in memory.
- a and b are also allocated contiguously in memory.
- Here memcpy() don't check for limits, so it is vulnerable.
- We want to write 108 bytes in annotation, then overwrite (N::*f)(N &) and then nb

Adresse      Contenu (objets)              Champ
---------    --------------------------    ---------------
0x0804a000   0x00000005                    a->nb
0x0804a004   0x080485a0                    a->func (par défaut : operator+)
0x0804a008   [ annotation (108 octets) ]   a->annotation
...                      
0x0804a080   0x00000006                    b->nb
0x0804a084   0x080485a0                    b->func (par défaut : operator+)

- We will just put a shellcode into the buffer, then we will add the padding until we write 108 bytes, we will then overwrite or member function pointer by the address of annotation to execute the shellcode. BUT it's not a function pointer, it's contained in a Class. Compiler acces the code by doing b->func (Looking for a pointer to member function, it can be operator-() for exemple). So the compiler need a member function address in order to be in accordance with the code.

- Address of annotation = 0x0804a00c (Found in parameter of memcpy())

```cpp
ltrace ./level9 AAAA
__libc_start_main(0x80485f4, 2, 0xbffff7d4, 0x8048770, 0x80487e0 <unfinished ...>
_ZNSt8ios_base4InitC1Ev(0x8049bb4, 0xb7d79dc6, 0xb7eebff4, 0xb7d79e55, 0xb7f4a330) = 0xb7fce990
__cxa_atexit(0x8048500, 0x8049bb4, 0x8049b78, 0xb7d79e55, 0xb7f4a330) = 0
_Znwj(108, 0xbffff7d4, 0xbffff7e0, 0xb7d79e55, 0xb7fed280) = 0x804a008
_Znwj(108, 5, 0xbffff7e0, 0xb7d79e55, 0xb7fed280) = 0x804a078
strlen("AAAA")                                = 4
memcpy(0x0804a00c, "AAAA", 4)                 = 0x0804a00c
_ZNSt8ios_base4InitD1Ev(0x8049bb4, 11, 0x804a078, 0x8048738, 0x804a00c) = 0xb7fce4a0
+++ exited (status 11) +++

```

- Address of operator-() 