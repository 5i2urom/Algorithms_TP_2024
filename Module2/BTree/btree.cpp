#include <iostream>
#include <vector>
#include <queue>

template <typename T>
struct DefaultComparator
{
    bool operator()(const T& l, const T& r) const
    {
        return l < r;
    }
};

template <typename T, typename Comparator=DefaultComparator<T>>
class BTree
{
public:
    struct Node
    {
        Node(bool leaf)
        : leaf(leaf)
        {
        }
        
        ~Node()
        {
            for (Node* child: children)
            {
                delete child;
            }
        }
        
        bool leaf;
        std::vector<T> keys;
        std::vector<Node*> children;
    };
    
    BTree(size_t minDegree, Comparator comp = Comparator())
    : t(minDegree), root(nullptr), comp(comp)
    {
    }
    
    ~BTree()
    {
        if (root)
            delete root;
    }

    BTree(const BTree& other)
    : t(other.t), comp(other.comp)
    {
        if (!other.root)
            return;
        
        root = new Node(other.root->leaf);

        std::queue<std::pair<Node*, Node*>> q;
        q.push({other.root, root});

        while (!q.empty())
        {
            auto [otherNode, thisNode] = q.front();
            q.pop();
            thisNode->keys = otherNode->keys;

            for (size_t i = 0; i < otherNode->children.size(); ++i)
            {
                Node *newNode = new Node(otherNode->children[i]->leaf);
                thisNode->children.push_back(newNode);
                q.push({otherNode->children[i], thisNode->children[i]});
            }             
        }
    }

    BTree& operator=(const BTree& other)
    {
        if (root)
        {
            delete root;
            root = nullptr;
        }

        t = other.t;
        comp = other.comp;

        if (!other.root)
            return *this;
        
        root = new Node(other.root->leaf);
        std::queue<std::pair<Node*, Node*>> q;
        q.push({other.root, root});

        while (!q.empty())
        {
            auto [otherNode, thisNode] = q.front();
            q.pop();
            thisNode->keys = otherNode->keys;

            for (size_t i = 0; i < otherNode->children.size(); ++i)
            {
                Node *newNode = new Node(otherNode->children[i]->leaf);
                thisNode->children.push_back(newNode);
                q.push({otherNode->children[i], thisNode->children[i]});
            }             
        }

        return *this;
    }
    
    void Insert(const T &key)
    {
        if (!root)
            root = new Node(true);
        
        if (isNodeFull(root))
        {
            Node *newRoot = new Node(false);
            newRoot->children.push_back(root);
            root = newRoot;
            splitChild(root, 0);
        }
        
        insertNonFull(root, key);
    }

   void printLayers()
    {
        if (!root)
            return;

        std::queue<Node*> q;
        q.push(root);

        while (!q.empty())
        {
            int levelSize = q.size();

            for (int i = 0; i < levelSize; i++)
            {
                Node* curr = q.front();
                q.pop();

                for (const T& key : curr->keys)
                    std::cout << key << " ";

                for (Node* child : curr->children)
                {
                    if (child)
                        q.push(child);
                }
            }

            std::cout << std::endl;
        }
    }
    
private:    
    bool isNodeFull(Node *node)
    {
        return node->keys.size() == 2*t - 1;
    }
    
    void splitChild(Node *node, size_t index)
    {
        Node* leftChild = node->children[index];
        Node* rightChild = new Node(leftChild->leaf);

        rightChild->keys.assign(leftChild->keys.begin() + t, leftChild->keys.end()); 

        if (!leftChild->leaf)
        {
            rightChild->children.assign(leftChild->children.begin() + t, leftChild->children.end());
            leftChild->children.resize(t); 
        }

        node->children.insert(node->children.begin() + index + 1, rightChild);
        node->keys.insert(node->keys.begin() + index, leftChild->keys[t - 1]);

        leftChild->keys.resize(t - 1);
    }
    
    void insertNonFull(Node *node, const T &key)
    {
        int pos = node->keys.size() - 1;
        
        if (node->leaf)
        {
            node->keys.resize(node->keys.size() + 1);
            while (pos >= 0 && comp(key, node->keys[pos]))
            {
                node->keys[pos + 1] = node->keys[pos];
                pos--;
            }
            node->keys[pos + 1] = key;
        }
        else
        {
            while (pos >= 0 && comp(key, node->keys[pos]))
            {
                pos--;
            }
            
            if (isNodeFull(node->children[pos + 1]))
            {
                splitChild(node, pos + 1);
                if (comp(node->keys[pos + 1], key))
                    pos++;
            }
            insertNonFull(node->children[pos + 1], key);
        }
    }
    
    size_t t;
    Node *root;
    Comparator comp;
};

int main()
{ 
    int t = 0;
    std::cin >> t;
    BTree<int> btree(t); 

    int digit = 0;
    while (std::cin >> digit)
    {
        btree.Insert(digit);
    }

    btree.printLayers();
    
    return 0;
}
