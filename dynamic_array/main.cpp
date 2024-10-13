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

    T GetAt( int index ) const
    {
        assert( index >= 0 && index < realSize && buffer != 0 );
        return buffer[index];
    }
    T operator[]( int index ) const { return GetAt( index ); }
    T& operator[]( int index )
    {
        assert( index >= 0 && index < realSize && buffer != 0 );
        return buffer[index];
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
        assert( realSize > 0 && buffer != 0 );
        --realSize;
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

void testArray()
{
    // Тест 1: Создание объекта и добавление элементов
    CArray<int> arr;
    arr.Add(10);
    arr.Add(20);
    arr.Add(30);

    std::cout << "Size of the array: " << arr.Size() << std::endl; // Ожидается: 3

    // Тест 2: Проверка доступа к элементам
    std::cout << "Elements of the array: ";
    for (int i = 0; i < arr.Size(); ++i) {
        std::cout << arr[i] << " "; // Ожидается: 10 20 30
    }
    std::cout << std::endl;
    for (int i = 0; i < arr.Size(); ++i) {
        arr[i] = 100;
    }
    for (int i = 0; i < arr.Size(); ++i) {
        std::cout << arr[i] << " "; // Ожидается: 100 100 100
    }
    std::cout << std::endl;

    // Тест 3: Копирование массива
    CArray<int> arrCopy = arr;
    std::cout << "Size of the copied array: " << arrCopy.Size() << std::endl; // Ожидается: 3

    // Тест 4: Изменение оригинального массива
    arr.Add(40);
    std::cout << "Size of the original array after adding an element: " << arr.Size() << std::endl; // Ожидается: 4
    std::cout << "Size of the copied array after modifying the original: " << arrCopy.Size() << std::endl; // Ожидается: 3

    for (int i = 0; i < arr.Size(); ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    arr.DeleteLast();

    for (int i = 0; i < arr.Size(); ++i) {
        std::cout << arr[i] << " "; 
    }
    std::cout << std::endl;

    CArray<int> arr2;
}

int main()
{
    testArray();
    return 0;
}