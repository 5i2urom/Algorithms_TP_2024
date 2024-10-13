// Выведите элементы в порядке in-order (слева направо)

#include <iostream>
#include <stack>
#include <vector>

template <typename T>
struct DefaultComp
{
    const bool operator()( const T& l, const T& r ) const
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

    BinaryTree& operator=( const BinaryTree& other )
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
                if ( parent->left == curr )
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

        explicit TreeNode( const T& value ): value( value ), left( nullptr ), right( nullptr ) {}

    };

    TreeNode* root;
    Comp comp;    
};

int main()
{
    int n = 0;
    std::cin >> n;

    std::vector<int> array(n);

    for ( int i = 0; i < n; ++i )
    {
        std::cin >> array[i];
    }

    BinaryTree<int> tree( array.data(), n );
    tree.InOrderTraversal();

    return 0;
}