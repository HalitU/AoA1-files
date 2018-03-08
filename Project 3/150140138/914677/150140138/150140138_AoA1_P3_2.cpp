/*
Student Name: Halit Uyanık
Student Number: 150140138
*/

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include <math.h>

using namespace std;
//Player class params: ID, CP
class Player {
public:
	long int ID;
	long int CP;	
};
//Game class where all of the operations done in.
class Game {
public:
	long int N;
	Player *clanA;
	Player *clanB;
	long int heapSize;
	long int size;
	Game(long int N) {
		this->N = N;
		this->heapSize = N;
		this->size = N;
	};
	//this is used for part 2
	void run() {
		/*
		Read clan A and clan B's data. And sort them.
		*/
		readClanA();
		readClanB();
		HeapSort(clanA);
		HeapSort(clanB);
		fix();
		this->heapSize--;
		readGame();
		//calculate the total CP's after game is finished.
		long long int totalACP = 0, totalBCP = 0;
		for (int i = 0; i < N; i++) {
			totalACP += clanA[i].CP;
			totalBCP += clanB[i].CP;
		}
		cout << "Results: A:" << totalACP << " B:" << totalBCP << "\n";
		if (totalACP > totalBCP)
			cout << "A wins.\n";
		else
			cout << "B wins.\n";
		writeA();
	}
	//this is used for part 1
	void run2() {
		readClanA();
		//we will get the timer for part A here
		clock_t start = clock();
		HeapSort(clanA);
		printf("Total execution time: %f\n", (double)(clock() - start) / CLOCKS_PER_SEC);
		//timer ended
		writeA2();
	}
	//writes A to A_results.txt
	void writeA() {
		ofstream f;
		f.open("A_results.txt");
		for (int i = 0; i < N; i++) {
			f << clanA[i].ID << " " << clanA[i].CP << "\n";
		}
	}
	//writes A to A_sorted.txt
	void writeA2() {
		ofstream f;
		f.open("A_sorted.txt");
		for (int i = 0; i < N; i++) {
			f << clanA[i].ID << " " << clanA[i].CP << "\n";
		}
	}
	//Reads the game file line by line and processes on Clan A and B
	void readGame() {
		ifstream s;
		s.open("gamelogs.txt");
		long int GameID, AttackerPos, AttackedPos;
		char clanOfAttacker;
		while (s >> GameID >> clanOfAttacker >> AttackerPos >> AttackedPos) {
			if (clanOfAttacker == 'A') {
				AAttack(AttackerPos, AttackedPos);
			}
			else {
				BAttack(AttackerPos, AttackedPos);
			}
			//sort after each attack
			HeapSort(clanA);
			HeapSort(clanB);
			fix();
		}
	}
	//A's attack function.
	void AAttack(long int AttackerPos, long int AttackedPos) {
		//leader
		if (AttackerPos == 0) {
			clanA[AttackerPos].CP += clanB[AttackedPos].CP / 2;
			clanB[AttackedPos].CP = clanB[AttackedPos].CP - clanB[AttackedPos].CP / 2;
		}
		//henchman
		else if (AttackerPos > 0 && AttackerPos < 8) {
			clanA[AttackerPos].CP += 500;
			clanB[AttackedPos].CP -= 500;

		}
		//simple solidier
		else {
			clanA[AttackerPos].CP += (abs((int)(log2(AttackerPos + 1)) - (int)(log2(AttackedPos + 1))) + 1) * 30;
			clanB[AttackedPos].CP -= 120;
		}
		//if the attacked side goes down below 0 it will get fixed to 0.
		if (clanB[AttackedPos].CP < 0) {
			clanB[AttackedPos].CP = 0;
		}
	}
	//B's attack function similar to A
	void BAttack(long int AttackerPos, long int AttackedPos) {
		if (AttackerPos == 0) {
			clanB[AttackerPos].CP += clanA[AttackedPos].CP / 2;
			clanA[AttackedPos].CP = clanA[AttackedPos].CP - clanA[AttackedPos].CP / 2;
		}
		else if (AttackerPos > 0 && AttackerPos < 8) {
			clanA[AttackedPos].CP -= 500;
			clanB[AttackerPos].CP += 500;
		}
		else {
			clanB[AttackerPos].CP += (abs((int)(log2(AttackerPos + 1)) - (int)(log2(AttackedPos + 1))) + 1) * 30;
			clanA[AttackedPos].CP -= 120;
		}
		if (clanA[AttackedPos].CP < 0) {
			clanA[AttackedPos].CP = 0;
		}
	}
	//takes the inverse of the array to use positions easier
	void fix() {
		for (int i = 0; i < this->size/2; i++) {
			Player pss = clanA[i];
			clanA[i] = clanA[N - 1 - i];
			clanA[N - 1 - i] = pss;
		}
		for (int i = 0; i < this->size / 2; i++) {
			Player pss = clanB[i];
			clanB[i] = clanB[N - 1 - i];
			clanB[N - 1 - i] = pss;
		}
	}
	//read clan A
	void readClanA() {
		clanA = new Player[N];
		ifstream s;
		s.open("ClanA.txt");
		long int ID, CP, counter = 0;
		while (s >> ID >> CP) {
			if (counter == N)
				break;
			clanA[counter].ID = ID;
			clanA[counter].CP = CP;
			counter++;
		}
		s.close();
	}
	//read clan B
	void readClanB() {
		clanB = new Player[N];
		ifstream s;
		s.open("ClanB.txt");
		long int ID, CP, counter = 0;
		while (s >> ID >> CP) {
			if (counter == N)
				break;
			clanB[counter].ID = ID;
			clanB[counter].CP = CP;
			counter++;
		}
		s.close();
	}
	//heapsort function
	void HeapSort(Player * A) {
		Build_Max_Heap(A);
		for (int i = N-1; i > 0; i--) {
			Player pss = A[0];
			A[0] = A[i];
			A[i] = pss;
			this->N--;
			Max_Heapify(A, 0);
		}
		this->N = this->size;
	}
	//build max heap
	void Build_Max_Heap(Player * A) {
		for (long int i = (N-1) / 2; i >= 0; i--) {
			Max_Heapify(A, i);
		}
	}
	//max heapify function
	void Max_Heapify(Player* A, long int i) {
		long int l = Left(i);
		long int r = Right(i);
		long int largest = i;
		if (l != -1 && l < N && A[l].CP > A[i].CP) {
			largest = l;
		}
		else {
			largest = i;
		}
		if (r != -1 && r < N && A[r].CP > A[largest].CP) {
			largest = r;
		}
		if (largest != i) {
			Player passer = A[largest];
			A[largest] = A[i];
			A[i] = passer;
			Max_Heapify(A, largest);
		}
	}
	long int Left(long int i) {
		if (i * 2 + 1 < N)
			return i * 2 + 1;
		return -1;
	}
	long int Right(long int i) {
		if (i * 2 + 2 < N)
			return (i * 2 + 2);
		return -1;
	}
};
//Main function
int main(int argc, char * argv[]) {
	if (atoi(argv[1]) == 1) {
		Game g(atoi(argv[2]));
		g.run2();
	}
	else if(atoi(argv[1]) == 2) {
		Game g(10000);
		g.run();
	}
}