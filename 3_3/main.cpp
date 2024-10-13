
// Реализовать очередь с помощью двух стеков. Использовать стек, реализованный с помощью динамического буфера.
// Обрабатывать команды push back и pop front.

#include <iostream>
#include <cassert>

class Stack
{
public:
    explicit Stack( int _bufferSize ):
       bufferSize( _bufferSize ), topIndex( -1 )         
    {
        buffer = new int[bufferSize];
    }

    ~Stack()
    {
        delete [] buffer;
    }

    Stack( const Stack &s) 
    {
        bufferSize = s.bufferSize;
        topIndex = s.topIndex;

        buffer = new int[bufferSize];
        for ( int i = 0; i <= topIndex; ++i )
        {
            buffer[i] = s.buffer[i];
        }     
    }

    Stack& operator=( const Stack &s ) 
    {
        delete [] buffer;

        bufferSize = s.bufferSize;
        topIndex = s.topIndex;

        buffer = new int[bufferSize];
        for ( int i = 0; i <= topIndex; ++i )
        {
            buffer[i] = s.buffer[i];
        } 

        return *this;

    }

    const bool is_empty()
    {
        return topIndex == -1;
    }

    void push( int val )
    {
        assert( topIndex + 1 < bufferSize );
        buffer[++topIndex] = val;
    }

    int pop()
    {
        assert( topIndex > -1 );
        return buffer[topIndex--];
    }

private:
    int *buffer;
    int bufferSize;
    int topIndex;
};

class Queue
{
public:
    explicit Queue(int _size):
        size( _size )
    {
        inbox = new Stack(size);
        outbox = new Stack(size);
    } 

    ~Queue()
    {
        delete inbox;
        delete outbox;
    }

    Queue( const Queue &q )
    {
        inbox = new Stack(*(q.inbox));
        outbox = new Stack(*(q.outbox));
        size = q.size;
    }

    Queue& operator=( const Queue &q )
    {
        delete inbox;
        delete outbox;

        inbox = new Stack(*(q.inbox));
        outbox = new Stack(*(q.outbox));
        size = q.size;

        return *this;
    }

    const bool is_empty()
    {
        return inbox -> is_empty() && outbox -> is_empty();
    }

    void push( int val )
    {
        inbox -> push(val);        
    }

    int pop()
    {
        if (outbox -> is_empty())
        {
            while (!inbox -> is_empty())
            {
                int popVal = inbox -> pop();
                outbox -> push( popVal );
            }
        } 

        return outbox -> pop();     
    }

private:
    Stack *inbox;
    Stack *outbox;
    int size; 
};

void process( std::istream& in, std::ostream& out )
{
    int n = 0;  
    in >> n;    
    Queue q(n);

    int command = -1;
    int input_value = -1;

    for (int i = 0; i < n; ++i)
    {   
        in >> command >> input_value; 
        int operation_value = -1;
        switch (command)
        {

        case 2: // pop front
            operation_value = ( !q.is_empty() ) ? q.pop() : -1;
            if ( input_value != operation_value ) 
            {
                out << "NO" << std::endl;
                return;
            }
            break;

        case 3: // push back
            q.push( input_value );
            break;
        }       
    }

    out << "YES" << std::endl;
}

int main()
{ 
    process( std::cin, std::cout );
    return 0;
}