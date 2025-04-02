
#include <cstdint>
#include <cstdio>
#include <limits>

using namespace std;

/*
 * Аналог функций BitScanForward, BitScanReverse для Linux.
 * 
 */
 
/*
 * https://gcc.gnu.org/onlinedocs/gcc-7.2.0/gcc/Other-Builtins.html
 
 Built-in Function: int __builtin_ffs (int x)

    Returns one plus the index of the least significant 1-bit of x, or if x is zero, returns zero. 

Built-in Function: int __builtin_clz (unsigned int x)

    Returns the number of leading 0-bits in x, starting at the most significant bit position. If x is 0, the result is undefined. 

Built-in Function: int __builtin_ctz (unsigned int x)

    Returns the number of trailing 0-bits in x, starting at the least significant bit position. If x is 0, the result is undefined. 

Built-in Function: int __builtin_clrsb (int x)

    Returns the number of leading redundant sign bits in x, i.e. the number of bits following the most significant bit that are identical to it. There are no special cases for 0 or other values. 

Built-in Function: int __builtin_popcount (unsigned int x)

    Returns the number of 1-bits in x. 
    
 * */

uint32_t BitScanForward(uint32_t Val)
{
	if (!Val)
		return 0;
	return __builtin_ffsll(Val);
}

uint32_t BitScanReverse(uint32_t Val)
{
	if (!Val)
		return 0;
	return sizeof(Val)*8 - __builtin_clz(Val);
}

void Print(uint32_t Val)
{
 uint32_t BSF = BitScanForward(Val);
 uint32_t BSR = BitScanReverse(Val);

 printf("val %u, BSF %d, BSR %d\n", Val, BSF, BSR);
}

int main(int argc, char** argv)
{
 
 Print(64);
 Print(64+4);
 Print(numeric_limits<uint32_t>::max());
 Print(0);

}
