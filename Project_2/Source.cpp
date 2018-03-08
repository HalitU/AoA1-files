/*
Halit Uyanık
150140138
*/

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <sstream>
#include <iterator>

using namespace std;
/*
This class is used for sorting numbers from numbes_to_sort.txt
It uses the QuickSort Algorithm
*/
class KSized {
public:
	vector<int> numbers;
	int K;
	KSized() {};
	KSized(int K) { 
		this->K = K;
	}
	void addToNumbers(int num) {
		this->numbers.push_back(num);
	}
	void readFromFile() {
		//reading the file
		ifstream file;
		file.open("numbers_to_sort.txt");
		long int a;
		long int id_counter = 0;
		while (file >> a)
		{
			if (id_counter == K)
				break;
			numbers.push_back(a);
			id_counter++;
		}
		file.close();
	}
	void printToFile() {
		ofstream outFile;
		outFile.open("numbers.txt");
		for (int i = 0; i < numbers.size(); i++) {
			outFile << numbers[i] << "\n";
		}
		outFile.close();
	}
	int randPartition(int p, int r) {
		int x = numbers[r];
		int i = p - 1;
		for (int j = p; j < r; j++) {
			if (numbers[j] <= x) {
				i++;
				int pss = numbers[i];
				numbers[i] = numbers[j];
				numbers[j] = pss;
			}
		}
		int pss2 = numbers[i + 1];
		numbers[i + 1] = numbers[r];
		numbers[r] = pss2;
		//return index
		return i + 1;
	}
	int randomizedPartition(int p, int r) {
		//choose a random number
		int i = p + rand() % (r - p);
		//exchange id
		int pss = numbers[r];
		numbers[r] = numbers[i];
		numbers[i] = pss;
		return randPartition(p, r);
	}
	void randQuickSort(int p, int r) {
		if (p < r) {
			int q = randomizedPartition(p, r);
			randQuickSort(p, q - 1);
			randQuickSort(q + 1, r);
		}
	}
};
/*
This class is used to hold each lines word and word IDs
It uses random quick sort algorithm
*/
class Line {
public:
	int lineID;
	vector<string> words;
	vector<int> wordIDs;
	Line() {};
	Line(int lineID, vector<string> words, vector<int> wordIDs) {
		this->lineID = lineID;
		this->words = words;
		this->wordIDs = wordIDs;
	}
	//print the line
	void printLine() {
		cout << "Line: " << lineID << " :: ";
		for (int i = 0; i < words.size(); i++) {
			cout << words[i] << " ";
		}
		cout << "\n";
	}
	//RANDOMIZED PART
	int randPartition(int p, int r) {
		int x = wordIDs[r];
		int i = p - 1;
		for (int j = p; j < r; j++) {
			if (wordIDs[j] <= x) {
				i++;
				int pss = wordIDs[i];
				wordIDs[i] = wordIDs[j];
				wordIDs[j] = pss;
				//exchange word string
				string str = words[i];
				words[i] = words[j];
				words[j] = str;
			}
		}	
		int pss2 = wordIDs[i + 1];
		wordIDs[i + 1] = wordIDs[r];
		wordIDs[r] = pss2;
		//exchange word string
		string str = words[i+1];
		words[i+1] = words[r];
		words[r] = str;
		//return index
		return i + 1;
	}
	int randomizedPartition(int p, int r) {
		//choose a random number
		int i = p + rand() % (r - p);
		//exchange id
		int pss = wordIDs[r];
		wordIDs[r] = wordIDs[i];
		wordIDs[i] = pss;
		//exchange word string
		string str = words[r];
		words[r] = words[i];
		words[i] = str;
		return randPartition(p, r);
	}
	void randQuickSort(int p, int r) {
		if (p < r) {
			int q = randomizedPartition(p, r);
			randQuickSort(p, q - 1);
			randQuickSort(q + 1, r);
		}
	}
};
/*
This class is used for main Quick Sort algorithm and reading the file.
*/
class QuickSrt {
public:
	vector<Line> lineV;
	int M, N;
	QuickSrt() {};
	QuickSrt(int M, int N) {
		this->M = M;
		this->N = N;
	};
	void readFromFile() {
		//reading the file
		ifstream file;
		file.open("mobydick.txt");
		long int a;
		string b, c;
		long int id_counter = 0;
		while (file >> a >> b >> c)
		{
			vector<string> test;
			test = splitLine(b, '_');
			vector<int> test2;
			test2 = splitIDs(c, '_');
			Line newLine(a, test, test2);
			lineV.push_back(newLine);
			id_counter++;
		}
		cout << "line count: " << id_counter << " " << lineV.size() << " " << lineV[id_counter-1].lineID << "\n";
		file.close();
		QuickSort(0, id_counter-1);
		cout << "Ordered the lines\n";
		//order words in each line...
		int ctr = 1;
		for (vector<Line>::iterator it = lineV.begin(); it != lineV.end(); ++it) {
			srand(255);
			it->randQuickSort(0, it->wordIDs.size() - 1);
			if (ctr == 551)
				it->printLine();
			ctr++;
		}
		cout << "Ordered the words\n";
		printToFile();
		cout << "Finished writing to file\n";
	}
	void printToFile() {
		ofstream outFile;
		outFile.open("novel.txt");
		for (int i = 0; i < lineV.size(); i++) {
			for (int j = 0; j < lineV[i].words.size(); j++) {
				outFile << lineV[i].words[j] << " ";
			}
			outFile << "\n";
		}
		outFile.close();
	}
	vector<Line> getSortedLine() {
		return this->lineV;
	}
	int getQuickSortCtr() {
		return this->quicksortctr;
	}
	vector<string> splitLine(string line, char seperator) {
		vector<string> vec;
		stringstream ss;
		ss.str(line);
		string crr;
		while (getline(ss, crr, seperator)) {
			string ses;
			if (crr.substr(0, 1).compare("{") == 0) {
				if (crr.substr(crr.length() - 1, crr.length()).compare("}") == 0) {
					ses = crr.substr(1, crr.length() - 2);
				}
				else {
					ses = crr.substr(1, crr.length() - 1);
				}
			}
			else if (crr.substr(crr.length() - 1, crr.length()).compare("}") == 0) {
				if (crr.substr(0, 1).compare("{") == 0) {
					ses = crr.substr(1, crr.length() - 2).c_str();
				}
				else {
					ses = crr.substr(0, crr.length() - 1);
				}
			}
			else {
				ses = crr;
			}
			vec.push_back(ses);
		};
		return vec;
	}
	vector<int> splitIDs(string line, char seperator) {
		vector<int> vec;
		stringstream ss;
		ss.str(line);
		string crr;
		int crrInt;
		while (getline(ss, crr, seperator)) {
			int ses;
			if (crr.substr(0, 1).compare("{") == 0) {
				if (crr.substr(crr.length() - 1, crr.length()).compare("}") == 0) {
					ses = atoi(crr.substr(1, crr.length() - 2).c_str());
				}
				else {
					ses = atoi(crr.substr(1, crr.length() - 1).c_str());
				}
			}
			else if (crr.substr(crr.length() - 1, crr.length()).compare("}") == 0) {
				if (crr.substr(0, 1).compare("{") == 0) {
					ses = atoi(crr.substr(1, crr.length() - 2).c_str());
				}
				else {
					ses = atoi(crr.substr(0, crr.length() - 1).c_str());
				}
			}
			else {
				ses = atoi(crr.c_str());
			}
			vec.push_back(ses);
		};
		return vec;
	}
	int quicksortctr = 0;
	void QuickSort(int p, int r) {
		quicksortctr++;
		if (quicksortctr == this->M) {
			cout << lineV[this->N].lineID << "\n";
		}
		if (p < r) {
			int q = Partition(p, r);
			QuickSort(p, q-1);
			QuickSort(q+1, r);
		}
	}
	int Partition(int p, int r) {
		Line x = lineV[r];
		int i = p - 1;
		for (int j = p; j < r; j++) {
			if (lineV[j].lineID <= x.lineID) {
				i++;
				Line pss = lineV[i];
				lineV[i] = lineV[j];
				lineV[j] = pss;
			}
		}
		Line pss2 = lineV[i+1];
		lineV[i + 1] = lineV[r];
		lineV[r] = pss2;
		return i + 1;
	};
};
/*
Main function
*/
int main(int argc, char* argv[]) {
	clock_t start = clock();
	if (atoi(argv[1]) == 1) {
		QuickSrt srt(atoi(argv[2]), atoi(argv[3]));
		srt.readFromFile();
		cout << "Quicksort call counter: " << srt.quicksortctr << "\n";
	}
	else{
		KSized ks(atoi(argv[2]));
		ks.readFromFile();
		ks.randQuickSort(0, ks.K - 1);
		ks.printToFile();
	}
	printf("Total execution time: %f\n", (double)(clock()-start)/CLOCKS_PER_SEC);
}

