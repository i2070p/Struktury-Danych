// KopiecSprzężony.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <math.h>
#include <queue>
#include <stack>
#include <sstream>
#include <time.h>

#define swapAdress(a,b)	Node*tmp=b;b=a;a=tmp;

using namespace std;

enum DIRECTION {
	L,
	R
};

enum SIDE {
	MIN,
	MAX
};

class RootNode {
public:
	RootNode();
	~RootNode(){};
	RootNode *left, *right; //left - minimum, right - maksimum
	bool isLeaf();
	bool haveOnlyLeft();
};

RootNode::RootNode() {
	left=NULL;
	right=NULL;
}


bool RootNode::isLeaf() {
	if (!right && !left) return(true);
	return(false);
}

bool RootNode::haveOnlyLeft() {
	if (left && !right) return(true);
	return(false);
}

class Node: public RootNode {
public:
	Node();
	Node(int, Node*);
	~Node(){};
	int key;
	Node *parent;
	Node* getLeft();
	Node* getRight();
};

Node::Node() {
	parent=NULL;
	left=NULL;
	right=NULL;
}
Node* Node::getLeft() {
	return((Node*)left);
}

Node* Node::getRight() {
	return((Node*)right);
}

Node::Node(int _key, Node* _parent): key(_key), parent(_parent) {
}

class DeapHeap {
public:
	DeapHeap();
	~DeapHeap(){};
	void addNode(int);
	void removeMaxKey();
	void removeMinKey();
	void addRandomNodes(int);
	string toString();
private:
	void swap(Node*, Node*);
	Node *getMin();
	Node *getMax();
	RootNode *root;
	Node* searchCousin(Node*, SIDE&);
	Node* searchLast();
};

Node* DeapHeap::getMin() {
	return((Node*)root->left);
}
Node* DeapHeap::getMax() {
	return((Node*)root->right);
}

void DeapHeap::swap(Node* A, Node* B) {
	int tmpKey = A->key;
	A->key=B->key;
	B->key=tmpKey;

	// i inne pola jakby były
}

Node* DeapHeap::searchCousin(Node* node, SIDE& side) {
	Node *tmp = node;
	stack<DIRECTION> wayPoints;

	while (tmp->parent!=root) {
		if (tmp->parent->left == tmp) wayPoints.push(L);
		else wayPoints.push(R);
		tmp=tmp->parent;
	}

	if (tmp==getMin()) {
		tmp=getMax();
		side=MIN;
	} else {
		tmp=getMin();
		side=MAX;
	}

	while (!wayPoints.empty() && !tmp->isLeaf()) {
		if (wayPoints.top() == L) tmp=tmp->getLeft(); else tmp=tmp->getRight();
		wayPoints.pop();
	}

	return(tmp);
}

DeapHeap::DeapHeap() {
	root=new RootNode();
}

void DeapHeap::addNode(int _key) {
	//<Faza I>
	Node *newNode;
	queue<RootNode*> q;
	q.push(root);
	RootNode *tmp=NULL;

	while (!q.empty())  { // wstawienie wezla oparte o algorytm przechodzenia drzewa - levelorder

		tmp = q.front();
		if (tmp->isLeaf()) {
			newNode = new Node(_key, (Node*)tmp); 
			tmp->left=newNode;
			break;
		} 
		if (tmp->haveOnlyLeft()) {
			newNode = new Node(_key, (Node*)tmp); 
			tmp->right=newNode;
			break;
		}

		q.pop();
		if (tmp->left) q.push(tmp->left);
		if (tmp->right) q.push(tmp->right);
	}
	
	//<Faza II>
	SIDE side; // strona po ktorej wstawiony element
	Node* cousin = searchCousin(newNode, side);

	if (cousin) { 
		if (side == MAX) { //ustalenie "sytuacji" wstawionego wezla oraz jego relacji z kuzynem
			if (!(newNode->key >= cousin->key && newNode->key <= newNode->parent->key) && (newNode->key < cousin->key)) {
				swap(newNode, cousin);
				newNode=cousin;
				//swapAdress(newNode, cousin);
				side=MIN;
			}
		} else {
			if (!(newNode->key <= cousin->key && newNode->key >= newNode->parent->key) && (newNode->key > cousin->key)) {
				swap(newNode, cousin);
				newNode=cousin;
				//swapAdress(newNode, cousin);
				side=MAX;
			}
		}

		//<Faza III>
		Node *nTmp = newNode; // "poprawianie w gore"
		while (nTmp->parent!=root) {
			if (side == MAX) { 
				if (nTmp->parent->key < nTmp->key) swap(nTmp, nTmp->parent);
			} else if (nTmp->parent->key > nTmp->key) swap(nTmp, nTmp->parent);
			nTmp=nTmp->parent;
		}

	}
}

Node* DeapHeap::searchLast() {
	queue<RootNode*> q;
	q.push(root);
	RootNode *tmp=NULL;
	while (!q.empty())  {
		tmp = q.front();
		q.pop();
		if (tmp->left) q.push(tmp->left);
		if (tmp->right) q.push(tmp->right);
	}
	return((Node*)tmp);
}

string DeapHeap::toString() {
	stringstream ss;
	queue<RootNode*> q;
	q.push(root);
	RootNode *tmp=NULL;
	int counter=0, power=1;
	while (!q.empty())  {
		tmp = q.front();
		q.pop();
		if(tmp!=root) ss << " " << ((Node*)tmp)->key;
		//if(tmp->left) ss << " L: " <<((Node*)tmp->left)->key;
		//if(tmp->right) ss << " R: " <<((Node*)tmp->right)->key;
		if (counter==pow(2.0f,power)) {
			ss << endl;
			power++;
			counter=0;
		}
		if (tmp->left) q.push(tmp->left);
		if (tmp->right) q.push(tmp->right);
		counter++;
	}
	ss << endl;
	return(ss.str());
}

void DeapHeap::removeMaxKey() {
	Node *maxRoot=getMax();

	if (maxRoot) 
		if (maxRoot->isLeaf()) {
			root->right=NULL;
			delete maxRoot;
		} else {

			Node *last=searchLast();
			swap(maxRoot, last);
			if (last->parent->left==last) last->parent->left=NULL; else last->parent->right=NULL;
			delete last;
			Node *beg=maxRoot; 

			while (!beg->isLeaf()) {
				Node* tmp=beg;
				bool A=beg->getLeft() && beg->getLeft()->key >= beg->key;
				bool B=beg->getRight() && beg->getRight()->key >= beg->key;
				if (A && B) {
					beg=(beg->getLeft()->key > beg->getRight()->key)?beg->getLeft():beg->getRight();
					swap(tmp, beg);
				} else if (A) {
					beg=beg->getLeft(); 
					swap(tmp, beg);
				} else if (B) { 
					beg=beg->getRight();
					swap(tmp, beg);
				} else break;
			}
		}
}

void DeapHeap::removeMinKey() {
	Node *minRoot=getMin();
	cout << minRoot->getLeft()->key << endl;
	if (minRoot) 
		if (minRoot->isLeaf()) {
			root->left=NULL;
			delete minRoot;
		} else {
			Node *last=searchLast();
			swap(minRoot, last);
			if (last->parent->left==last) last->parent->left=NULL; else last->parent->right=NULL;
			delete last;
			Node *beg=minRoot; 
			while (!beg->isLeaf()) {
				Node* tmp=beg;
				bool A=beg->getLeft() && beg->getLeft()->key <= beg->key;
				bool B=beg->getRight() && beg->getRight()->key <= beg->key;
				if (A && B) {
					beg=(beg->getLeft()->key < beg->getRight()->key)?beg->getLeft():beg->getRight();
					swap(tmp, beg);
				} else if (A) {
					beg=beg->getLeft(); 
					swap(tmp, beg);
				} else if (B) { 
					beg=beg->getRight();
					swap(tmp, beg);
				} else break;
			}
			SIDE side;
			Node* cousin = searchCousin(beg, side);
			if (beg->key > cousin->key) swap(beg, cousin);
		}
}
	

void DeapHeap::addRandomNodes(int count) {
	for (int i=0; i<count; i++) {
		addNode(rand() % 100);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(0));
	DeapHeap DH;

	DH.addNode(5);	
	DH.addNode(45);
	DH.addNode(10);
	DH.addNode(8);
	DH.addNode(25);
	DH.addNode(40);
	DH.addNode(15);
	DH.addNode(19);
	DH.addNode(9);
	DH.addNode(30);
	DH.addNode(20);
	DH.addNode(50);
	//DH.addNode(50);
	//DH.addNode(2);
	//DH.addNode(1);
	DH.addNode(2);
	//DH.addNode(50);
	cout << DH.toString();
	DH.removeMinKey();
		DH.removeMinKey();
			DH.removeMinKey();
				DH.removeMaxKey();
					DH.removeMaxKey();
						DH.removeMinKey();
							DH.removeMinKey();
								DH.removeMinKey();	DH.removeMinKey();	DH.removeMinKey();	DH.removeMinKey();
	cout << DH.toString();
	system("pause");
	return 0;
}

