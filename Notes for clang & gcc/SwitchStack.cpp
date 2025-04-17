
#include <cstdio>
#include <cstdint>
#include <iostream>

/**
 * Выделяем новый буфер и переключаем стек на него.
 * Вроде даже работает (GCC & Ubuntu).
 */
using namespace std;

uint64_t StartSP;
uint64_t LowerSP;
uint64_t f1(uint64_t n)
{
	if (n <= 0) 
	{
		asm volatile ("mov %%rsp, %0" : "=r" (LowerSP) );			
		
		return 1;
	}
	return n + f1(n-1);
};

void Test(uint64_t Num, uint64_t StartSP)
{
	uint64_t val = f1(Num);
    cout << "f1(" << Num << "): " << val << endl;
    cout << "Stack usage: " << StartSP - LowerSP << endl;
}

/*
 * It seems gcc & linix have 8MB default stack size.
 * 
 * https://unix.stackexchange.com/questions/473416/why-on-modern-linux-the-default-stack-size-is-so-huge-8mb-even-10-on-some-di
 * 
 * To get SP:
 
 * asm volatile ("mov %%rsp, %0" : "=r" (InitialSP) );
 * 
 * To set SP:
 * 
 * asm volatile ("mov %0, %%rsp" : : "r" (InitialSP) );
 * 
 * https://stackoverflow.com/questions/34489874/c-read-and-set-assembler-registers
 * 
 * https://stackoverflow.com/questions/10222899/how-to-print-64-bit-integer-in-gcc-4-4-1
 * 
 * https://stackoverflow.com/questions/2114163/reading-a-register-value-into-a-c-variable
 * 
 * https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
 * 
 * https://www.opennet.ru/base/dev/gccasm.txt.html
 * 
 * https://avrdudes.github.io/avr-libc/avr-libc-user-manual-2.2.0/inline_asm.html
 * 
 */
int main(int args, char** argv)
{
	uint64_t InitialSP = 0;
	asm volatile ("mov %%rsp, %0" : "=r" (InitialSP) );
		
	cout << "Current SP: " << InitialSP << endl;
		
    Test(200000, InitialSP);
    
    size_t BufferSize = 1024*1024*64;
    char* Buffer = new char[BufferSize];
    
    cout << "New Stack Size: " << BufferSize << endl;
    cout << "Switch Stack" << endl;
    
    asm volatile ("mov %0, %%rsp" : : "r" (Buffer + BufferSize) );
    
    uint64_t NewSP = 0;
    asm volatile ("mov %%rsp, %0" : "=r" (NewSP) );
    cout << "Buffer End: " << (uintptr_t)(Buffer + BufferSize) << endl;
    cout << "New SP    : " << NewSP << endl;
    
    Test(2000000, NewSP);

    cout << "Restore Stack" << endl;
    asm volatile ("mov %0, %%rsp" : : "r" (InitialSP) );

    delete[] Buffer;

	cout << "Completed" << endl;
	
	return 0;
}
