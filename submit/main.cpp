/*
* Dummy file입니다.
* 본인이 테스트하고 싶은 내용으로 변경 후 사용하세요.
*/
#include <iostream>
#include <stack>

#define max(x, y) (x > y ? x : y)

using namespace std;

enum rotatetype {
    NO, LL, LR, RL, RR  
};

template <typename T>
struct Node {
    T key;
    Node* left;
    Node* right;
    int bf;

    Node(T key_=0) : key(key_), left(nullptr), right(nullptr){}
};

template <typename T>
class AVL{
    Node<T>* root;

    Node<T>* getBSTNode() { return new Node<T>; }

    int height(Node<T>* node) {
        if (node == nullptr) return 0;

        return max(height(node->left), height(node->right)) + 1;
    }

    int size(Node<T>* node) {
        if (node == nullptr) return 0;

        return size(node->left) + size(node->right) + 1;
    }

    int bf(Node<T>* node) {
        if (node == nullptr) return 0;

        return height(node->left) - height(node->right);
    }

    bool insertBST(T newKey) {
        Node<T>* p = root;
        Node<T>* q = nullptr;

        while (p != nullptr) {
            if (newKey == p->key) return false;

            q = p;

            if (newKey < p->key) p = p->left;
            else p = p->right;
        }

        Node<T>* newNode = getBSTNode();
        newNode->key = newKey;

        if (root == nullptr) root = newNode;
        else if (newKey < q->key) q->left = newNode;
        else q->right = newNode;

        return true;
    } 

    bool eraseBST(T deleteKey) {
        Node<T>* p = root;
        Node<T>* q = nullptr;

        // find position of deleteKey
        while (p != nullptr && deleteKey != p->key) {
            q = p;

            if (deleteKey < p->key) p = p->left;
            else p = p->right;
        }

        if (p == nullptr) return false;

        if (p->left != nullptr && p->right != nullptr) { // case of degree 2
            q = p;
            Node<T>* tempNode = p;

            if (height(p->left) < height(p->right) || 
                ((height(p->left) == height(p->right)) && size(p->left) < size(p->right))) {
                p = p->right;
                while (p->left != nullptr) { // min Node of right Subtree (minNode(T) logic)
                    q = p;
                    p = p->left;
                }
            }
            else {
                p = p->left;
                while (p->right != nullptr) { // max Node of left Subtree (maxNode(T) logic)
                    q = p;
                    p = p->right;
                }
            }

            tempNode->key = p->key;
        }
        
        // delete p from T
        if (p->left == nullptr && p->right == nullptr) { // case of degree 0
            if (q == nullptr) root = nullptr; // case of root
            else if (q->left == p) q->left = nullptr;
            else q->right = nullptr;
        }
        else { // case of degree 1
            if (p->left != nullptr) {
                if (q == nullptr) root = root->left; // case of root
                else if (q->left == p) q->left = p->left;
                else q->right = p->left;
            }
            else {
                if (q == nullptr) root = root->right; // case of root
                else if (q->left == p) q->left = p->right;
                else q->right = p->right;
            }
        }

        delete p;

        return true;
    }

    Node<T>* getAVLNode() {
        return new Node<T>*;
    }

    void rotateTree(int rotateType, Node<T>* p, Node<T>* q) {
        Node<T>* a;
        Node<T>* b;
        Node<T>* c;

        switch (rotateType) {
            case LL: // rotateType "LL"
                a = p;
                b = p->left;

                a->left = b->right;
                b->right = a;
                break;
            case RR: // rotateType "RR"
                a = p;
                b = p->right;

                a->right = b->left;
                b->left = a;
                break;
            case LR: // rotateType "LR"
                a = p;
                b = p->left;
                c = b->right;

                b->right = c->left;
                a->left = c->right;
                c->left = b;
                c->right = a;
                b = c;
                break;
            case RL: // rotateType "RL"
                a = p; 
                b = p->right; 
                c = b->left;

                b->left = c->right;
                a->right = c->left;
                c->left = a;
                c->right = b;
                b = c;
                break;
            default:
                break;
        }

        if (q == nullptr) this->root = b;
        else if (a == q->left) q->left = b;
        else if (a == q->right) q->right = b;
    }

public:
    AVL() : root(nullptr) {}
    ~AVL() { clear(); }

    bool insertAVL(T newKey) {
        stack<Node<T>*> stk;

        Node<T>* p = this->root;
        Node<T>* q = nullptr;
        Node<T>* x = nullptr; // unbalanced node
        Node<T>* f = nullptr; // unbalnaced node's parent

        while (p != nullptr) { // find path to inserted node
            if (newKey == p->key) return false;

            q = p;
            stk.push(q);

            if (newKey < p->key) p = p->left;
            else p = p->right;
        } 
        
        Node<T>* t = getBSTNode();
        t->key = newKey;

        if (root == nullptr) root = t;
        else if (newKey < q->key) q->left = t;
        else q->right = t;

        while (!stk.empty()) { // check balance
            q = stk.top();
            stk.pop();

            if (1 < bf(q) || bf(q) < -1) {
                if (x == nullptr) {
                    x = q;
                    if (!stk.empty()) f = stk.top();
                }
            }
        } 

        // rebalance tree
        if (1 < bf(x)) {
            if (0 < bf(x->left)) rotateTree(LL, x, f);
            else rotateTree(LR, x, f);
        }
        else if (bf(x) < -1) {
            if (bf(x->right) < 0) rotateTree(RR, x, f);
            else rotateTree(RL, x, f);
        }

        return true;
    }

    bool eraseAVL(T deleteKey) {
        stack<Node<T>*> stk;

        Node<T>* p = this->root;
        Node<T>* q = nullptr;
        Node<T>* x = nullptr; // unbalanced node
        Node<T>* f = nullptr; // unbalnaced node's parent

        // BST deletion
        while (p != nullptr && deleteKey != p->key) { // find path to deleted node
            q = p;
            stk.push(q);

            if (deleteKey < p->key) p = p->left;
            else p = p->right;
        } 

        if (p == nullptr) return false; // deleteKey was not found

        if (p->left != nullptr && p->right != nullptr) {
            stk.push(p);
            Node<T>* tempNode = p;

            if (height(p->left) < height(p->right) || 
                ((height(p->left) == height(p->right)) && size(p->left) < size(p->right))) {
                p = p->right;
                while (p->left != nullptr) { // min Node of right Subtree (minNode(T) logic)
                    stk.push(p);
                    p = p->left;
                }
            }
            else {
                p = p->left;
                while (p->right != nullptr) { // max Node of left Subtree (maxNode(T) logic)
                    stk.push(p);
                    p = p->right;
                }
            }

            tempNode->key = p->key;
            q = stk.top();
        }

        // delete p from T
        if (p->left == nullptr && p->right == nullptr) { // case of degree 0
            if (q == nullptr) root = nullptr; // case of root
            else if (q->left == p) q->left = nullptr;
            else q->right = nullptr;
        }
        else { // case of degree 1
            if (p->left != nullptr) {
                if (q == nullptr) root = root->left; // case of root
                else if (q->left == p) q->left = p->left;
                else q->right = p->left;
            }
            else {
                if (q == nullptr) root = root->right; // case of root
                else if (q->left == p) q->left = p->right;
                else q->right = p->right;
            }
        }

        delete p;

        while (!stk.empty()) { // check balance
            q = stk.top();
            stk.pop();

            if (1 < bf(q) || bf(q) < -1) {
                x = q;
                if (!stk.empty()) f = stk.top();

                // rebalance tree
                if (1 < bf(x)) {
                    if (0 < bf(x->left)) rotateTree(LL, x, f);
                    else rotateTree(LR, x, f);
                }
                if (bf(x) < -1) {
                    if (bf(x->right) < 0) rotateTree(RR, x, f);
                    else rotateTree(RL, x, f);
                }
            }
        } 

        return true;
    }

    void inorder() { inorder_rec(root); }

    void clear() { clear_rec(root); }

private:
    void inorder_rec(Node<T>* node) {
        if (node == nullptr) return;
        
        cout << "<";
        inorder_rec(node->left);
        cout << " " << node->key << " ";
        inorder_rec(node->right);
        cout << ">";
    }

    void clear_rec(Node<T>* node) {
        if (node == nullptr) return;

        clear_rec(node->left);
        clear_rec(node->right);
        delete node;
    }
};

int main() {
    char opt;
    int key;
    AVL<int> avl;

    while (cin >> opt >> key) {
        if (opt == 'i') {
            if (avl.insertAVL(key)) { avl.inorder(); cout << "\n"; }
            else cout << "i " << key << ": The key already exists\n";
        }   
        else if (opt == 'd') {
            if (avl.eraseAVL(key)) { avl.inorder(); cout << "\n"; }
            else cout << "d " << key << ": The key does not exist\n";
        }   
    }

    avl.clear();
}
