#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <time.h>


using namespace std;

enum Order {
        DESC = 0,
        ASC = 1
};

class Node {
	public:
        Node(){}
        Node(int _key);
        Node(int _key, Node* _prev, Node* _next): key(_key), prev(_prev), next(_next){}
        ~Node(){}
        Node* getNext();
        Node* getPrev();
        void setNext(Node*);
        void setPrev(Node*);
        int getKey();
        string toString();
	private:
        int key;
        Node *next, *prev;
};

class List {
	public:
        List();
        ~List(){}
        Node* locateNode(int);
        string toString(Order);
        bool addNode(int);
        bool removeNode(int);
        void addRandomNodes(int);
		void insertNodes(int, int);
	private:
        Node *head, *tail;
};


int main()
{
        List *lista = new List();
        lista->addNode(4);
        lista->addNode(1);
        lista->addNode(7);
        lista->addNode(-4);
        lista->addNode(-100);
        lista->addNode(-16);
        lista->addNode(1);

        lista->removeNode(-100);
        lista->addRandomNodes(10);

        cout << lista->toString(ASC);
        cout << "_____________________" << endl;
        cout << lista->toString(DESC);

        getchar();
        return(0);
}

//**********************************NODE*********************************

Node::Node(int _key): key(_key) {
        next=NULL;
        prev=NULL;
}

Node* Node::getNext() {
        return(next);
}

Node* Node::getPrev() {
        return(prev);
}

void Node::setNext(Node* _next) {
        next=_next;        
}

void Node::setPrev(Node* _prev) {
        prev=_prev;
}

int Node::getKey() {
        return(key);
}

string Node::toString() {
        stringstream ss;
        ss << "Key: " << key << endl;
        return(ss.str());
}

//**********************************LIST*********************************
List::List() {
        head=NULL;
        tail=NULL;
}

string List::toString(Order order) {
        stringstream ss;
        Node *tmp;
        if (head!=NULL) {
                if (order==ASC) tmp=head; else tmp=tail;
                while (tmp!=NULL) {
                        ss << tmp->toString();
                        if (order==ASC) tmp=tmp->getNext(); else tmp=tmp->getPrev();
                }
        } else ss << "List is empty" << endl;
        return(ss.str());
}

Node* List::locateNode(int key) {
        Node* tmp=head;
        while (tmp!=NULL) {
                if (tmp->getKey()==key) return(tmp);
                tmp=tmp->getNext();
        }
        return(NULL);
}

void List::addRandomNodes(int count) {
        srand((unsigned int)time(0));
        int key=(rand() % 10)+1+tail->getKey(), i=0;
        while (i<count) if(addNode(key)) {
			i++;
			key=(rand() % 10)+1+tail->getKey();
        }
}

void List::insertNodes(int from, int count) {
	int i=from, key=from;
	while (i<count+from) if(addNode(key++)) i++;
}

bool List::removeNode(int key) {
        bool result=false;
        if (locateNode(key)!=NULL) {
                result=true;
                Node* tmp;
                if (head->getKey()==key) {
                        tmp=head;
                        head=head->getNext();
                        head->setPrev(NULL);
                        tmp->~Node();
                } else
                        if (tail->getKey()==key) {
                                tmp=tail;
                                tail=tail->getPrev();
                                tail->setNext(NULL);
                                tmp->~Node();
                        } else {
                                tmp=locateNode(key);
                                Node *left=tmp->getPrev(), *right=tmp->getNext();
                                left->setNext(right);
                                right->setPrev(left);
                                tmp->~Node();
                        }
        }
        return result;
}

bool List::addNode(int key) {
        bool result=false;
        if (locateNode(key)==NULL) {
                result=true;
                Node* tmp;
                if (head==NULL) {
                        tmp=new Node(key);
                        head=tmp;
                        tail=tmp;
                } else {
                        if (head->getKey()>key) {
                                tmp=new Node(key,NULL,head);
                                head->setPrev(tmp);
                                head=tmp;
                        } else
                                if (tail->getKey()<key) {
                                        tmp=new Node(key,tail,NULL);
                                        tail->setNext(tmp);
                                        tail=tmp;
                                } else {
                                        //if (head->getKey()<key && tail->getKey()>key) {                                
											Node *w, *right=NULL, *left=NULL;
                                            w=head;
                                            while (!(key > w->getKey() && key < w->getNext()->getKey())) w=w->getNext();
                                            left=w;
                                            right=w->getNext();
                                            tmp=new Node(key,left,right);
                                            left->setNext(tmp);
                                            right->setPrev(tmp);                                                        
                                        }
                }

        }
        return(result);
}
