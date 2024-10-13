#include <iostream>
#include <cassert>
#include <vector>

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
      : data(data), left(nullptr), right(nullptr), height(1)
      {
      }
      
      T data;
      Node *left;
      Node *right;
      size_t height;


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
    
    void Add(const T &data)
    {
        root = addInternal(root, data);
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
private:
    
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
            
            // поддерево, из которого берем элемент взамен удаляемого, выбираем на основе сравнения глубин.
            // (берем из более глубокого)

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
    
    Node* addInternal(Node *node, const T &data)
    {
        if (!node)
            return new Node(data);
        if (!comp(data, node->data))
            node->right = addInternal(node->right, data);
        else
            node->left = addInternal(node->left, data);
        
        return doBalance(node);
    }
    
    size_t getHeight(Node *node)
    {
        return node ? node->height : 0;
    }
    
    void fixHeight(Node *node)
    {
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
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
        fixHeight(node);
        fixHeight(tmp);
        return tmp;
    }
    
    Node* rotateRight(Node *node)
    {
        Node *tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        fixHeight(node);
        fixHeight(tmp);
        return tmp;
    }
    
    Node* doBalance(Node *node)
    {
        fixHeight(node);
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

void test()
{
    AvlTree<int> tree;
    
    // Тест 1: Добавление элементов и проверка наличия
    tree.Add(10);
    tree.Add(5);
    tree.Add(15);
    tree.Add(3);
    tree.Add(7);
    tree.Add(12);
    tree.Add(17);
    
    assert(tree.Has(10));
    assert(tree.Has(5));
    assert(tree.Has(15));
    assert(tree.Has(3));
    assert(tree.Has(7));
    assert(tree.Has(12));
    assert(tree.Has(17));
    
    // Тест 2: Удаление элемента и проверка наличия
    tree.Delete(7);
    assert(!tree.Has(7));
    
    // Тест 3: Удаление корневого элемента и проверка наличия
    tree.Delete(10);
    assert(!tree.Has(10));
    
    // Тест 4: Удаление элемента без детей и проверка наличия
    tree.Delete(3);
    assert(!tree.Has(3));
    
    // Тест 5: Удаление элемента с одним ребенком и проверка наличия
    tree.Delete(5);
    assert(!tree.Has(5));
    
    // Тест 6: Удаление элемента с двумя детьми и проверка наличия
    tree.Delete(15);
    assert(!tree.Has(15));
    
    std::cout << "Все тесты пройдены успешно!" << std::endl;
}

void test2()
{
    AvlTree<int> tree;

    // Тест на вставку
    std::vector<int> values = {10, 20, 5, 15, 25, 3, 7, 12, 18, 22, 30};
    for (int value : values) {
        tree.Add(value);
    }

    // Проверка содержимого дерева
    for (int value : values) {
        assert(tree.Has(value));
    }
    assert(!tree.Has(100));

    // Тест на удаление
    tree.Delete(10);
    assert(!tree.Has(10));
    tree.Delete(20);
    assert(!tree.Has(20));
    tree.Delete(3);
    assert(!tree.Has(3));

    // Проверка оставшихся элементов
    std::vector<int> remaining = {5, 7, 12, 15, 18, 22, 25, 30};
    for (int value : remaining) {
        assert(tree.Has(value));
    }

    // Тест на вставку после удаления
    tree.Add(10);
    assert(tree.Has(10));

    std::cout << "All tests passed!" << std::endl;
}

int main(int argc, const char * argv[]) {
    test();
    test2();
    return 0;
}   