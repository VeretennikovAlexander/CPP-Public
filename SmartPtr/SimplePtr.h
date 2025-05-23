﻿
/*

Пример для курса ФИЛП, УрФУ, 2017-2025. Веретенников А. Б.

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
 * Базовый пример 
 * 
 * 1) Определение операторов -> и *
 * 2) excplicit конструктор
 * 3) Удаление вложенного объекта в деструкторе.
 * 4) Запрет копирования.
 * 
 */

template <class T>
class CSimplePtr
{
	T* Ptr;

public:

	CSimplePtr(const CSimplePtr& Other) = delete;

	CSimplePtr& operator=(const CSimplePtr& Other) = delete;

	explicit CSimplePtr(T* V) : Ptr(V)
	{

	}

	~CSimplePtr()
	{
		if (Ptr) delete Ptr;
	}

	T* operator->() const
	{
		return Ptr;
	}

	T operator*() const
	{
		return *Ptr;
	}
};

void SimplePtrTest()
{
	CSimplePtr<int> Ptr(new int());
	//CSimplePtr<int> Ptr2 = Ptr;

	int q = *Ptr;
}
