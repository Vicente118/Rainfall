
During a penetration test, imagine getting your first foothold on a Linux machine and you want to escalate your privileges to root.
You take a look and then perform this command: `find / -perm -4000 2>/dev/null`

This command look for executables on the system that has SUID bit set. This means that no matter who is running the program, it will be executed as the owner of the file.
![[Pasted image 20251204123538.png]]

We can see for exemple here that /usr/bin/pkexec has the SUID bit set:
![[Pasted image 20251204123616.png]]

Since this program is owned by root and SUID bit is set, it will always be run as root user.
But what if we can inject code in it ? Like a shell, then we will automatically get a root shell since the binary is run as root.

Let's get back to our Penetration Test. We are running this : `find / -perm -4000 2>/dev/null`
And we find a unknown binary called `test`
![[Pasted image 20251204124151.png]]

For convenience, let's say that the reverse engineering team already work on the binary and reconstruct the code for us:

```C
int run()
{
	fwrite("Good... Wait what?\n", 1, 0x13, stdout)
	return system("/bin/sh")
}

int main()
{
    char buf[];
    return gets(&buf)
}
```

This code just run initiate a buffer of a fixed size that we don't really know for now and then ask the user to write to it.
![[Pasted image 20251204124649.png]]

The gets() function ask for a user input but do not define any limitation, so what if I write a lot of characters and overflow my buffer size ?
![[Pasted image 20251204124810.png]]

We get a segmentation fault. This means we corrupt stack memory in a way that the program can't continue.

Let's see why happens.
Firstly we need to understand what the purpose of the instruction pointer is. The instruction pointer called EIP (32bit binary) or RIP (64 bit binary) hold the address of the next instruction. Let's have an exemple with our code and GDB.
![[Pasted image 20251204125609.png]]


We don't really need to understand everything, the crucial part is to see that with gdb we can run the program instruction by instruction. Here I 'pause' the program a little bit before the gets() call and we see that the address in eip is indeed the next address to be executed.

Now that we know how EIP works, let's try to take a look of his value if we overflow the buffer.
![[Pasted image 20251204130210.png]]

So this is the state of the program just after sending a bunch of `AAAAAAAAAAAAAAAAA...`
If we take a look to the next instruction address to be executed, EIP, we see that it contains AAAA.

This means that we control the next instruction address with our input !

Now we would like to know exactly the number of character we have to write before EIP.
This can be done with gdb with the gef plugin I use.

1. Create a long enough pattern
![[Pasted image 20251204130752.png]]

2. Send this input to the buffer and look EIP
![[Pasted image 20251204130924.png]]

3. We see that EIP contain `taaa` this pattern is unique in our string we send, we could try to find this pattern our string to know exactly when we write in EIP or just use the gef plugin:
   ![[Pasted image 20251204131113.png]]
   
We know that we need to write 73 character before writing EIP.

Next step is to know what to write in EIP, there is a lot of methods more or less hard but we have luck, a function already exist in the code that spawn a shell so let's use it.
The only thing we need is 
1. to find the address of the run() function
2. Write 73 random character before eip
3. append the address of run to this string so it will be written in eip and will then execute the function.

An easy way to find the address of run() here is with objdump (CLI disassembler):
![[Pasted image 20251204131558.png]]

We found that the address of run is 0x08048444

So we would like to construct this string in python:
```bash
python2 -c 'print "A"*76 + "\x44\x84\x04\x08"'
```

The bytes order of the address has to be inverted (See little-endian/ big-endian)  
We can then send the output of this string directly to the program like that
```bash
python2 -c 'print "A"*76 + "\x44\x84\x04\x08"' | ./test
```
PS: we cannot just copy paste the ouput of the print function because there are none printable character in it (Like 0x84).

Last little trick in order to make it work because right now we wouldn't have an interactive shell cuz bash pipe is doing shit.

Final payload:
```bash
(python2 -c 'print "A"*76 + "\x44\x84\x04\x08"' ; cat) | ./test
```

![[Pasted image 20251204132253.png]]