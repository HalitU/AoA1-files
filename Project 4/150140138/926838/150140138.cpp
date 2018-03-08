/*
Student Name: Halit Uyanık
Student Number: 150140138
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>

using namespace std;
//main class
class myHash {
public:
	int _tablesize;
	int collision = 0;
	myHash(int tablesize);
	void insert(string s);
	bool isSame(long long int, string);
	long long int hashFunc(string s);
	bool retrieve(string s);
	bool spell_checker(string s);
	bool remove(string s);
	void printCollision(long long int);
	void printOutput();
	void readFile(char *);
private:
	string *myHashTable;
	int totalEntries = 0;
};
//constructor
myHash::myHash(int tablesize) {
	this->_tablesize = tablesize;
	this->myHashTable = new string[this->_tablesize];
}
//insert method
void myHash::insert(string s) {
	long long int collision = 0;
	long long int index = hashFunc(s);
	long long int pss_index = index;
	//checks the location for the same string
	if (isSame(index, s)) {
		cout << "WARNING: The word '" << s << "' is already in the dictionary!" << endl;
		return;
	}
	//if the location is empty or previously deleted input the there
	if (this->myHashTable[index].empty() || this->myHashTable[index] == "*") {
		cout << "h(x) result is " << pss_index << " for: " << s << endl;
		cout << "INSERT: The word '" << s << "' is put in the cell number " << index << endl;
		this->myHashTable[index] = s;
		printCollision(collision);
		return;
	}
	//else search for an empty space
	else {
		bool add = true;
		while (!this->myHashTable[index].empty() && this->myHashTable[index] != "*") {
			collision++;
			index++;
			if (index == pss_index) {
				cout << "WARNING: There are no empty space in the hash table for the word: " << s << endl;
				add = false;
				printCollision(collision);
				break;
			}
			//if the index reaches size then we need to continue from beginning
			if (index == _tablesize) {
				index = 0;
			}
			if (isSame(index, s)) {
				cout << "WARNING: The word '" << s << "' is already in the dictionary!" << endl;				printCollision(collision);
				add = false;
				break;
			}
		}
		if (add) {
			cout << "h(x) result is " << pss_index << " for: " << s << endl;
			cout << "INSERT: The word '" << s << "' is put in the cell number " << index << endl;
			this->myHashTable[index] = s;
			printCollision(collision);
			return;
		}
	}
}
//COLLISION print method
void myHash::printCollision(long long int collision) {
	cout << "COLLISIONS :" << collision << endl;
	this->collision += collision;
}
//checks two strings are same
bool myHash::isSame(long long int position, string s) {
	if (myHashTable[position].compare(s) == 0)
		return true;
	return false;
}
//calculates the initial position
long long int myHash::hashFunc(string s) {
	long long int result = 1;
	for (int i = 0; i < s.size(); i++) {
		result *= (s[i] % this->_tablesize);
		result = (result % this->_tablesize);
	}
	return result;
}
//retrieve method
bool myHash::retrieve(string s) {
	long long int collisions = 0;
	long long int location = hashFunc(s);
	long long int loc_pss = location;
	if (isSame(location, s)) {
		cout << "RETRIEVE: The word '" << s << "' found in the dictionary with index: " << location << endl;
		printCollision(collisions);
		return true;
	}
	while (!this->myHashTable[location].empty()) {
		collisions++;
		location++;
		if (location == _tablesize) {
			location = 0;
		}
		if (location == loc_pss) {
			cout << "WARNING: The word '" << s << "' couldn't be found in the dictionary" << endl;
			printCollision(collisions);
			spell_checker(s);
			break;
		}
		if (isSame(location, s)) {
			cout << "RETRIEVE: The word '" << s << "' found in the dictionary with index: " << location << endl;
			printCollision(collisions);
			break;
		}
	}
	return false;
}
//checks the type mistake for all characters between ASCII decimal 97('a') to 122('z')
bool myHash::spell_checker(string s) {
	string pss = s;
	long long int pss_loc = 0;
	long long int location = 0;
	cout << "SUGGESTIONS for " << s << ":";
	//we will search for all 26 different characters for each slot
	for (int i = 0; i < s.size(); i++) {
		s = pss;
		//there are 26 characters in english alphabet
		for (int j = 97; j < 122; j++) {
			s[i] = j;
			location = hashFunc(s);
			pss_loc = location;
			//after making the change control the map for item
			while (!this->myHashTable[location].empty()) {
				if (isSame(location, s)) {
					cout << " " << s << ", ";
				}
				location++;
				if (location == _tablesize)
					location = 0;
				if (location == pss_loc)
					break;
			}
		}
	}
	cout << endl;
	return true;
}
//deletion method
bool myHash::remove(string s) {
	long long int index = hashFunc(s);
	long long int pss_index = index;
	long long int collisions = 0;
	//at deletion previously deleted items are treated as not empty
	while (!this->myHashTable[index].empty()) {
		if (isSame(index, s)) {
			cout << "REMOVE: The word '" << s << "' is removed from the dictionary." << endl;			this->myHashTable[index] = "*";
			printCollision(collisions);
			return true;
		}
		collisions++;
		index++;
		if (index == _tablesize) {
			index = 0;
		}
		if (index == pss_index) {
			cout << "WARNING: The word '" << s << "' couldn't be found in the dictionary" << endl;
			printCollision(collisions);
			return false;
		}
	}
	return false;
}
//print the hash table to 150140138_output.txt file
void myHash::printOutput() {
	ofstream f;
	f.open("150140138_output.txt");
	for (int i = 0; i < this->_tablesize; i++) {
		f << i << ": " << this->myHashTable[i] << endl;
	}
}

//read the file line by line and process operands and words
void myHash::readFile(char * file_name) {
	FILE *f = fopen(file_name, "r");
	char ch;
	string operand, word;
	bool isOp = true;
	while (!feof(f)) {
		ch = fgetc(f);
		if (ch == '\n') {
			isOp = true;
			cout << "-------------------------------" << endl;
			//before reading the next line do the operations
			if (operand.compare("insert") == 0) {
				this->insert(word);
			}
			else if (operand.compare("delete") == 0) {
				this->remove(word);
			}
			else if (operand.compare("retrieve") == 0) {
				this->retrieve(word);
			}
			operand = "";
			word = "";
		}
		else {
			if (ch == ':') {
				isOp = false;
			}else if (isOp) {
				operand += ch;
			}else {
				word += ch;
			}
		}
	}
	cout << "=============================" << endl;
	cout << "TOTAL COLLISIONS: " << this->collision << endl;
	cout << "Final form of hash table is saved as 150140138_output.txt" << endl;
	this->printOutput();
}

//main function
int main(int argc, char *argv[]) {
	myHash *hush = new myHash(atoi(argv[2]));
	hush->readFile(argv[1]);
}