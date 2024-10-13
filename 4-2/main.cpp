// Топ K пользователей из лога.
// Имеется лог-файл, в котором хранятся пары для N пользователей (Идентификатор пользователя, посещаемость сайта).
// Напишите программу, которая выбирает K пользователей, которые чаще других заходили на сайт, 
// и выводит их в порядке возрастания посещаемости. Количество заходов и идентификаторы пользователей не повторяются.

#include <cassert>
#include <iostream>

// Динамический массив
template<typename T>
class CArray {
public:
    CArray() : buffer( 0 ), bufferSize( 0 ), realSize( 0 ) {}
    ~CArray() { delete[] buffer; }

    CArray( const CArray& arr )
    {
        bufferSize = arr.bufferSize;
        realSize = arr.realSize;
        buffer = new T[bufferSize];

        for ( int i = 0; i < arr.Size(); ++i )
        {
            buffer[i] = arr[i];   
        }    
    }

    CArray& operator=( const CArray& arr )
    {
        delete[] buffer;

        bufferSize = arr.bufferSize;
        realSize = arr.realSize;
        buffer = new T[bufferSize];

        for ( int i = 0; i < arr.Size(); ++i )
        {
            buffer[i] = arr[i];   
        } 

        return *this; 

    }
    
    int Size() const { return realSize; }

    const T GetAt( int index ) const
    {
        assert( index >= 0 && index < realSize && buffer != 0 );
        return buffer[index];
    }
    const T operator[]( int index ) const { return GetAt( index ); }

    T& operator[]( int index )
    {
        assert( index >= 0 && index < realSize && buffer != 0 );
        return buffer[index];
    }

    const T Last() const
    {
        assert( !IsEmpty() );
        return buffer[realSize - 1];
    }

    void Add( T element )
    {
        if( realSize == bufferSize )
            grow();
        assert( realSize < bufferSize && buffer != 0 );
        buffer[realSize++] = element;
    }

    void DeleteLast()
    {
        assert( !IsEmpty() && buffer != 0 );
        --realSize;
    }

    const bool IsEmpty() const
    {
        return realSize == 0;
    }

private:
    T* buffer; 
    int bufferSize;
    int realSize;
    int DefaultInitialSize = 16;

    void grow()
    {
        int newBufferSize = ( bufferSize > 0 ) ? bufferSize * 2 : DefaultInitialSize;
        T* newBuffer = new T[newBufferSize];

        for( int i = 0; i < realSize; ++i )
            newBuffer[i] = buffer[i];
        delete[] buffer;
        buffer = newBuffer;
        bufferSize = newBufferSize;
    }
};

struct UserPair
{
    int id;
    int activity;

    const bool operator<(const UserPair& up) const
    {
        return activity < up.activity;
    }

    const bool operator>(const UserPair& up) const
    {
        return activity > up.activity;
    }
};

template <typename T>
struct DefaultComp
{
    const bool operator()(const T& l, const T& r) const
    {
        return l < r;
    }
};

// Двоичная куча
template <typename T, typename Comp = DefaultComp<T>>
class Heap {
public:
    Heap(Comp _comp = Comp()): arr(), comp(_comp) {}
    ~Heap() {}

    explicit Heap( const CArray<T>& _arr, Comp _comp = Comp()): arr(_arr), comp(_comp)
    {
        buildHeap();
    }

    void Insert( T element )
    {
        arr.Add( element );
        if (arr.Size() > 1)
        {
            siftUp( arr.Size() - 1 );
        }
    }

    const T ExtractMin()
    {
        assert( !arr.IsEmpty() );
        T result = arr[0];
        arr[0] = arr.Last();
        arr.DeleteLast();
        if( !IsEmpty() )
        {
            siftDown( 0 );
        }
        return result;
    }

    const T PeekMin() const
    {
        assert( !IsEmpty() );
        return arr[0];
    }

    const bool IsEmpty() const
    {
        return arr.IsEmpty();
    }

private:
    CArray<T> arr;

    Comp comp;

    void buildHeap()
    {
        for( int i = arr.Size() / 2 - 1; i >= 0; --i )
        {
            siftDown( i );
        }
    }
    
    void siftUp( int i )
    {
        while( i > 0 ) {
            int parent = ( i - 1 ) / 2;
            if( !comp( arr[i], arr[parent] ) )
                return;
            std::swap( arr[i], arr[parent] );
            i = parent;
        }
    }

    void siftDown( int i ) 
    {
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        int smallest = i;
        if( left < arr.Size() && comp( arr[left], arr[i] ) )
            smallest = left;

        if( right < arr.Size() && comp(arr[right], arr[smallest] ) )
            smallest = right;

        if( smallest != i ) {
            std::swap( arr[i], arr[smallest] );
            siftDown( smallest );
        }
    }
};

void printTopUsers(int n, int k)
{
    CArray<UserPair> arr;
    int id = 0, activity = 0;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> id >> activity;
        arr.Add({id, activity});
    }

    Heap<UserPair> heap;
    for (int i = 0; i < k; ++i)
    {
        heap.Insert(arr[i]);
    }

    for (int i = k; i < n; ++i)
    {
        if (arr[i] > heap.PeekMin())
        {
            heap.ExtractMin();
            heap.Insert(arr[i]);
        }
    }

    while (!heap.IsEmpty())
    {
        std::cout << heap.ExtractMin().id << " ";        
    }
    
}

int main()
{
    int n = 0, k = 0;
    std::cin >> n >> k;
    printTopUsers(n, k);   

    return 0;
};

