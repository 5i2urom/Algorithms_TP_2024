// Выведите элементы в порядке in-order (слева направо)

#include <iostream>
#include <stack>
#include <vector>
#include <cassert>

template <typename T>
struct DefaultComp
{
    const bool operator()(const T& l, const T& r) const
    {
        return l < r;
    }
};

template <typename T, typename Comp=DefaultComp<T>>
class BinaryTree
{
public:
    BinaryTree( Comp comp = Comp() ): comp( comp ), root( nullptr ) {}
    BinaryTree( const T* arr, size_t size, Comp comp = Comp() ): comp( comp ), root( nullptr )
    {
        for ( size_t i = 0; i < size; ++i )
        {
            InsertNode( arr[i] );
        }
    }

    ~BinaryTree()
    {
        ClearTree();
    }

    BinaryTree( const BinaryTree& other ): root( nullptr ), comp( other.comp )
    {        
        if ( !other.root )
            return;

        std::stack<TreeNode*> stack;
        TreeNode* curr = other.root;
        TreeNode* prev = nullptr;

        while ( curr || !stack.empty() )
        {
            if ( curr )
            {
                stack.push( curr );
                curr = curr->left;
            }
            else
            {
                curr = stack.top();
                stack.pop();

                InsertNode( curr->value );

                curr = curr->right;
            }
        }
    }

    BinaryTree& operator=(const BinaryTree& other)
    {
        ClearTree();

        comp = other.comp;

        if ( !other.root )
            return *this;

        std::stack<TreeNode*> stack;
        TreeNode* curr = other.root;
        TreeNode* prev = nullptr;

        while ( curr || !stack.empty() )
        {
            if ( curr )
            {
                stack.push( curr );
                curr = curr->left;
            }
            else
            {
                curr = stack.top();
                stack.pop();

                InsertNode( curr->value );

                curr = curr->right;
            }
        }

        return *this;

    }

    void InsertNode( T value )
    {
        TreeNode* newNode = new TreeNode( value );

        if ( !root )
        {            
            root = newNode;
            return;
        }

        TreeNode* curr = root;
        TreeNode* parent = nullptr;

        
        while ( curr )
        {
            parent = curr;
            curr = ( comp( value, curr->value ) ) ? curr->left : curr->right;  
        }

        if ( comp( value, parent->value ) )
            parent->left = newNode;
        else
            parent->right = newNode;      
    }

    bool FindNode( T value ) const
    {   
        if ( !root )           
            return false;

        TreeNode* curr = root;
        
        while ( curr )
        {
            if ( curr->value == value )
                return true;
            curr = ( comp( value, curr->value ) ) ? curr->left : curr->right;  
        }

        return false;
    }

    bool DeleteNode( T value )
    {
        if ( !root )           
            return false;

        TreeNode* curr = root;
        TreeNode* parent = nullptr;

        while ( curr )
        {
            if ( curr->value == value )
                break;
            parent = curr;
            curr = ( comp( value, curr->value ) ) ? curr->left : curr->right;  
        }

        if ( !curr )
            return false;

        // Если нет левого поддерева
        if ( !curr->left ) 
        {
            if ( parent )
            {
                if (parent->left == curr)
                    parent->left = curr->right;
                else
                    parent->right = curr->right;
            }

            else 
                root = curr->right;

            delete curr;
            return true;
        }

        // Если нет правого поддерева
        else if ( !curr->right ) 
        {
            if ( parent )
            {
                if ( parent->left == curr )
                    parent->left = curr->left;
                else
                    parent->right = curr->left;
            }

            else
                root = curr->left;

            delete curr;
            return true;
        }

        // Есть оба поддерева
        else
        {
            TreeNode* replaceNode = curr->right;
            
            while ( replaceNode->left )
            {
                replaceNode = replaceNode->left;
            }
            curr->value = replaceNode->value;

            delete replaceNode;
            return true;
        }

        return false;

    }

    void ClearTree()
    {
        std::stack<TreeNode*> stack;
        TreeNode* curr = root;

        while ( curr || !stack.empty() )
        {
            if ( curr )
            {
                stack.push( curr );
                curr = curr->left;
            }
            else
            {
                curr = stack.top();
                stack.pop();
                TreeNode* next = curr->right;
                delete curr;
                curr = next;
            }
        }   

        root = nullptr; 
    }

    void InOrderTraversal() const
    {     
        std::stack<TreeNode*> stack;
        TreeNode* curr = root;

        while (curr || !stack.empty())
        {
            if (curr)
            {
                stack.push(curr);
                curr = curr->left;
            }
            else
            {
                curr = stack.top();
                stack.pop();
                std::cout << curr->value << " ";
                curr = curr->right;
            }
        }
        std::cout << std::endl; 
    }

private:
    struct TreeNode
    {
        T value;
        TreeNode* left;
        TreeNode* right;

        explicit TreeNode( const T& value ): value(value), left(nullptr), right(nullptr) {}

    };

    TreeNode* root;
    Comp comp;    
};

void testCopyConstructor()
{
    // Создаем дерево tree1
    BinaryTree<int> tree1;
    tree1.InsertNode(50);
    tree1.InsertNode(30);
    tree1.InsertNode(70);
    tree1.InsertNode(20);
    tree1.InsertNode(40);
    tree1.InsertNode(60);
    tree1.InsertNode(80);

    // Проверяем оператор копирования
    BinaryTree<int> tree2 = tree1;

    // Убеждаемся, что tree2 содержит те же элементы, что и tree1
    std::cout << "tree2 после копирования tree1:\n";
    assert(tree2.FindNode(50));
    assert(tree2.FindNode(30));
    assert(tree2.FindNode(70));
    assert(tree2.FindNode(20));
    assert(tree2.FindNode(40));
    assert(tree2.FindNode(60));
    assert(tree2.FindNode(80));
}

void testAssignmentOperator()
{
    // Создаем дерево tree1
    BinaryTree<int> tree1;
    tree1.InsertNode(50);
    tree1.InsertNode(30);
    tree1.InsertNode(70);
    tree1.InsertNode(20);
    tree1.InsertNode(40);
    tree1.InsertNode(60);
    tree1.InsertNode(80);

    // Создаем дерево tree3
    BinaryTree<int> tree3;
    tree3.InsertNode(10);
    tree3.InsertNode(5);
    tree3.InsertNode(15);

    // Проверяем оператор присваивания
    tree3 = tree1;

    // Убеждаемся, что tree3 содержит те же элементы, что и tree1
    std::cout << "tree3 после присваивания:\n";
    assert(tree3.FindNode(50));
    assert(tree3.FindNode(30));
    assert(tree3.FindNode(70));
    assert(tree3.FindNode(20));
    assert(tree3.FindNode(40));
    assert(tree3.FindNode(60));
    assert(tree3.FindNode(80));

    assert(!tree3.FindNode(10));
    assert(!tree3.FindNode(5));
    assert(!tree3.FindNode(15));
}

int main()
{
    testCopyConstructor();
    testAssignmentOperator();

    std::cout << "Тесты успешно пройдены!\n";

    return 0;
}