
// Даны два массива неубывающих целых чисел, упорядоченные по возрастанию.
// A[0..n-1] и B[0..m-1]. n >> m. Найдите их пересечение.

#include <iostream>

using namespace std;

int binary_search( const int *arr, int len, int element )
{
    int first = 0;
    int last = len;

    while ( first < last )
    {
        int mid = ( first + last ) / 2;
        if ( arr[mid] < element ) 
            first = mid + 1;
        else 
            last = mid;
    }
    return ( first == len || arr[first] != element ) ? -1 : first;
} 

int *intersect_arr( const int *A, int sizeA, const int *B, int sizeB, int *result, int &resultSize )
{   
    resultSize = 0;

    int exp_start = 0;
    int indOf2 = 1;    
    for ( int i = 0; i < sizeB; ++i )  
    {
        //Экспоненциальный поиск        
        while ( indOf2 < sizeA && A[indOf2] <= B[i] )
        {
            exp_start = indOf2;
            indOf2 *= 2;
        }
        
        int exp_stop = ( indOf2 < sizeA ) ? indOf2 : sizeA - 1;        
        int new_len = exp_stop - exp_start + 1;

        int pos_el = binary_search( &A[exp_start], new_len, B[i] );
        if ( pos_el > -1 )
            result[resultSize++] = B[i];      

        indOf2 = (exp_start > 0) ? exp_start : 1;
    }

    return result; 
    
}

int main()
{
    int n = 0, m = 0;
    cin >> n;
    cin >> m;
         
    int *A = new int [n];
    int *B = new int [m];
    
    for ( int i = 0; i < n; ++i )
    {
        cin >> A[i];
    }
    
    for ( int i = 0; i < m; ++i )
    {
        cin >> B[i];
    }
    
    int *result = new int [n];
    int resultSize = 0;

    result = intersect_arr( A, n, B, m, result, resultSize );  

    for( int i = 0; i < resultSize; ++i )
    {
        cout << result[i] << " ";
    }      
    cout << endl;

    delete [] A;
    delete [] B;
    delete [] result;    

    return 0;
}

