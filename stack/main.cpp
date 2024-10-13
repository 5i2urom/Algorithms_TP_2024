
// АСД Стек (Односвязный список)

#include <iostream>
#include <cassert>

using namespace std;

class Stack 
{
    struct Node
    {
        Node *next;
        int val;

        Node(int val): next(nullptr), val(val) {}
    };

public:
    explicit Stack(): top(nullptr) {}

    ~Stack()
    {
        while (!is_empty()) 
            pop();                
    }

    Stack(const Stack &s)
    {
        Node *cur = s.top;
        Node *prev = nullptr;
        while (cur)
        {
            Node *newNode = new Node(cur->val);
            if (prev)
                prev -> next = newNode;
            else
                top = newNode;
            prev = newNode;

            cur = cur -> next;
        }
    }

    Stack& operator=(const Stack &s)
    {
        while (!is_empty())
            pop();
                Node *cur = s.top;

        Node *prev = nullptr;
        while (cur)
        {
            Node *newNode = new Node(cur->val);
            if (prev)
                prev -> next = newNode;
            else
                top = newNode;
            prev = newNode;

            cur = cur -> next;
        }

        return *this;
    }

    Stack(Stack &&)=delete;
    Stack &operator=(Stack&&)=delete;

    bool is_empty() const {
        return top == nullptr;
    }

    void push(int val) {
        Node* cur = top;
        top = new Node(val);
        top -> next = cur;
    }

    int pop()
    {
        assert (top);

        int val = top -> val;
        Node* cur = top;
        top = top -> next;

        delete cur;
        return val;
    }

    int peek() 
    {
        assert (top);
        return top -> val;
    }
    
private:
    Node *top;
};

void testStack()
{
    {
        Stack s = Stack();
        assert(s.is_empty());
    }

    {
        Stack s = Stack();
        s.push(5);
        s.push(4);
        s.push(3);
        s.push(2);
        s.push(1);
        assert(s.pop() == 1);
        assert(s.pop() == 2);
        assert(s.pop() == 3);
        assert(s.pop() == 4);
        assert(s.pop() == 5);
        assert(s.is_empty());
    }

    {
        Stack s = Stack();
        s.push(5);
        s.push(4);
        s.pop();
        s.push(3);
        s.pop();
        assert(s.pop() == 5);
        assert(s.is_empty());
    }

    {
        Stack s = Stack();
        s.push(1);
        s.push(2);
        s.push(3);

        Stack s2 = s;
        assert(s2.pop() == 3);
        assert(s2.pop() == 2);
        assert(s2.pop() == 1);
        assert(s2.is_empty());

        assert(s.pop() == 3);
        assert(s.pop() == 2);
        assert(s.pop() == 1);
        assert(s.is_empty());
    }

    {
        Stack s = Stack();
        s.push(1);
        s.push(2);

        Stack s2 = Stack();
        s2.push(4);
        s2.push(5);

        s2 = s;
        assert(s2.pop() == 2);
        assert(s2.pop() == 1);
        assert(s2.is_empty());

        assert(s.pop() == 2);
        assert(s.pop() == 1);
        assert(s.is_empty());
    }

    cout << "Success" << endl;    
}

int main() {
    testStack();

    return 0;
}