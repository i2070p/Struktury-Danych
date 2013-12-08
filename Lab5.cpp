// BST.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <time.h>
using namespace std;

enum order {
        INORDER = 0,
        PREORDER = 1
};

class Node {
public:
        Node(long);
        Node(long, Node*);
        ~Node(){}
        long getKey();
        bool isRoot();
        bool have2Child();
        Node* getLeft();
        Node* getRight();
        Node* getParent();
        void setKey(long);
        void setLeft(Node*);
        void setRight(Node*);
        void setParent(Node*);
		int incBalance();
		void setBalance(int);
		int decBalance();
		int getBalance();
        string getPath();
private:
        Node *left, *right, *parent;
        long key;
		int balance;
};

class BST {
public:
        BST();
        ~BST();
        bool addNode(long);
        bool removeNode(long);
        Node* getRoot();
        Node* locateNode(long);
        Node* getPrev(long);
        Node* getNext(long);
        Node* getMaxKeyNode(Node*);
        Node* getMinKeyNode(Node*);
        void setRoot(Node*);
        void addRandomNodes(int);
		void RR(Node*);
		void LL(Node*);
		void RL(Node*);
		void LR(Node*);
		string toString(order);
private:
        Node *root;
        void deleteNode(Node*,int);
        void INOrder(stringstream*, Node*);

};

int _tmain(int argc, _TCHAR* argv[])
{

        BST *bst = new BST();

        bst->addNode(10);
        bst->addNode(2);
        bst->addNode(18);
        bst->addNode(0);
        bst->addNode(1);
        bst->addNode(12);
        bst->addNode(24);
        bst->addNode(9);
        bst->addNode(3);
        bst->addNode(5);
        bst->addNode(4);
        bst->addNode(7);
		//cout << bst->toString(INORDER)<<endl;
      //  bst->removeNode(2);
		//bst->RR(bst->locateNode(0));
	    //bst->LL(bst->locateNode(2));
		//bst->LR(bst->locateNode(2));
		//bst->RL(bst->locateNode(10));
                 cout << "poszlo\n";
        cout << bst->toString(INORDER)<<endl;

        getchar();
        return 0;
}

//**************************class BST*****************************
BST::BST() {
        root=NULL;
}


void  BST::RL(Node* node) {
	LL(node->getRight());
	RR(node);
}

void  BST::LR(Node* node) {
	RR(node->getLeft());
	LL(node);
}

void BST::LL(Node *node) {
	if (node) {
		Node *tmp = node->getLeft(), *parent = node->getParent();
		node->setLeft(tmp->getRight());
		if (node->getLeft()) node->getLeft()->setParent(node);
		tmp->setRight(node);
		tmp->setParent(parent);
		node->setParent(tmp);
		if(parent) {
			if(parent->getLeft() == node) parent->setLeft(tmp); else parent->setRight(tmp);
		} else setRoot(tmp);

		if(tmp->getBalance() == -1) {
			node->setBalance(0);
			tmp->setBalance(0);
		} else {
			node->setBalance(1); 
			tmp->setBalance(-1);
		}
	}

}

void BST::RR(Node *node) {
	Node *tmp = node->getRight(), *parent = node->getParent();
	node->setRight(tmp->getLeft());
	if (node->getRight()) node->getRight()->setParent(node);
	tmp->setLeft(node);
	tmp->setParent(parent);
	node->setParent(tmp);
	if(parent) {
		if(parent->getLeft() == node) parent->setLeft(tmp); else parent->setRight(tmp);
	} else setRoot(tmp);

	if(tmp->getBalance() == -1) {
		node->setBalance(0);
		tmp->setBalance(0);
	} else {
		node->setBalance(-1); 
		tmp->setBalance(1);
	}
}

void BST::deleteNode(Node *n, int depth) {
        ++depth;
        Node *parent=n->getParent(), *tmp=NULL;
        if(n->have2Child()) {
                srand(time(NULL));
                if (rand() % 2) tmp = getPrev(n->getKey()); else tmp = getNext(n->getKey());
                deleteNode(tmp, depth);

                tmp->setLeft(n->getLeft());
                if(tmp->getLeft()) tmp->getLeft()->setParent(tmp);

                tmp->setRight(n->getRight());
                if(tmp->getRight()) tmp->getRight()->setParent(tmp);
        } else if (n->getLeft()) tmp=n->getLeft(); else tmp=n->getRight();

        if(tmp) tmp->setParent(parent);
        if(!parent) setRoot(tmp); else if (parent->getLeft()==n) parent->setLeft(tmp); else parent->setRight(tmp);
        if (depth==0) delete(n);
}

bool BST::removeNode(long key) {
        Node* tmp=locateNode(key);
        if (tmp) deleteNode(tmp,0); else return(false);
        return(true);
}

Node* BST::getMaxKeyNode(Node* node) {
        Node *tmp = node;
        while(tmp->getRight()) tmp=tmp->getRight();
        if (tmp) return(tmp); else return(NULL);
}

Node* BST::getMinKeyNode(Node* node) {
        Node *tmp = node;
        while(tmp->getLeft()) tmp=tmp->getLeft();
        if (tmp) return(tmp); else return(NULL);
}

Node* BST::getNext(long key) {
        Node *tmp = locateNode(key);
        if (tmp) {
                if (tmp->getRight()) return(getMinKeyNode(tmp->getRight()));
                Node *tmp_p=tmp;
                while(tmp && tmp->getLeft()!=tmp_p) {
                        tmp_p=tmp;
                        tmp=tmp->getParent();
                }
        }
        return(tmp);
}

Node* BST::getPrev(long key) {
        Node *tmp = locateNode(key);
        if (tmp) {
                if (tmp->getLeft()) return(getMaxKeyNode(tmp->getLeft()));
                Node *tmp_p=tmp;
                while(tmp && tmp->getRight()!=tmp_p) {
                        tmp_p=tmp;
                        tmp=tmp->getParent();
                }
        }
        return(tmp);
}

Node* BST::locateNode(long key) {
        Node *tmp = getRoot();
        while (tmp) {
                if (tmp->getKey()==key) return(tmp);
                if (key<tmp->getKey()) tmp=tmp->getLeft(); else tmp=tmp->getRight();
        }
        return(NULL);
}

void BST::addRandomNodes(int count) {
         srand((unsigned int)time(0));
        long key=(rand() % 10)+1+getMaxKeyNode(getRoot())->getKey();
                int i=0;
        while (i<count) {
                                
                if(addNode(key)) i++;
                                long max_key=getMaxKeyNode(getRoot())->getKey();
                            
                                key=(rand());
                                key = key << 16;
                                key+=rand()*rand();
        }
}

void BST::INOrder(stringstream *ss, Node *node) {
        if (node) {
                INOrder(ss, node->getLeft());
                //*ss << node->getKey() << " ";
                *ss<< node->getPath() <<endl;
                INOrder(ss, node->getRight());
        }
}

string BST::toString(order _order) {
        stringstream ss;
        switch (_order) {
        case INORDER:
                INOrder(&ss, getRoot());
                break;
        case PREORDER:

                break;
        }
        return(ss.str());
}

bool BST::addNode(long key) {
	Node *n = new Node(key);   
	Node * y = NULL, * z, *x=getRoot();
	while(x) {
		if(x->getKey() == n->getKey()) {
			delete n;
			return(false);
		}
		y = x;
		x = (n->getKey() < x->getKey()) ? x->getLeft() : x->getRight();
	}
	n->setParent(y);
	if(!(n->getParent())) {
		root = n;
		return true;
	}

	if(n->getKey() < y->getKey()) y->setLeft(n); else y->setRight(n);
	if(y->getBalance()) {
		y->setBalance(0);
		return true;
	}
	y->setBalance((y->getLeft() == n) ? 1 : -1);
	z = y->getParent();
	while(z) {
		if(z->getBalance()) break;
		z->setBalance((z->getLeft() == y) ? 1 : -1);
		y = z; z = z-> getParent();
	}

	if(!z) return true;

	if(z->getBalance() == 1) {
		if(z->getRight() == y) {
			z->setBalance(0);
			return true;
		}
		if(y->getBalance() == -1) LR(z); else LL(z);
		return true;
	}
	else {
		if(z->getLeft() == y) {
			z->setBalance(0);
			return true;
		}
		if(y->getBalance() == 1) RL(z); else RR(z);
		return true;
	}
}

void BST::setRoot(Node* _root) {
        root=_root;
}

Node* BST::getRoot() {
        return(root);
}

//**************************class Node****************************
string Node::getPath() {
        stringstream ss;
        Node *tmp = this;
        ss << "[" << getKey() << "]";
        while (tmp->getParent()) {
                tmp=tmp->getParent();
                ss << " -> (" << tmp->getKey() << ")";
        }
        if(getLeft()) ss << " Left: " << getLeft()->getKey();
        if(getRight()) ss << " Right: " << getRight()->getKey();
	    if(getParent()) ss << " Parent: " << getParent()->getKey();
        ss << " Balance: " << getBalance();
        return(ss.str());
}


int Node::incBalance() {
	return(++balance);
}

void Node::setBalance(int _balance) {
	balance=_balance;
}

int Node::decBalance() {
	return(--balance);
}

int Node::getBalance() {
	return(balance);
}

bool Node::isRoot() {
        if (!getLeft() && !getRight()) return(true);
        return(false);
}
bool Node::have2Child() {
        if (getLeft() && getRight()) return(true);
        return(false);
}

Node::Node(long _key): key(_key) {
        left=NULL;
        right=NULL;
        parent=NULL;
		balance=0;
}

Node::Node(long _key, Node* _parent): key(_key), parent(_parent) {
        left=NULL;
        right=NULL;
	    balance=0;
}


long Node::getKey() {
        return(key);
}

Node* Node::getLeft() {
        return(left);
}

Node* Node::getRight() {
        return(right);
}

Node* Node::getParent() {
        return(parent);
}
void Node::setKey(long _key) {
        key=_key;
}

void Node::setLeft(Node* _left) {
        left=_left;
}

void Node::setRight(Node* _right) {
        right=_right;
}

void Node::setParent(Node* _parent) {
        parent=_parent;
}
