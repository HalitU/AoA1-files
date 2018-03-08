#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cstring>

using namespace std;
/*
This class is used to store id, x, y, and distance to the given point values of a warehouse
*/
class Nokta {
public:
	long int id = 0, x, y;
	float distance = -1;
	Nokta() {}
	Nokta(long int id, long int x, long int y) {
		this->id = id;
		this->x = x;
		this->y = y;
	}
};

void searchMain(long int, long int, char *, float, float);

class Search{
public:
	//fields
	Nokta * arr;
	long int N, K;
	char * type;
	double x1, y1;
	Search() {};
	Search(long int N, long int K, char * type, double x1, double y1) {
		this->N = N;
		this->K = K;
		this->type = type;
		this->x1 = x1;
		this->y1 = y1;
	};
	/*
	Chooses the appopriate function call
	*/
	void run() {
		readFromFile();
		//select the operation type
		if (strcmp(type, "IS") == 0) {
			printf("called IS\n");
			insertion_sort(N);
			//printResult();
		}
		else if (strcmp(type, "MS") == 0) {
			printf("called MS\n");
			merge_sort(0, N);
			//printResult();
		}
		else if (strcmp(type, "LS") == 0) {
			printf("called LS\n");
			linear_search();
		}
		printToFile();
	}
	/*
	Reads the warehouse data from file
	*/
	void readFromFile() {
		//initialize array
		this->arr = new Nokta[this->N+1];
		//reading the file
		ifstream file;
		file.open("warehouselocations.txt");
		long int a, b, c;
		long int id_counter = 0;
		while (file >> a >> b >> c)
		{
			if (id_counter > N) {
				break;
			}
			//appoint array values
			arr[id_counter].id = a;
			arr[id_counter].x = b;
			arr[id_counter].y = c;
			arr[id_counter].distance = distance(x1, y1, b, c);
			id_counter++;
		}
		file.close();
	}
	/*
	Linear Search Function
	Creates an initial K array
	And checks the main array linearly
	*/
	Nokta* linear_search() {
		Nokta *lin_arr = new Nokta[K];
		long int arr_ptr = 0;
		for (long int i = 0; i < N; i++) {
			for (long int j = 0; j < K; j++) {
				if (lin_arr[j].distance == -1) {
					lin_arr[arr_ptr] = arr[i];
					arr_ptr++;
					break;
				}
				else if (lin_arr[j].distance > arr[i].distance) {
					lin_arr[j] = arr[i];
					break;
				}
			}
			if (arr_ptr == 10) {
				arr_ptr = 0;
			}
		}
		//printResult();
		return NULL;
	}
	/*
	Insertion Sort Function
	*/
	void insertion_sort(long int n) {
		for (long int i = 1; i < n; i++) {
			Nokta pss = arr[i];
			long int j = i - 1;
			while (j > -1 && arr[j].distance > pss.distance) {
				arr[j + 1] = arr[j];
				j--;
			}
			arr[j + 1] = pss;
		}
	}
	/*
	Main merge-sort function
	*/
	void merge_sort(long int p, long int r) {
		int q = 0;
		if (p < r) {
			q = (p + r) / 2;
			merge_sort(p, q);
			merge_sort(q + 1, r);
			merge(p, q, r);
		}
	}
	/*
	Merge function 
	*/
	void merge(long int p, long int q, long int r) {
		long int n1 = (q - p) + 1;
		long int n2 = (r - q);
		Nokta *L = new Nokta[n1];
		Nokta *R = new Nokta[n2];
		for (long int i = 0; i < n1; i++) {
			L[i] = arr[(p + i)];
		}
		for (long int j = 0; j < n2; j++) {
			R[j] = arr[q + j + 1];
		}
		long int i = 0, j = 0, k = p;
		while (i < n1 && j < n2) {
			if (L[i].distance <= R[j].distance) {
				arr[k] = L[i];
				i++;
			}
			else {
				arr[k] = R[j];
				j++;
			}
			k++;
		}
		if (i == n1) {
			for (long int l = j; l < n2; l++) {
				arr[k] = R[j];
				j++;
				k++;
			}
		}
		else if (j == n2) {
			for (long int l = i; l < n1; l++) {
				arr[k] = L[i];
				i++;
				k++;
			}
		}
	}
	/*
	Euclidean Distance function
	*/
	float distance(long int x1, long int y1, long int x2, long int y2) {
		float result = 0.0;
		result = fabs(sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)));
		return result;
	}
	/*
	Prints the results to output.txt file
	*/
	void printToFile() {
		ofstream outFile;
		outFile.open("output.txt");
		for (long int i = 0; i < K; i++) {
			outFile << arr[i].id << "\t" << arr[i].x << "\t" << arr[i].y << "\t" << (float)arr[i].distance << "\n";
		}
		outFile.close();
	}
	/*
	This function is used for debugging
	*/
	void printResult() {
		printf("print called\n");
		for (long int i = 0; i < K; i++) {
			printf("id: %ld, x: %ld, y: %ld, distance: %f\n", arr[i].id, arr[i].x, arr[i].y, arr[i].distance);
		}
	}
};

/*
Main Function
*/
int main(int argc, char* argv[]) {
	searchMain(atof(argv[1]), atof(argv[2]), argv[3], atof(argv[4]), atof(argv[5]));
}
/*
calls the clock for execution time checking 
creates a search class with initial values from argv which came from main
*/
void searchMain(long int NODE_COUNT, long int result_length, char * input, float x1, float y1) {
	//start clock
	clock_t t;
	int f;
	t = clock();
	Search *s = new Search(NODE_COUNT, result_length, input, x1, y1);
	s->run();
	//print time
	t = clock() - t;
	printf("It took me %d clicks (%f seconds).\n\n", t, ((float)t) / CLOCKS_PER_SEC);
}

