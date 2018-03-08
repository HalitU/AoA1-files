/*
Student Name: Halit Uyanık
Student Number: 150140138

Tested on ssh, cygwin, and visual studio
*/
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <fstream>

using namespace std;
//human class
//used to hold the data from file
//I used this class since otherwise the data on Node class would be quite a lot and unorganized
class Human {
public:
	string name;
	char gender;
	int age;
	Human(string, char, int);
};
//Constructor
Human::Human(string name, char gender, int age) {
	this->name = name;
	this->gender = gender;
	this->age = age;
}
//node class
//Defines nodes in the RB tree
class Node {
public:
	Human *person;
	char colour;
	Node *parent;
	Node *right;
	Node *left;
	//fields for the B) part
	int num_adult;
	int num_non_adult;
	Node();
	Node(Human*, char);
};
//default constructor
Node::Node() {};
//constructor
Node::Node(Human *person, char colour) {
	this->person = person;
	this->colour = colour;
	this->parent = NULL;
	this->right = NULL;
	this->left = NULL;
	this->num_adult = 0;
	this->num_non_adult = 0;
}
//main tree class
//Holds a head pointer for the root of RB tree
class WhiteBlackTree {
public:
	Node *head;
	int memberNumber;
	WhiteBlackTree();
	void ReadFile(char*);
	void insert(Node*);
	void insertToTree(Node*);
	bool compareNames(string, string);
	void caseOne(Node*, Node*, Node*, Node*);
	void leftRotate(Node*);
	void leftRotateTwo(Node*);
	void rightRotate(Node*);
	int updateCounts(Node*, int);
	string n_th_adult(Node *,int);
	string n_th_non_adult(Node *, int);
	Node* n_th_adult_SELECT(Node*, int);
	Node* n_th_non_adult_SELECT(Node*, int);
	void printTree(Node*, int);
};
//constructor
WhiteBlackTree::WhiteBlackTree() {
	this->head = NULL;
	this->memberNumber = 0;
};
//reads the input file
//Creates a new node object and inserts it to the tree
void WhiteBlackTree::ReadFile(char * fileName) {
	ifstream file;
	file.open(fileName);
	string name; 
	char gender; 
	int age;
	while (file >> name >> gender >> age) {
		//cout << name << " " << gender << " " << age << endl;
		Human *newPerson = new Human(name, gender, age);
		Node *newNode = new Node(newPerson, 'B');
		if (newNode->person->age >= 18)
			newNode->num_adult = 1;
		else
			newNode->num_non_adult = 1;
		insert(newNode);
		this->memberNumber++;
	}
};
//inserts the given node into the tree then does modifications
void WhiteBlackTree::insert(Node *x) {
	//first insert the value in to the tree
	insertToTree(x);
	//after inserting the node into the binary tree properly reallocate the RB tree
	x->colour = 'R';

	while (x != this->head && x->parent->colour  == 'R') {
		//cout << "Location check for: " << x->person->name << " " << x->parent->colour << endl;
		Node *self = x;
		Node *parent = x->parent;
		Node *gParent = parent->parent;
		//it is guaranteed to have a black head for the first three nodes
		//since head will always be black
		//zaten siyah olup olmadığını kontrol ediyoruz döngü içinde

		//cout << "Red parent for: " << x->person->name << endl;
		//uncle of node is on the right side
		if (gParent->left == parent) {
			Node *uncle = gParent->right;
			//uncle is black or null
			if (uncle == NULL || (uncle->colour == 'B')) {
				//cout << "Black uncle for: " << x->person->name << endl;

				if (x == parent->right) {
					leftRotate(x);
					//new x will be its old parent
					x = x->left;
				}
				rightRotate(x);
			}
			//uncle is red we only need to recolor no rotation
			else if (uncle->colour == 'R') {
				caseOne(x, uncle, parent, gParent);
				if(gParent != NULL)
					x = gParent;
			}
		}
		//uncle of node is on the left side
		else if (gParent->right == parent) {
			Node *uncle = gParent->left;
			//uncle is black or null
			if (uncle == NULL || (uncle->colour == 'B')) {
				if (x == parent->left)
					rightRotate(x);
				leftRotateTwo(x);
			}
			//uncle is red we only need to color no rotation
			else if (uncle->colour == 'R') {
				caseOne(x, uncle, parent, gParent);
				//set x as grandparent since gParent may violate the rule
				if (gParent != NULL)
					x = gParent;
			}
		}
		//head needs to be black after rotations
		this->head->colour = 'B';
	}
	//update the counters
	updateCounts(this->head, -1);
	//head needs to be black
	this->head->colour = 'B';
};
//counter update for adult and non_adult numbers
//recursively updates the values from leaf to root
int WhiteBlackTree::updateCounts(Node *x, int LR) {
	if (x == NULL)
		return 0;
	//total counts for this node
	int total_adult = 0;
	int total_non_adult = 0;

	//taking itself first
	if (x->person->age >= 18)
		total_adult += 1;
	else
		total_non_adult += 1;
	
	//then recursively take all subtrees starting from this node
	total_adult += updateCounts(x->left, 0);
	total_adult += updateCounts(x->right, 0);
	total_non_adult += updateCounts(x->left, 1);
	total_non_adult += updateCounts(x->right, 1);

	x->num_adult = total_adult;
	x->num_non_adult = total_non_adult;


	if (LR == 0)
		return total_adult;
	else
		return total_non_adult;
}
//Case 1 modification for the RB tree
void WhiteBlackTree::caseOne(Node *x, Node *uncle, Node *parent, Node *gParent) {
	//cout << "Red uncle for: " << x->person->name << endl;
	uncle->colour = 'B';
	parent->colour = 'B';
	gParent->colour = 'R';
}
//inserts the new node into the binary tree
void WhiteBlackTree::insertToTree(Node *node) {
	if (this->head == NULL) {
		//cout << node->person->name << " is at head" << endl;
		this->head = node;
		return;
	}
	//search for a position
	Node *crr = this->head;
	Node *pss = this->head;
	while (crr != NULL) {
		pss = crr;
		if (this->compareNames(node->person->name, crr->person->name)) {
			//cout << node->person->name << " is at right" << endl;
			if (crr->right == NULL) {
				node->parent = pss;
				pss->right = node;
				break;
			}
			crr = crr->right;
		}
		else {
			//cout << node->person->name << " is at left" << endl;
			if (crr->left == NULL) {
				node->parent = pss;
				pss->left = node;
				break;
			}
			crr = crr->left;
		}
	}
	//cout << this->head->person->name << endl;
};
//Left Rotation for case 2 and case2-3
void WhiteBlackTree::leftRotate(Node *node) {
	Node *gParent = node->parent->parent;
	Node *parent = node->parent;
	//left rotate
	gParent->left = node;
	node->parent = gParent;

	if (node->left != NULL) {
		parent->right = node->left;
		node->left->parent = parent;
	}
	else {
		parent->right = NULL;
	}

	node->left = parent;
	parent->parent = node;
}
/*
Left Rotation for 3 node case
This happens when we need to rotate left and the grandparent is also involved
*/
void WhiteBlackTree::leftRotateTwo(Node *node) {
	Node *parent = node->parent;
	Node *gParent = parent->parent;
	Node *ggParent = gParent->parent;
	Node *parentLeft = parent->left;

	if (ggParent->right == gParent) {
		ggParent->right = parent;
		parent->parent == ggParent;
	}
	else if (ggParent->left == gParent) {
		ggParent->left = parent;
		parent->parent = ggParent;
	}

	parent->left = gParent;
	gParent->parent = parent;

	gParent->right = parentLeft;
	if (parentLeft != NULL)
		parentLeft->parent = gParent;

	//recolor
	parent->colour = 'B';
	gParent->colour = 'R';
}
/*
Right Rotation
This rotates 3 nodes self, parent, grandparent to right
as shown in class slides
*/
void WhiteBlackTree::rightRotate(Node *node) {
	//initialize again just in case x changed because of left rotation
	Node *parent = node->parent;
	Node *gParent = parent->parent;
	Node *ggParent = gParent->parent;
	Node *parentRight = parent->right;
	//right rotation
	if (ggParent->right == gParent) {
		ggParent->right = parent;
		parent->parent = ggParent;
	}
	else {
		ggParent->left = parent;
		parent->parent = ggParent;
	}

	parent->right = gParent;
	gParent->parent = parent;

	gParent->left = parentRight;
	if (parentRight != NULL)
		parentRight->parent = gParent;

	//recolor
	parent->colour = 'B';
	gParent->colour = 'R';
}
/*
Compare two key values.
*/
bool WhiteBlackTree::compareNames(string a, string b) {
	//take the comparison size as the smaller length
	int lenA = a.length();
	int lenB = b.length();
	int cmpLen = 0;
	if (lenA > lenB)
		cmpLen = lenB;
	else
		cmpLen = lenA;
	//cout << a << " " << b << " " << cmpLen << endl;
	//compare two strings char by char
	for (int i = 0; i < cmpLen; i++) {
		if (a[i] > b[i]) {
			return true;
		}
		else if (a[i] < b[i])
			return false;
	}
	return false;
};
/*
Print the tree via pre-order traversal
*/
void WhiteBlackTree::printTree(Node *node, int height) {
	//cout << height << endl;
	if (node == NULL) {
		//cout << "dead!" << endl;
		return;
	}
	//put '-' symbol
	for (int i = 0; i < height; i++) {
		cout << '-';
	}
	cout << '(' << node->colour << ')' << node->person->name << " " << node->person->gender << " " << node->person->age << " --- " << node->num_adult << " " << node->num_non_adult << "\n";
	//cout << "calling left" << endl;
	printTree(node->left, height+1);
	//cout << "calling right" << endl;
	printTree(node->right, height+1);
};
/*
Calls the select function for wanted ith adult and returns its name
*/
string WhiteBlackTree::n_th_adult(Node *x, int i) {
	return n_th_adult_SELECT(x, i)->person->name;
}
/*
Calls the select function for wanted ith non_adult and returns its name
*/
string WhiteBlackTree::n_th_non_adult(Node *x, int i) {
	return n_th_non_adult_SELECT(x, i)->person->name;
}
/*
Recursively tries to find the ith adult in the tree
Main functionality is to be able to benefit from child and parent
adult, non_adult numbers to slice search are for the node.
*/
Node* WhiteBlackTree::n_th_adult_SELECT(Node *x, int i) {
	//cout << "Checking: " << x->person->name << " for adult: " << i << endl;
	//get left and right amounts if exists
	int left = 0, right = 0;
	if (x->left != NULL)
		left = x->left->num_adult;
	if (x->right != NULL)
		right = x->right->num_adult;
	//check if x is adult or not
	int adult = 0;
	if (x->person->age >= 18)
		adult++;
	//check if self is result
	if (i - left - adult == 0 && adult == 1)
		return x;
	//otherwise diverge to tree
	if (left >= i)
		return n_th_adult_SELECT(x->left, i);
	else
		return n_th_adult_SELECT(x->right, i - left - adult);
}
/*
Similar to the adult search.
*/
Node* WhiteBlackTree::n_th_non_adult_SELECT(Node *x, int i) {
	//cout << "Checking: " << x->person->name << " for non_adult: " << i << endl;
	//get left and right amounts if exists
	int left = 0, right = 0;
	if (x->left != NULL)
		left = x->left->num_non_adult;
	if (x->right != NULL)
		right = x->right->num_non_adult;
	//check if x is adult or not
	int non_adult = 0;
	if (x->person->age < 18)
		non_adult++;
	//check if self is result
	if (i - left - non_adult == 0 && non_adult == 1)
		return x;
	//otherwise diverge to tree
	if (left >= i)
		return n_th_non_adult_SELECT(x->left, i);
	else
		return n_th_non_adult_SELECT(x->right, i - left - non_adult);
}
//main function
int main(int argc, char *argv[]) {
	WhiteBlackTree *newRB = new WhiteBlackTree();
	newRB->ReadFile(argv[1]);
	newRB->printTree(newRB->head, 0);
	cout << "5th adult: " << newRB->n_th_adult(newRB->head, 5) << endl;
	cout << "3rd non-adult: " << newRB->n_th_non_adult(newRB->head, 3) << endl;
};