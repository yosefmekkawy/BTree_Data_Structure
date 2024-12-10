#include <iostream>

template <typename T, int ORDER>
class BTree
{
private:
    struct Node
    {
        T keys[ORDER];
        Node *children[ORDER + 1];
        Node *parent;
        int numKeys;
        bool isLeaf;

        Node() : parent(nullptr), numKeys(0), isLeaf(true)
        {
            for (int i = 0; i < ORDER + 1; ++i)
            {
                children[i] = nullptr;
            }
        }
    };

    Node *root;

    void splitChild(Node *node)
    {
        int midIndex = ORDER / 2;
        T midKey = node->keys[midIndex];

        Node *newRightChild = new Node();
        newRightChild->isLeaf = node->isLeaf;
        newRightChild->numKeys = ORDER - midIndex - 1;
        newRightChild->parent = node->parent;

        for (int i = 0; i < newRightChild->numKeys; ++i)
        {
            newRightChild->keys[i] = node->keys[midIndex + 1 + i];
        }

        if (!node->isLeaf)
        {
            for (int i = 0; i <= newRightChild->numKeys; ++i)
            {
                newRightChild->children[i] = node->children[midIndex + 1 + i];
                if (newRightChild->children[i])
                {
                    newRightChild->children[i]->parent = newRightChild;
                }
            }
        }

        node->numKeys = midIndex;

        if (node->parent == nullptr)
        {
            // If node is the root, create a new root
            root = new Node();
            root->isLeaf = false;
            root->keys[0] = midKey;
            root->numKeys = 1;
            root->children[0] = node;
            root->children[1] = newRightChild;
            node->parent = root;
            newRightChild->parent = root;
        }
        else
        {
            // Insert midKey into the parent
            Node *parent = node->parent;
            int insertIndex = 0;

            while (insertIndex < parent->numKeys && parent->keys[insertIndex] < midKey)
            {
                insertIndex++;
            }

            for (int i = parent->numKeys; i > insertIndex; --i)
            {
                parent->keys[i] = parent->keys[i - 1];
                parent->children[i + 1] = parent->children[i];
            }

            parent->keys[insertIndex] = midKey;
            parent->children[insertIndex + 1] = newRightChild;
            parent->numKeys += 1;

            newRightChild->parent = parent;

            if (parent->numKeys == ORDER)
            {
                splitChild(parent);
            }
        }
    }

    void insertAtLeaf(Node *node, const T &key)
    {
        int i = node->numKeys - 1;

        while (i >= 0 && key < node->keys[i])
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }

        node->keys[i + 1] = key;
        node->numKeys += 1;

        if (node->numKeys == ORDER)
        {
            splitChild(node);
        }
    }

    Node *findLeaf(Node *node, const T &key)
    {
        if (node->isLeaf)
        {
            return node;
        }

        int i = 0;
        while (i < node->numKeys && key > node->keys[i])
        {
            i++;
        }

        return findLeaf(node->children[i], key);
    }

    void printNode(Node *node, int depth = 0) const
    {
        if (node == nullptr)
            return;

        for (int i = 0; i < depth; ++i)
        {
            std::cout << "  ";
        }

        for (int i = 0; i < node->numKeys; ++i)
        {
            std::cout << node->keys[i];
            if (i < node->numKeys - 1)
            {
                std::cout << ",";
            }
        }
        std::cout << std::endl;

        for (int i = 0; i <= node->numKeys; ++i)
        {
            printNode(node->children[i], depth + 1);
        }
    }

    void deleteNode(Node *node)
    {
        if (node == nullptr)
            return;
        for (int i = 0; i <= node->numKeys; ++i)
        {
            deleteNode(node->children[i]);
        }
        delete node;
    }

public:
    BTree() : root(nullptr) {}

    ~BTree()
    {
        deleteNode(root);
    }

    void Insert(const T &key)
    {
        if (root == nullptr)
        {
            root = new Node();
        }

        Node *leaf = findLeaf(root, key);
        insertAtLeaf(leaf, key);
    }

    void Print() const
    {
        printNode(root);
    }
};

int main()
{
    // Construct a BTree of order 3, which stores int data
    BTree<int, 3> t1;

    t1.Insert(1);
    t1.Insert(5);
    t1.Insert(0);
    t1.Insert(4);
    t1.Insert(3);
    t1.Insert(2);

    t1.Print(); // Should output the following on the screen:

    /*
    1,4
      0
      2,3
      5
    */

    // Construct a BTree of order 5, which stores char data
    BTree<char, 5> t;

    // Look at the example in our lecture:
    t.Insert('G');
    t.Insert('I');
    t.Insert('B');
    t.Insert('J');
    t.Insert('C');
    t.Insert('A');
    t.Insert('K');
    t.Insert('E');
    t.Insert('D');
    t.Insert('S');
    t.Insert('T');
    t.Insert('R');
    t.Insert('L');
    t.Insert('F');
    t.Insert('H');
    t.Insert('M');
    t.Insert('N');
    t.Insert('P');
    t.Insert('Q');

    t.Print(); // Should output the following on the screen:

    /*
    K
      C,G
        A,B
        D,E,F
        H,I,J
      N,R
        L,M
        P,Q
        S,T
    */

    return 0;
}