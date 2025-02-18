
#include <cstdint>
#include <map>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

class CStringComparator
{
public:

	/*
		A < B --> true
	*/
	bool operator()(const char* A, const char* B) const
	{
		while (true)
		{
			if (A[0] == B[0])
			{
				//A = B
				if (!A[0])
					return false;

				A++;
				B++;
			}
			else
			{
				return A[0] < B[0];
			}
		}
	}
};

class CCustomAllocator
{
public:
	
	void* Alloc(size_t Size)
	{
		return malloc(Size);
	}
	
	void Free(void* Ptr)
	{
		free(Ptr);
	}
};

CCustomAllocator CustomAllocator;

template <class T>
class CMyAllocator
{
public:
	using value_type = T;

	CMyAllocator()
	{

	}

	template <class U>
	CMyAllocator(const CMyAllocator<U> &V)
	{

	}

	T* allocate(size_t Count)
	{
		//printf("Allocate %d\n", (int)(Count * sizeof(T)));

		return (T*)CustomAllocator.Alloc(sizeof(T) * Count);
	}

	void deallocate(T* V, size_t Count)
	{
		//printf("Free %d\n", (int)(Count * sizeof(T)));

		CustomAllocator.Free(V);
	}
};

void TextMapTest()
{
	map<const char*, size_t, CStringComparator, CMyAllocator<pair<const char* const, size_t> >> Map;

	const char* Words[] = { "Who", "Are", "You", "Who" };
	const char* Word;

	for (size_t i = 0; i < sizeof(Words) / sizeof(Words[0]); i++)
	{
		Word = Words[i];
		auto It = Map.find(Word);
		if (It == Map.end())
		{
			Map.insert(make_pair(Word, 1));
		}
		else
		{
			It->second++;
		}
	}

	for (auto Entry : Map)
	{
		printf("Word %s, count %llu\n", Entry.first, (unsigned long long)Entry.second);
	}

}

#include <chrono>

uint64_t GetCurrentTimeMs() {
    auto now = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
}



int main()
{
	uint64_t Start = GetCurrentTimeMs();

	TextMapTest();

	uint64_t End = GetCurrentTimeMs();

	printf("Time (ms) %d\n", (int)(End - Start));
	
	printf("Press any key\r\n");

	getchar();

    return 0;
}


