// Закраска прямой 1.
// На числовой прямой окрасили N отрезков. Известны координаты левого и правого концов каждого отрезка (Li и Ri).
// Найти длину окрашенной части числовой прямой.

#include <iostream>

template <typename T>
struct DefaultComp
{
    const bool operator()( const T& l, const T& r ) const
    {
        return l < r;
    }
};

template<typename T, typename Comp = DefaultComp<T>>
void Merge( T* arr1, int arrLen1, T* arr2, int arrLen2, T* newArr, Comp comp = Comp() )
{
    if ( arrLen1 < 0 || arrLen2 < 0 )
        return;

    int i = 0, j = 0, k = 0;
    while ( i < arrLen1 && j < arrLen2 )
    {
        newArr[k++] = ( comp( arr1[i], arr2[j] ) ) ? arr1[i++] : arr2[j++];
    }    

    while ( j < arrLen2)
    {
        newArr[k++] = arr2[j++];
    }

    while ( i < arrLen1 )
    {
        newArr[k++] = arr1[i++];
    }    
}

template<typename T, typename Comp = DefaultComp<T>>
void MergeSort( T* arr, int n, Comp comp = Comp() )
{
    T* buffer = new T[n];
    T* merged_arrs = new T[n];
    for ( int i = 0; i < n; ++i )
    {
        buffer[i] = arr[i];
        merged_arrs[i] = arr[i];
    }   

    for ( int mLen = 1; mLen < n; mLen *= 2 )
    {
        for ( int i = 0; i < n; i += mLen * 2)
        {
            int len1 = ( n - i < mLen ) ? n - i : mLen;
            int len2 = ( n - i - mLen < mLen ) ? n - i - mLen : mLen;  
            Merge( &buffer[i], len1, &buffer[i+mLen], len2, &merged_arrs[i] );
        }

        std::swap( buffer, merged_arrs );
    }

    for ( int i = 0; i < n; ++i )
    {
        arr[i] = buffer[i];
    }
    delete [] merged_arrs;
    delete [] buffer;
}

struct Point
{
    Point(): _num( 0 ) {}
    Point( int num, bool start, bool end )
        :_num( num ), _start( start ), _end( end ) {}

    int _num;
    bool _start = false;
    bool _end = false;

    const bool operator<( const Point& p ) const
    {
        return _num < p._num;
    }
};

struct Segment
{
    Segment(): _start(), _end() {}
    Segment( int start, int end )
        : _start( start, true, false ), _end( end, false, true ) {}

    Point _start;
    Point _end;
};

int PaintedLine( Segment* segments, int n )
{
    Point* points = new Point[n*2];

    int k = 0;
    for ( int i = 0; i < n; ++i )
    {
        points[k++] = segments[i]._start;
        points[k++] = segments[i]._end;
    }

    MergeSort<Point>( points, k );

    int count = 0;
    int color = 0;
    for ( int i = 0; i < n * 2; ++i )
    {        
        if ( count > 0)
        {
            int dif = points[i]._num - points[i-1]._num;
            color += dif;
        }  

        if ( points[i]._start ) ++count;
        if ( points[i]._end ) --count; 
          
    } 

    delete [] points;
    return color;
}

int main()
{
    int n = 0;
    std::cin >> n;

    Segment* segments = new Segment[n];

    for ( int i = 0; i < n; ++i )
    {
        int start = 0;
        int end = 0;
        std::cin >> start >> end;
        
        segments[i] = Segment( start, end );
    }

    std::cout << PaintedLine( segments, n );

    return 0;
}