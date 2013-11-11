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
	Node(int);
	Node(int, Node*);
	~Node(){}
	int getKey();
	bool isRoot();
	bool have2Child();
	Node* getLeft();
	Node* getRight();
	Node* getParent();
	void setKey(int);
	void setLeft(Node*);
	void setRight(Node*);
	void setParent(Node*);
	string getPath();
private:
	Node *left, *right, *parent;
	int key;
};

class BST {
public:
	BST();
	~BST();
	bool addNode(int);
	bool removeNode(int);
	Node* getRoot();
	Node* locateNode(int);
	Node* getPrev(int);
	Node* getNext(int);
	Node* getMaxKeyNode(Node*);
	Node* getMinKeyNode(Node*);
	void setRoot(Node*);
	void addRandomNodes(int);
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
	bst->addNode(8);
	bst->addNode(9);
	bst->addNode(3);
	bst->addNode(5);
	bst->addNode(4);
	bst->addNode(7);

	cout << bst->toString(INORDER)<<endl;
	bst->removeNode(2);
	cout << bst->toString(INORDER)<<endl;
	bst->removeNode(8);
	cout << bst->toString(INORDER)<<endl;
	getchar();
	return 0;
}

//**************************class BST*****************************
BST::BST() {
	root=NULL;
}

void BST::deleteNode(Node *n,int depth) {
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

bool BST::removeNode(int key) {
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

Node* BST::getNext(int key) {
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

Node* BST::getPrev(int key) {
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

Node* BST::locateNode(int key) {
	Node *tmp = getRoot();
	while (tmp) {
		if (tmp->getKey()==key) return(tmp);
		if (key<tmp->getKey()) tmp=tmp->getLeft(); else tmp=tmp->getRight(); 
	}
	return(NULL);
}

void BST::addRandomNodes(int count) {
	srand((unsigned int)time(0));
	int key=(rand() % 10)+1+getMaxKeyNode(getRoot())->getKey(), i=0;
	while (i<count) {
		if(addNode(key)) i++; 
		int max_key=getMaxKeyNode(getRoot())->getKey();
		if (i % 2 == 0) key=(rand() % 10)+max_key; else key=(rand() % 10)-max_key; 
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

bool BST::addNode(int key) {
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

Node::Node(int _key): key(_key) {
	left=NULL;
	right=NULL;
	parent=NULL;
}

Node::Node(int _key, Node* _parent): key(_key), parent(_parent) {
	left=NULL;
	right=NULL;
}


int Node::getKey() {
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
void Node::setKey(int _key) {
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
