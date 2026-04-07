
/*

Пример для курса ФИЛП, УрФУ, 2017-2018. Веретенников А. Б.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/*
	H - uint32_t -> BitsPerLevel = 5 -> Maximum Children Nodes = 2^5 = 32.
*/
template <class K, class V, class H, class N = uint32_t, size_t BitsPerLevel = 5>
class CHashArrayMappedTrie
{
	struct CNode;

	struct CBitmapIndexedNode;

	struct CMultiLeafNode;

	typedef shared_ptr<CNode> PNode;

	enum { NodeSize = 1 << BitsPerLevel };
	
	struct CNode
	{
		virtual ~CNode()
		{

		}

		CMultiLeafNode& AsMultiLeafNode()
		{
			return *(CMultiLeafNode*)this;
		}

		CBitmapIndexedNode& AsBitmapIndexedNode()
		{
			return *(CBitmapIndexedNode*)this;
		}

		virtual int Type()
		{
			return -1;
		}

		virtual void Dump(size_t Level) = 0;

		virtual size_t Height() = 0;

		virtual size_t ValueHeight() = 0;
	};

#ifdef __GNUC__
	static unsigned int popcnt(unsigned int Value)
	{
		return __builtin_popcount(Value);
	}
	
	static unsigned int popcnt(unsigned long Value)
	{
		return __builtin_popcountl(Value);
	}

	static unsigned long long popcnt(unsigned long long Value)
	{
		return __builtin_popcountll(Value);
	}
#else
	static unsigned int popcnt(unsigned int Value)
	{
		return __popcnt(Value);
	}

	static unsigned __int64 popcnt(unsigned __int64  Value)
	{
		return __popcnt64(Value);
	}
#endif

	struct CBitmapIndexedNode: public CNode
	{
		N Bitmap;
		PNode* Array;

		N GetIndexFromBit(N Bit)
		{
			Bit--;
			N NewBitmap = Bitmap & Bit;
			N Index = popcnt(NewBitmap);
			return Index;
		}

		PNode Get(N Mask)
		{
			N Bit = 1 << Mask;
			if ((Bitmap & Bit) == 0)
			{
				return PNode();
			}
			N Index = GetIndexFromBit(Bit);
			return Array[Index];
		}
		
		N GetIndex(N Mask)
		{
			N Bit = 1 << Mask;
			return GetIndexFromBit(Bit);
		}

		PNode Put(H Mask, PNode Child)
		{
			return PNode(new CBitmapIndexedNode(Bitmap, Array, Mask, Child));
		}

		CBitmapIndexedNode(N SourceBitmap, PNode* SourceArray, H Mask, PNode NewChild)
		{
			N CurrentCount = popcnt(SourceBitmap);
			/*
				Updating an existing node.
			*/
			if (SourceBitmap & (1 << Mask))
			{
				Bitmap = SourceBitmap;
				Array = new PNode[CurrentCount];
				N Index = GetIndex(Mask);
				for (N i = 0; i < Index; i++)
					Array[i] = SourceArray[i];
				Array[Index] = NewChild;
				for (N i = Index + 1; i < CurrentCount; i++)
					Array[i] = SourceArray[i];
			}
			else
			{
				Bitmap = SourceBitmap | (1 << Mask);
				Array = new PNode[CurrentCount + 1];
				N Index = GetIndex(Mask);

				for (N i = 0; i < Index; i++)
					Array[i] = SourceArray[i];
				Array[Index] = NewChild;
				for (N i = Index; i < CurrentCount; i++)
					Array[i + 1] = SourceArray[i];
			}
		}

		CBitmapIndexedNode(H Mask, PNode Child)
		{
			Array = new PNode[1];
			Array[0] = Child;
			Bitmap = 1 << Mask;
		}

		~CBitmapIndexedNode()
		{
			delete[] Array;
		}

		CBitmapIndexedNode(const CBitmapIndexedNode& Other) = delete;

		CBitmapIndexedNode& operator=(const CBitmapIndexedNode& Other) = delete;

		int Type() override
		{
			return 0;
		}

		/*
			Only for Debug.
			Assume, K, V - some integer types.
		*/
		virtual void Dump(size_t Level) override
		{
			for (size_t i = 0; i < Level; i++)
				printf(" ");

			N Count = popcnt(Bitmap);
			printf("Bitmap %d\r\n",(int)Count);

			N Mask = Bitmap;
			N Index = 0;
			for (size_t i = 0; i < (1 << BitsPerLevel); i++)
			{
				if (Mask & 1)
				{
					for (size_t i = 0; i <= Level; i++)
						printf(" ");
					printf("Link Bit %d, Index %d\r\n", (int)i, (int)Index);
					Array[Index++]->Dump(Level + 1);
				};
				Mask >>= 1;
			}

			/*
			for (size_t i = 0; i < Count; i++)
			{
				Array[i]->Dump(Level + 1);
			}*/
		}

		virtual size_t Height() override
		{
			size_t R = 0;
			N Count = popcnt(Bitmap);
			for (size_t i = 0; i < Count; i++)
			{
				R = max(R, Array[i]->Height());
			}
			return R + 1;
		}

		virtual size_t ValueHeight()
		{
			size_t R = 0;
			N Count = popcnt(Bitmap);
			for (size_t i = 0; i < Count; i++)
			{
				R = max(R, Array[i]->ValueHeight());
			}
			return R + 1;
		}
	};

	struct CMultiLeafNode : public CNode
	{
		CList<pair<K, V>> List;

		CMultiLeafNode(K Key, V Value)
		{
			List = List.PushFront(make_pair(Key, Value));
		}

		CMultiLeafNode(CList<pair<K, V>> AList) : List(AList)
		{

		}

		PNode ConvertToBitmap()
		{
			return make_shared<CBitmapIndexedNode>(0, make_shared<CMultiLeafNode>(List));
		};

		PNode Put(K Key, V Value)
		{
			if (List.Filter([Key](pair<K,V> Item) { 
				return Item.first == Key; 
				}).IsEmpty())
			{
				return PNode(new CMultiLeafNode(List.PushFront(make_pair(Key, Value))));
			}
			else
			{
				return PNode(new CMultiLeafNode(List.template Map<pair<K, V> >([Key, Value](pair<K, V> Pair) {
						if (Pair.first == Key)
						{
							return make_pair(Key, Value);
						}
						else
						{
							return Pair;
						}
					})));
			}
		}

		int Type() override
		{
			return 1;
		}

		virtual void Dump(size_t Level) override
		{
			for (size_t i = 0; i < Level; i++)
				printf(" ");
			printf("MultiLeaf %d\r\n", (int)List.Count());
			List.ForEach([Level](pair<K,V> Pair)
			{
				for (size_t i = 0; i <= Level; i++)
					printf(" ");
				printf("K/V %d, %d\r\n", (int)Pair.first, (int)Pair.second);
				return true;
			});
		}

		virtual size_t Height() override
		{
			return 1;
		}

		virtual size_t ValueHeight() override
		{
			return List.Count();
		}
	};

	PNode Node;
	
	H Mask(H Key) const
	{
		return Key & (NodeSize - 1);
	}

	H Shift(H Key) const
	{
		return Key >> BitsPerLevel;
	}

	PNode Insert(PNode Node, H Hash, K Key, V Value) const
	{
		H HashMask = Mask(Hash);

		if (Node)
		{
			if (Node->Type() == 0)
			{
				PNode Child = Node->AsBitmapIndexedNode().Get(HashMask);

				PNode NextNode = Insert(Child, Shift(Hash), Key, Value);

				return Node->AsBitmapIndexedNode().Put(HashMask, NextNode);
			}
			else
			{
				if (Hash == 0)
				{
					return Node->AsMultiLeafNode().Put(Key, Value);
				}
				else
				{
					PNode Result = Node->AsMultiLeafNode().ConvertToBitmap();
					return Insert(Result, Hash, Key, Value);
				}
			}
		}
		else
		{
			if (Hash == 0)
			{
				return make_shared<CMultiLeafNode>(Key, Value);
			}

			return PNode(new CBitmapIndexedNode(HashMask, Insert(PNode(), Shift(Hash), Key, Value)));
		}
	}

	CHashArrayMappedTrie(PNode ANode, function<H(K)> AHashFunction) : Node(ANode), HashFunction(AHashFunction)
	{

	}

	function<H(K)> HashFunction;

public:

	CHashArrayMappedTrie()
	{

	}

	CHashArrayMappedTrie Put(K Key, V Value, H Hash)
	{
		return CHashArrayMappedTrie(Insert(Node, Hash, Key, Value), HashFunction);
	}

	pair<bool,V> Get(K Key, H Hash) const
	{
		PNode Current = Node;

		while (true)
		{
			H CurrentMask = Mask(Hash);
			
			if (!Current)
			{
				return make_pair(false, V());
			}

			if (Current->Type() == 0)
			{
				Current = Current->AsBitmapIndexedNode().Get(CurrentMask);
			}
			else
			{
				if (CurrentMask != 0)
				{
					return make_pair(false, V());
				}
				
				CList<pair<K,V>> List = Current->AsMultiLeafNode().List.Filter([Key](pair<K, V> Pair) {
					return Pair.first == Key;
					});
								
				if (List.IsEmpty())
				{
					return make_pair(false, V());
				}
				return make_pair(true, List.Front().second);
			}

			Hash = Shift(Hash);
		}
	}


	void SetHashFunction(function<H(K)> Value)
	{
		HashFunction = Value;
	}

	CHashArrayMappedTrie Put(K Key, V Value)
	{
		H Hash = HashFunction(Key);
		return CHashArrayMappedTrie(Insert(Node, Hash, Key, Value), HashFunction);
	}

	pair<bool, V> Get(K Key) const
	{
		H Hash = HashFunction(Key);
		return Get(Key, Hash);
	}

	void Dump()
	{
		printf("\r\nTree:\r\n");
		if (Node)
			Node->Dump(0);
	}

	size_t Height() const
	{
		if (Node)
			return Node->Height();
		return 0;
	}

	size_t ValueHeight() const
	{
		if (Node)
			return Node->ValueHeight();
		return 0;
	}
};

class CHashArrayMappedTrieTest
{
public:

	void Test3()
	{
		CHashArrayMappedTrie<uint32_t, uint32_t, uint32_t, uint32_t, 5> Tree;

		Tree = Tree.Put(1, 100, 1);
		Tree.Dump();

		Tree = Tree.Put(0, 10, 0);
		Tree.Dump();

		Tree = Tree.Put(32, 200, 32);
		Tree.Dump();
	}
	void Test2()
	{
		CHashArrayMappedTrie<uint32_t, uint32_t, uint32_t, uint32_t, 5> Tree;
		Tree.SetHashFunction([](uint32_t Value) {
			return Value % 1777;
		});
		
		int Check = 2000;

		for (int i = 0; i < Check; i++)
		{
			Tree = Tree.Put(i, i);
		}

		printf("Tree height %d, value height %d\r\n", (int)Tree.Height(), (int)Tree.ValueHeight());

		for (int i = 0; i < Check; i++)
		{
			pair<bool,uint32_t> R = Tree.Get(i);
			if (!R.first)
			{
				printf("Search error %d\r\n", (int)(i));
				break;
			}
			if (R.second != (uint32_t)i)
			{
				printf("Search value error %d\r\n", (int)(i));
				break;
			}
		}

		printf("Test N 2 has been completed\r\n");
	}

	void Test()
	{		
		printf("Hash Array Mapped Trie\r\n");

		CHashArrayMappedTrie<uint32_t, uint32_t, uint32_t, uint32_t, 5> Tree;

		int Check = 20000;

		for (int i = 0; i < Check; i+=3)
		{
			Tree = Tree.Put(i, i*2, i);
			Tree = Tree.Put(i+1, (i+1)*2, i);
			Tree = Tree.Put(i+2, (i + 2) * 2, i);
			//Tree.Dump();
		}
				
		printf("Get\r\n");

		pair<bool, uint32_t> R = Tree.Get(10, 10);
		printf("%d, %d\r\n", (int)R.first, (int)R.second);
		R = Tree.Get(0, 0);
		printf("%d, %d\r\n", (int)R.first, (int)R.second);

		//Tree.Dump();

		for (int i = 0; i < Check; i+=3)
		{
			pair<bool, uint32_t> R = Tree.Get(i, i);
			if (!R.first)
			{
				printf("Search error %d\r\n", (int)i);
				break;
			}
			if (R.second != (uint32_t)(i*2))
			{
				printf("Search value error %d\r\n", (int)i);
				break;
			}

			R = Tree.Get(i+1, i);
			if (!R.first)
			{
				printf("Search error %d\r\n", (int)(i+1));
				break;
			}
			if (R.second != (uint32_t)((i + 1) * 2))
			{
				printf("Search value error %d\r\n", (int)(i + 1));
				break;
			}

			R = Tree.Get(i + 2, i);
			if (!R.first)
			{
				printf("Search error %d\r\n", (int)(i + 2));
				break;
			}
			if (R.second != (uint32_t)((i + 2) * 2))
			{
				printf("Search value error %d\r\n", (int)(i + 2));
				break;
			}
		}

		printf("Test has been completed\r\n");

		Test2();
		Test3();
	}

};
