int a = 1;

int func1(int a)
{
	return a + 1;
}

int main()
{
	int b = 1;
	a = func1(a);
	int c;
	c = a + b;
	return 0;
}

// riscv64-unknown-elf-gcc -S a.s -o a.s