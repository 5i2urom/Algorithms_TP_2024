// Реализуйте стратегию выбора опорного элемента “случайный элемент”.
// Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.

#include <iostream>
#include <ctime> 

template <typename T>
struct DefaultComp
{
    const bool operator()( const T& l, const T& r ) const
    {
        return l < r;
    }
};

template <typename T, typename Comp = DefaultComp<T>>
int Partition( T* arr, int l, int r, Comp comp = Comp() ) {

    // Случайный выбор опорного элемента
    srand( time( nullptr ) );
    
    int rand_idx = l + rand() % ( r - l );
    std::swap( arr[rand_idx], arr[l] );
    const T& pivot = arr[l]; // Опорный элемент в начале массива

    int i = r - 1;
    for ( int j = i; j > l; --j )
    {
        if ( !comp( arr[j], pivot ) )
        {
            std::swap( arr[j], arr[i--] );
        }
    }
    std::swap(arr[l], arr[i]);   

    return i;
}

template <typename T, typename Comp = DefaultComp<T>>
void kth_element( T* a, int n, int k, Comp comp = Comp() )
{
    int l = 0;
    int r = n;
    while ( l <= r ) 
    {
        int idx = Partition( a, l, r, comp );
        if ( idx == k )
            return;
        else if ( idx < k )
            l = idx + 1;
        else
            r = idx;
    }
}

template <typename T, typename Comp = DefaultComp<T>>
T GetPercentile( T* a, int n, int percent, Comp comp = Comp() )
{
    int k = n * percent / 100;
    kth_element( a, n, k, comp);
    return a[k];
}


int main()
{
    int n = 0;
    std::cin >> n;

    int* arr = new int[n];

    for ( int i = 0; i < n; ++i )
        std::cin >> arr[i];
    
    std::cout << GetPercentile( arr, n, 10 ) << std::endl;
    std::cout << GetPercentile( arr, n, 50 ) << std::endl;
    std::cout << GetPercentile( arr, n, 90 ) << std::endl;

    delete [] arr;
}
