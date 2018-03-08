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

class Number {
public:
	long int id;
	long int *number;
	Number() {};
};

class CountingSort {
public:
	int max = 0;
	int *C;
	Number *B;
	CountingSort() {};
	Number * Sort (Number* arrA, int *A, long int N, long int pos) {
		findMax(A, N);
		//counting sort algorithm
		for (int i = 0; i <= max; i++) {
			C[i] = 0;
		}
		for (int i = 0; i < N; i++) {
			C[A[i]]++;
		}
		for (int i = 1; i <= max; i++) {
			C[i] = C[i] + C[i - 1];
		}
		for (int i = (N-1); i >= 0; i--) {
			B[C[A[i]]-1] = arrA[i];
			C[A[i]]--;
		}
		//After finishing sorting apply the changes on new array on main number array
		return B;
	}
	void findMax(int *A, long int N) {
		//initialize C
		for (long int i = 0; i < N; i++) {
			if (A[i] > max) {
				max = A[i];
			}
		}
		B = new Number[N];
		C = new int[max+1];
	}
};

class Radix {
public:
	Number *n_arr;
	long int N;
	int *A, *B, *C, max_number = 0;
	Radix() {};
	Radix(long int N) {
		this->N = N;
	};
	void ReadFromFile() {
		//initialize the arrays
		n_arr = new Number[N];
		//reading the file
		ifstream file;
		file.open("numbers_to_sort.txt");
		long long int a, b;
		long long int id_counter = 0;
		while (file >> a >> b)
		{
			if (id_counter == N) {
				break;
			}
			n_arr[id_counter].id = a;
			n_arr[id_counter].number = new long int[10];

			int position = 9;
			for (int i = 0; i < 10; i++) {
				n_arr[id_counter].number[i] = (long long int)(b / pow(10, position)) % 10;
				position--;
			}
			id_counter++;
		}
		file.close();
	};
	void Sort() {
		int *A = new int[N];
		for (int j = 9; j >= 0; j--) {
			for (int i = 0; i < N; i++) {
				A[i] = n_arr[i].number[j];
			}
			CountingSort srt;
			Number * B = srt.Sort(n_arr, A, N, j);
			//apply changes on main data array
			for (int i = 0; i < N; i++) {
				n_arr[i] = B[i];
			}
			free(B);
		}
	};
	void writeToFile() {
		ofstream f;
		f.open("sorted_numbers.txt");
		for (int i = 0; i < N; i++) {
			f << n_arr[i].id << " ";
			for (int j = 0; j < 10 ; j++) {
				f << n_arr[i].number[j];
			}
			f << "\n";
		}
	}
};

class ts {
public:
	int id;
	int no;
	ts() {

	}
	ts(int id, int no) {
		this->id = id;
		this->no = no;
	}
};

int main(int argc, char * argv[]) {
	clock_t start = clock();
	Radix r(atoi(argv[1]));
	r.ReadFromFile();
	r.Sort();
	r.writeToFile();
	printf("Total execution time: %f\n", (double)(clock() - start) / CLOCKS_PER_SEC);

	//-------Freezing Sort Algorithm----------
	//ts *t = new ts[7];
	//t[0] = ts(1, 7);
	//t[1] =  ts(2, 2);
	//t[2] =  ts(3, 4);
	//t[3] =  ts(4, 3);
	//t[4] =  ts(5, 4);
	//t[5] =  ts(6, 1);
	//t[6] =  ts(7, 6);
	//int n = 7;
	//for (int frz = 0; frz < 7; frz++) {
	//	for (int i = frz; i < (7 + frz) / 2 + 1; i++) {
	//		if (i != n - i + frz && t[i].no < t[n - i + frz].no) {
	//			ts temp = t[n - i + frz];
	//			t[n - i + frz] = t[i];
	//			t[i] = temp;
	//		}
	//	}
	//	if (frz != 0) {
	//		for (int i = n - frz; i > (n - frz) / 2; i--) {
	//			if (i != n - i - frz && t[n - i - frz].no < t[i].no) {
	//				ts temp = t[n - i + frz];
	//				t[n - i + frz] = t[i];
	//				t[i] = temp;
	//			}
	//		}
	//	}
	//	cout << "After each phase:\n";
	//	for (int i = 0; i < 7; i++) {
	//		cout << t[i].id << " " << t[i].no << "\n";
	//	}
	//	cout << "--------------\n";
	//}
	//for (int i = 0; i < 7; i++) {
	//	cout << t[i].id << " " << t[i].no << "\n";
	//}
}
