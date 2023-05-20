#pragma once

template <class T>
class CArray
{
	T* Value;
	size_t Count;

public:

	CArray(const CArray& Other) = delete;

	CArray& operator=(const CArray& Other) = delete;

	CArray(size_t ACount, T DefaultValue)
	{
		Count = ACount;
		Value = new T[Count];
		for (size_t i = 0; i < Count; i++)
		{
			Value[i] = DefaultValue;
		}
	}

	~CArray()
	{
		delete[] Value;
	}

	T operator[](size_t Index) const
	{
		return Value[Index];
	}

	T& operator[](size_t Index)
	{
		return Value[Index];
	}

	size_t GetDataSize() const
	{
		return Count * sizeof(T);
	}

};

template <>
class CArray<bool>
{
	unsigned char* Value;
	size_t Count;

public:

	CArray(const CArray& Other) = delete;

	CArray& operator=(const CArray& Other) = delete;

	size_t CalculateDataSize() const
	{
		size_t BitmapByteSize = (Count / 8) + 1;
		return BitmapByteSize;
	}

	CArray(size_t ACount, size_t DefaultValue = 0)
	{
		Count = ACount;
		size_t BitmapByteSize = CalculateDataSize();

		Value = new unsigned char[BitmapByteSize];

		if (DefaultValue)
		{
			memset(Value, (unsigned char)0xFF, BitmapByteSize);
		}
		else
		{
			memset(Value, 0, BitmapByteSize);
		}
	}

	~CArray()
	{
		delete[] Value;
	}

	class Reference
	{
		size_t Index;
		CArray* Owner;
	public:

		Reference(CArray* AOwner, size_t AIndex): Owner(AOwner), Index(AIndex)
		{

		}

		operator bool() const
		{
			return Owner->Get(Index);
		}

		Reference& operator=(bool Value)
		{
			Owner->Set(Index, Value);
			return *this;
		}

		Reference& operator=(const Reference Value)
		{
			Owner->Set(Index, (bool)Value);
			return *this;
		}
	};
	
	Reference operator[](size_t Index)
	{
		return Reference(this, Index);
	}

	void Set(size_t Index, bool V)
	{
		size_t BitIndex = Index & 7;
		size_t ByteIndex = Index >> 3;
		char Byte = Value[ByteIndex];
		if (V)
			Byte |= 1 << BitIndex; else
			Byte &= ~(1 << BitIndex);
		Value[ByteIndex] = Byte;
	}

	bool Get(size_t Index) const
	{
		size_t BitIndex = Index & 7;
		size_t ByteIndex = Index >> 3;
		char Byte = Value[ByteIndex];
		return (Byte & (1 << BitIndex)) != 0;
	}

	size_t GetDataSize() const
	{
		return CalculateDataSize();
	}

};

template <class T>
class CArrayTest
{
public:

	void Test()
	{
		size_t Elements = 10;
		CArray<T> Array(Elements, 0);
		printf("%d\r\n", (int)(Array[5] == 1));
		Array[5] = 1;
		printf("%d\r\n", (int)(Array[5] == 1));
		printf("Elements %d, data size %d\r\n", (int)Elements, (int)Array.GetDataSize());
	}
};

template <int N>
struct CFactorial
{
	enum : uint64_t { Value = CFactorial<N - 1>::Value * N };
};

template <>
struct CFactorial<1>
{
	enum : uint64_t {Value = 1};
};

template <uint64_t N>
struct CBitCount
{
private:

	template <uint64_t N, int I>
	struct CBitCount1
	{
		enum { Value1 = N & (1 << I) ? 1 : 0 };
		enum { Value = Value1 + CBitCount1<N, I - 1>::Value };
	};

	template <uint64_t N>
	struct CBitCount1<N, -1>
	{
		enum { Value = 0 };
	};

public:

	enum {Value = CBitCount1<N, sizeof(N)*4 - 1>::Value};
};

template <int i>
class Loop
{
public:

	template <class F>
	static void Run(F f)
	{
		Loop<i - 1>::Run(f);
		f(i);
	}
};


template <>
class Loop<0>
{
public:

	template <class F>
	static void Run(F f)
	{

	}
};

/*
	Helper functions
*/

void HandleValue(int P)
{
	printf("%d ", P);
}

void HandleValue(float P)
{
	printf("%2.3f ", P);
}

void HandleValue(const char* P)
{
	printf("%s ", P);
}

void HandleValue(const string& P)
{
	printf("%s ", P.c_str());
}


//Tuple Print, the first version

template <class T, int n = tuple_size<T>::value>
struct CPrintTuple
{
	static void Print(const T& t)
	{
		CPrintTuple<T, n - 1>::Print(t);
		auto Val = get<n - 1>(t);
		HandleValue(Val);
	}
};

template <class T>
struct CPrintTuple<T, 0>
{
	static void Print(const T& t)
	{

	}
};

//Tuple Print, the second version

template <class T, int i = tuple_size<T>::value>
void TuplePrintHelper(const T& t)
{
	if constexpr (i > 1)
	{
		TuplePrintHelper<T, i-1>(t);
	}
	HandleValue(get<i - 1>(t));
}


void TemplateRecursionTest()
{
	printf("Factorial 20 = %I64d\r\n", (uint64_t)CFactorial<20>::Value);
	printf("BitCount 15 = %I64d\r\n", (uint64_t)CBitCount<15>::Value);
	Loop<3>::Run([](int i) { 
		printf("%d\n", i);
	}
	);
	
	printf("\r\n");
	printf("PrintTuple\r\n");

	tuple<int, string, float> t = { 16, "Str", 12.3f };
	CPrintTuple<decltype(t)>::Print(t);

	printf("\r\n");
	printf("PrintTuple\r\n");

	TuplePrintHelper(t);
			
	printf("\r\n");
};

