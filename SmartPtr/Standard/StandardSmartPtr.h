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


#pragma once

class CWeakPtrTester
{
	class CTree;

	class CLeaf
	{
		int Value;
		weak_ptr<CTree> Parent;
	public:
		CLeaf(const CLeaf& Tree) = delete;
		CLeaf& operator=(const CLeaf& Tree) = delete;

		CLeaf(int AValue, shared_ptr<CTree> AParent)
		{
			Value = AValue;
			Parent = AParent;

			printf("Leaf has been created %d\r\n", Value);
		}

		~CLeaf()
		{
			printf("Leaf has been destroyed %d\r\n", Value);
		}
	};

	class CTree
	{
		vector<shared_ptr<CLeaf>> Leafs;
	public:
		CTree(const CTree& Tree) = delete;
		CTree& operator=(const CTree& Tree) = delete;

		CTree()
		{
			printf("Tree has been created\r\n");
		}

		~CTree()
		{
			printf("Tree has been destroyed\r\n");
		}

		void Add(shared_ptr<CLeaf> Leaf)
		{
			Leafs.push_back(Leaf);
		}

		shared_ptr<CLeaf> Get(size_t Index)
		{
			return Leafs[Index];
		}

	};

public:

	void Test()
	{
		weak_ptr<CLeaf> Leaf;

		{
			shared_ptr<CTree> Tree(new CTree());
			Tree->Add(make_shared<CLeaf>(10, Tree));
			Tree->Add(make_shared<CLeaf>(11, Tree));

			Leaf = Tree->Get(0);

			shared_ptr<CLeaf> Ptr = Leaf.lock();
			printf("Has ptr %d\r\n", Ptr != 0);
		}

		shared_ptr<CLeaf> Ptr = Leaf.lock();
		printf("Has ptr %d\r\n", Ptr != 0);
	}
};

class CCustomDeleterTester
{
public:

	void Test()
	{
		shared_ptr<int> F1((int*)malloc(sizeof(int)), [](int* p) {
			printf("Custom Deleter\r\n");
			free(p);
		});
	}
};


class CWeakPtrCacheTester
{

	class CFile
	{
		wstring FileName;
	public:

		CFile(wstring AFileName): FileName(AFileName)
		{
			printf("Open file %S\r\n", FileName.c_str());

			//Reading the file into memory.
		}

		~CFile()
		{
			printf("Close file %S\r\n", FileName.c_str());
		}
	};

	shared_ptr<CFile> GetFile(wstring FileName)
	{
		static map<wstring, weak_ptr<CFile> > Cache;
		static mutex m;

		lock_guard<mutex> lock(m);
		auto sp = Cache[FileName].lock();
		if (!sp)
		{
			Cache[FileName] = sp = make_shared<CFile>(FileName);
		}

		return sp;
	}

public:

	void Test()
	{
		{
			auto File = GetFile(L"1.txt");
			auto File1 = GetFile(L"1.txt");
		}

		printf("New block\r\n");

		auto File2 = GetFile(L"1.txt");
	}
};

class CStandardSmartPtrTester
{
public:

	void Test()
	{
		CWeakPtrTester WeakPtrTester;
		WeakPtrTester.Test();

		CCustomDeleterTester CustomDeleterTester;
		CustomDeleterTester.Test();

		CWeakPtrCacheTester WeakPtrCacheTester;
		WeakPtrCacheTester.Test();
	}
};
