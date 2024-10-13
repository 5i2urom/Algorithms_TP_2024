// Солдаты. В одной военной части решили построить в одну шеренгу по росту.
// Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат,
// а именно для каждого приходящего солдата указывать, перед каким солдатом в строе он должен становится. 

#include <iostream>
#include <cassert>

template <typename T>
struct DefaultComparator
{
    bool operator()(const T& l, const T& r) const
    {
        return l < r;
    }
};

template <typename T>
struct InverseComparator
{
    bool operator()(const T& l, const T& r) const
    {
        return l > r;
    }
};

template <typename T, typename Comparator=DefaultComparator<T>>
class AvlTree
{
  struct Node
  {
      Node(const T &data)
      : data(data), left(nullptr), right(nullptr), height(1), count(1)
      {
      }
      
      T data;
      Node *left;
      Node *right;
      size_t height;
      size_t count;
  };
    
public:
    AvlTree(Comparator comp = Comparator())
    : root(nullptr)
    {
    }
    
    ~AvlTree()
    {
        destroyTree(root);
    }

    AvlTree(const AvlTree& other)
    {
        root = copyTreeInternal(other.root);
    }

    AvlTree& operator=(const AvlTree& other)
    {
        destroyTree(root);
        root = copyTreeInternal(other.root);

        return *this;        
    }
    
    void Add(const T &data)
    {
        int k = 0;
        root = addInternal(root, data, k);
    }
    
    bool Has(const T &data)
    {
        Node *tmp = root;
        while (tmp)
        {
            if (comp(tmp->data, data))
                tmp = tmp->right;
            else if (comp(data, tmp->data))
                tmp = tmp->left;
            else
                return true;
        }
        return false;
    }

    void Delete(const T &data)
    {
        root = deleteInternal(root, data);
    }

    int KthPosition(const T& data)
    {    
        int k = 0;  
        root = addInternal(root, data, k);
        return k;        
    }

    void DeleteKthPosition(int k)
    {
        root = DeleteKthPositionInternal(root, k);
    }
private:
    Node* copyTreeInternal(Node* node)
    {
        if (!node)
            return nullptr;

        Node* newNode = new Node(node->data);
        newNode->left = copyTreeInternal(node->left);
        newNode->right = copyTreeInternal(node->right);          
        newNode->height = node->height;
        newNode->count = node->count;
        return newNode;     
    }

    void destroyTree(Node *node)
    {
        if (node)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
    
    Node* deleteInternal(Node *node, const T &data)
    {
        if (!node)         
            return nullptr;
        if (comp(node->data, data))
            node->right = deleteInternal(node->right, data);
        else if (comp(data, node->data))
            node->left = deleteInternal(node->left, data);
        else
        {
            Node *left = node->left;
            Node *right = node->right;
            
            int balance = getBalance(node);

            delete node;
            
            if (!right)
                return left;

            Node *extremal = nullptr;

            if (balance >= 0)
                right = findAndRemoveMin(right, extremal);
            else
                left = findAndRemoveMax(left, extremal);

            extremal->left = left;
            extremal->right = right;
            
            return doBalance(extremal);
        }
        
        return doBalance(node);
    }

    Node* findAndRemoveMin(Node *node, Node *& min)
    {
        if (!node->left)
        {
            min = node;
            return node->right;
        }
        node->left = findAndRemoveMin(node->left, min);
        return doBalance(node);
    }

    Node* findAndRemoveMax(Node *node, Node *& max)
    {
        if (!node->right)
        {
            max = node;
            return node->left;
        }
        node->right = findAndRemoveMax(node->right, max);
        return doBalance(node);
    }
    
    Node* addInternal(Node *node, const T &data, int &k)
    {
        if (!node)
            return new Node(data);
        if (!comp(data, node->data))
        {
            k += getCount(node->left) + 1;
            node->right = addInternal(node->right, data, k);
        }
        else
        {
            node->left = addInternal(node->left, data, k);
        }
        
        return doBalance(node);
    }
    
    Node* DeleteKthPositionInternal(Node *node, int &k)
    {
        if (!node)
            return nullptr;
        if (k > getCount(node->left))
        {
            k = k - getCount(node->left) - 1;
            node->right = DeleteKthPositionInternal(node->right, k);
        }
        else if (k < getCount(node->left))
        {
            node->left = DeleteKthPositionInternal(node->left, k);
        }
        else
        {
            Node *left = node->left;
            Node *right = node->right;
            
            int balance = getBalance(node);

            delete node;
            
            if (!right)
                return left;

            Node *extremal = nullptr;

            if (balance >= 0)
                right = findAndRemoveMin(right, extremal);
            else
                left = findAndRemoveMax(left, extremal);

            extremal->left = left;
            extremal->right = right;
            
            return doBalance(extremal);
        }
        
        return doBalance(node);
    }

    size_t getHeight(Node *node)
    {
        return node ? node->height : 0;
    }

    size_t getCount(Node *node)
    {
        return node ? node->count : 0;
    }
    
    void fixHeightAndCount(Node *node)
    {
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
        node->count = getCount(node->left) + getCount(node->right) + 1;
    }
    
    int getBalance(Node *node)
    {
        return getHeight(node->right) - getHeight(node->left);
    }
    
    Node* rotateLeft(Node *node)
    {
        Node *tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        fixHeightAndCount(node);
        fixHeightAndCount(tmp);
        return tmp;
    }
    
    Node* rotateRight(Node *node)
    {
        Node *tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        fixHeightAndCount(node);
        fixHeightAndCount(tmp);
        return tmp;
    }
    
    Node* doBalance(Node *node)
    {
        fixHeightAndCount(node);
        switch (getBalance(node))
        {
            case 2:
            {
                if (getBalance(node->right) < 0)
                    node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
            case -2:
            {
                if (getBalance(node->left) > 0)
                    node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
            default:
                return node;
        }
    }
    
    Node *root;
    Comparator comp;
};

void testing()
{
    // Создаем дерево и добавляем несколько элементов
    AvlTree<int, InverseComparator<int>> tree;
    tree.Add(5);
    tree.Add(3);
    tree.Add(7);

    // Тестируем оператор копирования
    AvlTree<int, InverseComparator<int>> copiedTree = tree;
    // Проверяем, что копирование прошло успешно
    assert(copiedTree.Has(5));
    assert(copiedTree.Has(3));
    assert(copiedTree.Has(7));

    // Создаем дерево с изначальным содержимым
    AvlTree<int, InverseComparator<int>> assignedTree;
    assignedTree.Add(10);
    assignedTree.Add(15);

    // Присваиваем дерево с элементами
    assignedTree = tree;
    // Проверяем, что присваивание прошло успешно
    assert(assignedTree.Has(5));
    assert(assignedTree.Has(3));
    assert(assignedTree.Has(7));

    // Проверяем, что присваивание полностью заменило содержимое assignedTree на содержимое tree
    assert(!assignedTree.Has(10));
    assert(!assignedTree.Has(15));

    std::cout << "All tests passed successfully!" << std::endl;
}

int main() {
    testing();
    // AvlTree<int, InverseComparator<int>> tree;
    // int n = 0;
    // int operation = 0;
    // int digit = 0; 
    // std::cin >> n;
    // for (int i = 0; i < n; ++i) 
    // {
    //     std::cin >> operation >> digit;
    //     switch (operation)
    //     {
    //     case 1:
    //         std::cout << tree.KthPosition(digit) << std::endl;
    //         break;
        
    //     case 2:
    //         tree.DeleteKthPosition(digit);
    //         break;
    //     }

    // }
    return 0;
}   