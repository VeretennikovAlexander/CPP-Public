// FunctionalDataStructures.cpp : Defines the entry point for the console application.
//

#ifdef __GNUC__
#else
#include "stdafx.h"
#endif

#include <string>
#include <functional>
#include <memory>
#include <algorithm>
#include <vector>

#ifdef __GNUC__
#else
#include <intrin.h>
#endif

using namespace std;

#include "List.h"
#include "Queue.h"
#include "RandomAccessList.h"
#include "BinominalHeap.h"
#include "BinaryNumber.h"
#include "Array.h"
#include "HashArrayMappedTrie.h"
#include "BinaryTree.h"

int main()
{	
	CListTest Test;
	Test.Test();

	CQueueTest QueueTest;
	QueueTest.Test();

	CRandomAccessListTest RandomAccessListTest;
	RandomAccessListTest.Test();

	CBinominalHeapTest BinominalHeapTest;
	BinominalHeapTest.Test();

	CBinaryNumberTest BinaryNumberTest;
	BinaryNumberTest.Test();

	CArrayTest ArrayTest;
	ArrayTest.Test();

	CHashArrayMappedTrieTest HashArrayMappedTrieTest;
	HashArrayMappedTrieTest.Test();

	CBinaryTreeTest BinaryTreeTest;
	BinaryTreeTest.Test();
	
//	getchar();
    return 0;
}

