// AVL.cpp : Defines the entry point for the console application.
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

class AVL {
public:
        AVL();
        ~AVL();
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
		int nodesCount();
private:
        Node *root;
        Node* deleteNode(Node*);
        void INOrder(stringstream*, Node*);
		void computeNodes(int*, Node*);
};

int _tmain(int argc, _TCHAR* argv[])
{

        AVL *avl = new AVL();

        avl->addNode(10);
        avl->addNode(2);
        avl->addNode(18);
        avl->addNode(0);
        avl->addNode(1);
        avl->addNode(12);
        avl->addNode(24);
        avl->addNode(9);
        avl->addNode(3);
        avl->addNode(5);
        avl->addNode(4);
        avl->addNode(7);
		avl->addRandomNodes(100000);
        cout << "Poszlo\n";
		cout << "Liczba wezlow na drzewie: " << avl->nodesCount() << endl;
        avl->removeNode(10);
		avl->removeNode(7);
		avl->removeNode(5);
		cout << avl->getRoot()->getKey() << endl;
		avl->removeNode(avl->getRoot()->getKey());
		cout << "Liczba wezlow na drzewie: " << avl->nodesCount() << endl;

        //cout << avl->toString(INORDER)<<endl;

        getchar();
        return 0;
}

//**************************class AVL*****************************
AVL::AVL() {
        root=NULL;
}


int AVL::nodesCount() {
        int i=0;
        computeNodes(&i, getRoot());
        return(i);
}

void AVL::computeNodes(int* count, Node* node) {
        if (node) {
                computeNodes(count, node->getLeft());
                                (*count)++;
                computeNodes(count, node->getRight());
        }
}


void  AVL::RL(Node* node) {
  //LL(node->getRight());
  //RR(node);
  Node * L = node->getRight(), * LR = L->getLeft(), *parent = node->getParent();
  L->setLeft(LR->getRight());
  if(L->getLeft()) L->getLeft()->setParent(L);
  node->setRight(LR->getLeft());
  if(node->getRight()) node->getRight()->setParent(node);
  LR->setLeft(node);
  LR->setRight(L);
  node->setParent(LR);
  L->setParent(LR);
  LR->setParent(parent);
  if(parent) {
    if(parent->getLeft() == node) parent->setLeft(LR); else parent->setRight(LR);
  } else setRoot(LR);

  if (LR->getBalance() == -1 ) node->setBalance(1); else node->setBalance(0);
  if (LR->getBalance() == 1) L->setBalance(-1); else L->setBalance(0);
  LR->setBalance(0);
}

void  AVL::LR(Node* node) {
  //RR(node->getLeft());
  //LL(node);
  Node * L = node->getLeft(), * LR = L->getRight(), *parent = node->getParent();
  L->setRight(LR->getLeft());
  if(L->getRight()) L->getRight()->setParent(L);
  node->setLeft(LR->getRight());
  if(node->getLeft()) node->getLeft()->setParent(node);
  LR->setRight(node);
  LR->setLeft(L);
  node->setParent(LR);
  L->setParent(LR);
  LR->setParent(parent);
  if(parent) {
    if(parent->getLeft() == node) parent->setLeft(LR); else parent->setRight(LR);
  } else setRoot(LR);

  if (LR->getBalance() == 1 ) node->setBalance(-1); else node->setBalance(0);
  if (LR->getBalance() == -1) L->setBalance(1); else L->setBalance(0);
  LR->setBalance(0);
}

void AVL::LL(Node *node) {
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

		if(tmp->getBalance() == 1) {
			node->setBalance(0);
			tmp->setBalance(0);
		} else {
			node->setBalance(1); 
			tmp->setBalance(-1);
		}
	}

}

void AVL::RR(Node *node) {
	if (node) {
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
}

Node* AVL::deleteNode(Node *node) {
	Node *tmp, *tmp_p, *tmp2;
	bool b;
	if((node->getLeft()) && (node->getRight())) {
		tmp_p = deleteNode(getPrev(node->getKey()));
		b = false;
	}
	else {
		if(node->getLeft()) {
			tmp_p = node->getLeft(); node->setLeft(NULL);
		} else {
			tmp_p = node->getRight(); node->setRight(NULL);
		}
		node->setBalance(0);
		b = true;
	}
	if(tmp_p) {
		tmp_p->setParent(node->getParent());
		tmp_p->setLeft(node->getLeft());
		if(tmp_p->getLeft())  tmp_p->getLeft()->setParent(tmp_p);
		tmp_p->setRight(node->getRight());
		if(tmp_p->getRight()) tmp_p->getRight()->setParent(tmp_p);
		tmp_p->setBalance(node->getBalance());
	}
	if(node->getParent()) {
		if(node->getParent()->getLeft() == node) node->getParent()->setLeft(tmp_p); else node->getParent()->setRight(tmp_p);
	}
	else root = tmp_p;
	if(b) {
		tmp2 = tmp_p;
		tmp_p = node->getParent();
		while(tmp_p) {
			if(!(tmp_p->getBalance())) {
				tmp_p->setBalance((tmp_p->getLeft() == tmp2) ? -1 : 1);
				break;
			}
			else {
				if(((tmp_p->getBalance() ==  1) && (tmp_p->getLeft()  == tmp2)) || ((tmp_p->getBalance() == -1) && (tmp_p->getRight() == tmp2))) {
					tmp_p->setBalance(0);
					tmp2 = tmp_p; tmp_p = tmp_p->getParent();
				}
				else {
					tmp = (tmp_p->getLeft() == tmp2) ? tmp_p->getRight() : tmp_p->getLeft();
					if(!(tmp->getBalance())) {
						if(tmp_p->getBalance() == 1) LL(tmp_p); else RR(tmp_p);
						break;                      
					}
					else if(tmp_p->getBalance() == tmp->getBalance()) {
						if(tmp_p->getBalance() == 1) LL(tmp_p); else RR(tmp_p);
						tmp2 = tmp; tmp_p = tmp->getParent();            
					}
					else {
						if(tmp_p->getBalance() == 1) LR(tmp_p); else RL(tmp_p);
						tmp2 = tmp_p->getParent(); tmp_p = tmp2->getParent();              
					}
				}
			}
		}
	}
	return node;
}



bool AVL::removeNode(long key) {
        Node* tmp=locateNode(key);
        if (tmp) deleteNode(tmp); else return(false);
        return(true);
}

Node* AVL::getMaxKeyNode(Node* node) {
        Node *tmp = node;
        while(tmp->getRight()) tmp=tmp->getRight();
        if (tmp) return(tmp); else return(NULL);
}

Node* AVL::getMinKeyNode(Node* node) {
        Node *tmp = node;
        while(tmp->getLeft()) tmp=tmp->getLeft();
        if (tmp) return(tmp); else return(NULL);
}

Node* AVL::getNext(long key) {
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

Node* AVL::getPrev(long key) {
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

Node* AVL::locateNode(long key) {
        Node *tmp = getRoot();
        while (tmp) {
                if (tmp->getKey()==key) return(tmp);
                if (key<tmp->getKey()) tmp=tmp->getLeft(); else tmp=tmp->getRight();
        }
        return(NULL);
}

void AVL::addRandomNodes(int count) {
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

void AVL::INOrder(stringstream *ss, Node *node) {
        if (node) {
                INOrder(ss, node->getLeft());
                *ss << node->getKey() << " ";
                //*ss<< node->getPath() <<endl;
                INOrder(ss, node->getRight());
        }
}

string AVL::toString(order _order) {
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

/* schemat dodawania 

1.Wstawiamy wezel tradycyjnie jak w AVL

2.Sprawdzamy czy rodzic wstawionego wezla ma balance!=0 jezeli tak to ustawiamy jego balance na 0 i konczymy (bo nastapila rownowaga)

3.Jezeli rodzic wstawionego wezla ma balance==0 to ustawiamy je na 1 (jezeli wstawiony wezel jest jego lewym potomkiem) lub -1 (jezeli wstawiony wezel jest jego prawym potomkiem)

4.Rozpoczynamy wedrowke od rodzica nowo-wstawionego wezla az do rodzica roota (NULL'a). Wedrowka przerywa sie w momencie gdy dotrzemy do celu
lub gdy balance ktoregos z napotkanych wezlow jest rozne od 0
po tej operacji musimy miec 2 informacje (wezel ktory wedrowal (tmp_p_p) i jego dziecko (tmp)

5.Jezeli wedrowka dobiegla szczesliwie do konca (bez przerwania czyli dotarla az do rodzica roota (NULL'a)) to znaczy ze wszystkie napotkane wezly sa w stanie rownowagi (balance==0)
stad wynika ze drzewo jest wywazone wiec konczymy (return(true);)

6.Jezeli wedrowka nie dobiegla szczesliwie do konca to sprawdzamy czy balance wezla na ktorym sie zatrzymala (nazywany dalej wezlem A) jest rowny 1
-jezeli jest rowny 1 to sprawdz czy prawy potomek wezla A jest wezlem tmp jezeli tak to ustaw balance wezla A na 0 i zakoncz operacje wstawiania w przeciwnym wypadku sprawdz czy balance wezla tmp jest rowne 1 jezeli jest to wykonaj rotacje LL(A) jezeli nie to rotacje LR(A)
-jezeli jest rowny -1 to sprawdz czy lewy potomek wezla A jest wezlem tmp jezeli tak to ustaw balance wezla A na 0 i zakoncz operacje wstawiania w przeciwnym wypadku sprawdz czy balance wezla tmp jest rowne -1 jezeli jest to wykonaj rotacje RR(A) jezeli nie to rotacje RL(A)

*/
bool AVL::addNode(long key) {
	//<1>
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
	 //</1>
     //<2>
		if (tmp_p->getBalance()) {
			tmp_p->setBalance(0);
			return(true);
		}
     //</2>
     
	 //<3>
		if (tmp_p->getLeft()==n) tmp_p->setBalance(1); else tmp_p->setBalance(-1);
		//tmp_p->setBalance((tmp_p->getLeft()==n)?1:-1);
	 //</3>	
	 //<4>
		Node * tmp_p_p=tmp_p->getParent();
		tmp=tmp_p;
		while (tmp_p_p && !tmp_p_p->getBalance()) {
			if (tmp_p_p->getLeft()==tmp) tmp_p_p->setBalance(1); else tmp_p_p->setBalance(-1);
			//tmp_p_p->setBalance((tmp_p_p->getLeft()==tmp)?1:-1);
			tmp=tmp_p_p;
			tmp_p_p=tmp_p_p->getParent();
		}
	 //</4>	
	 //<5>
		if(tmp_p_p==NULL) return(true);
     //</5>
	 //<6>
		if (tmp_p_p->getBalance()==1) {
			if (tmp_p_p->getRight()==tmp) {
				tmp_p_p->setBalance(0);
				return(true);
			}
			if (tmp->getBalance()==1) LL(tmp_p_p); else LR(tmp_p_p);
			return(true);
		} else {
			if (tmp_p_p->getLeft()==tmp) {
				tmp_p_p->setBalance(0);
				return(true);
			}
			if (tmp->getBalance()==-1) RR(tmp_p_p); else RL(tmp_p_p);
			return(true);
		}
	 //</6>
	}
}

void AVL::setRoot(Node* _root) {
        root=_root;
}

Node* AVL::getRoot() {
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
