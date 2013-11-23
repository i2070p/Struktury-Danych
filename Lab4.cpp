// BST.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <time.h>
#include <math.h>

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
	Node* getGrandParent();
	string getPath();
private:
	Node *left, *right, *parent;
	long key;
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
	void rotateRight(int);
	void rotateLeft(int);
	void makeList();
	void DSW();
	string toString(order);

private:
	Node *root;
	void deleteNode(Node*,int);
	void INOrder(stringstream*, Node*);
	void rotateLeft(Node*);
	void rotateRight(Node*);
	int rightChildCount();
	int getmParam(int);
};

int _tmain(int argc, _TCHAR* argv[])
{
	srand((unsigned int)time(0));
	BST *bst = new BST();

	/*bst->addNode(2);
	bst->addNode(10);
	bst->addNode(18);
	bst->addNode(1);
	bst->addNode(8);
	bst->addNode(9);
	bst->addNode(3);
	bst->addNode(5);
	bst->addNode(4);
	bst->addNode(7);
	bst->addNode(12);
	bst->addNode(400);
	bst->addNode(402);*/
	bst->addRandomNodes(100000);

	cout << "dodano" << endl;

	bst->DSW();

	cout << "wywazono" << endl;
	cout << bst->toString(INORDER)<<endl;
	getchar();
	return 0;
}

//**************************class BST*****************************
BST::BST() {
	root=NULL;
}


int BST::getmParam(int N) {
	int m=pow(2.0,(int)(log((double)N)/log(2.0)))-1;
	return(m);
}

int BST::rightChildCount() {
	Node* tmp = getRoot();
	int i=0;
	while (tmp) {
		tmp=tmp->getRight();
		i++;
	}
	return(i);
}

void BST::DSW() {
	if (getRoot()) {
		makeList();
		int rcc=rightChildCount(), m=getmParam(rcc);
		Node* tmp = getRoot();
		for (int i=0; i<rcc-m && tmp; i++) {
			if (tmp->getRight()) {
				rotateLeft(tmp);
				tmp=tmp->getRight()->getRight();
			}
		}
		while (m>1) {
			m=m/2;
			tmp = getRoot()->getRight();
			for (int i=0; i<m && tmp; i++) {
				rotateLeft(tmp);

				tmp=tmp->getRight()->getRight();
			}
		}
	}
}

void BST::makeList() {
	Node* tmp = getRoot(), *left=NULL; 
	while (tmp) {
		left=tmp->getLeft();
		if (left) {
			rotateRight(left); 
			tmp=left;   
		}
		else tmp=tmp->getRight();
	}
}

void BST::rotateLeft(int key) {
	Node* tmp=locateNode(key);
	if (tmp) rotateLeft(tmp);
}

void BST::rotateLeft(Node* node) {
	if (node && node!=getRoot() && node->getParent()->getRight()) {
		Node *child = node,
			*grandParent = node->getGrandParent(),
			*parent = node->getParent();

		if (grandParent) {
			if (grandParent->getRight()==parent) grandParent->setRight(child);			
			else grandParent->setLeft(child);
		} else setRoot(child);

		parent->setParent(child);
		parent->setRight(child->getLeft());
		if (child->getLeft()) child->getLeft()->setParent(parent);
		child->setLeft(parent);
		child->setParent(grandParent);
	}
}

void BST::rotateRight(int key) {
	Node* tmp=locateNode(key);
	if (tmp) rotateRight(tmp);
}

void BST::rotateRight(Node* node) {
	if (node && node!=getRoot() && node->getParent()->getLeft()) {
		Node *child = node,
			*grandParent = node->getGrandParent(),
			*parent = node->getParent();

		if (grandParent) {
			if (grandParent->getRight()==parent) grandParent->setRight(child);			
			else grandParent->setLeft(child);
		} else setRoot(child);

		parent->setParent(child);
		parent->setLeft(child->getRight());
		if (child->getRight()) child->getRight()->setParent(parent);
		child->setRight(parent);
		child->setParent(grandParent);
	}
}

void BST::deleteNode(Node *n, int depth) {
	++depth;
	Node *parent=n->getParent(), *tmp=NULL;
	if(n->have2Child()) {
		//srand(time(NULL));
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
	int i=0;
	long key=0;
	while (i<count) {
		key=(rand());
		key = key << 16;
		key += rand();
		key=abs(key);
		if(addNode(key)) i++;
		long max_key=getMaxKeyNode(getRoot())->getKey();
	}
}

void BST::INOrder(stringstream *ss, Node *node) {
	if (node) {
		INOrder(ss, node->getLeft());
		*ss << node->getKey() << " ";
		//*ss<< node->getPath() <<endl;
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
	if (getRoot()==NULL) {
		Node *n = new Node(key);
		setRoot(n);
	} else {
		Node *tmp = getRoot(), *tmp_p = NULL;

		while (tmp) {
			if (tmp->getKey()==key) return(false);
			tmp_p=tmp;
			if (key<tmp->getKey()) tmp=tmp->getLeft(); else tmp=tmp->getRight();
		}
		Node *n = new Node(key, tmp_p);
		if (key<tmp_p->getKey()) tmp_p->setLeft(n); else tmp_p->setRight(n);
	}
	return(true);
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
	if(getParent()) ss << " Parent: " << getParent()->getKey();;
	return(ss.str());
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
}

Node::Node(long _key, Node* _parent): key(_key), parent(_parent) {
	left=NULL;
	right=NULL;
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

Node* Node::getGrandParent() {
	if (getParent()==NULL) return(NULL);
	return(parent->getParent());
}
