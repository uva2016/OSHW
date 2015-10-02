#include <pthread.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <iostream>
using namespace std;



vector< vector<int> > grid(9, vector<int>(9));
void *box(void *param);
void *rowCheck(void *param);
void *colCheck(void *param);

struct parameters {
	int row;
	int col;
};

int main(int argc, char *argv[]) {
	string s;
	ifstream myfile ("sudoku.txt");
	getline (myfile,s);
	cout << s << endl;
	int i = 0; 
	for(int b = 0; b < 9; b++) {
		for(int j = 0; j < 9; j++) {
			grid[b][j] = (s[i] - '0');
			i+=2;
		}
	}	

	pthread_t workers[11];

	pthread_t tid1;
	pthread_attr_t attr1;
	pthread_attr_init(&attr1);
	struct parameters data1;
	data1.row = 0;
	data1.col = 0;
	pthread_create(&tid1, &attr1, box, &data1);
	workers[0] = tid1;

	pthread_t tid2;
	pthread_attr_t attr2;
	pthread_attr_init(&attr2);
	struct parameters data2;
	data2.row = 0;
	data2.col = 3;
	pthread_create(&tid2, &attr2, box, &data2);
	workers[1] = tid2;

	pthread_t tid3;
	pthread_attr_t attr3;
	pthread_attr_init(&attr3);
	struct parameters data3;
	data3.row = 0;
	data3.col = 6;
	pthread_create(&tid3, &attr3, box, &data3);
	workers[2] = tid3;

	pthread_t tid4;
	pthread_attr_t attr4;
	pthread_attr_init(&attr4);
	struct parameters data4;
	data4.row = 3;
	data4.col = 0;
	pthread_create(&tid4, &attr4, box, &data4);
	workers[3] = tid4;

	pthread_t tid5;
	pthread_attr_t attr5;
	pthread_attr_init(&attr5);
	struct parameters data5;
	data5.row = 3;
	data5.col = 3;
	pthread_create(&tid5, &attr5, box, &data5);
	workers[4] = tid5;

	pthread_t tid6;
	pthread_attr_t attr6;
	pthread_attr_init(&attr6);
	struct parameters data6;
	data6.row = 3;
	data6.col = 6;
	pthread_create(&tid6, &attr6, box, &data6);
	workers[5] = tid6;

	pthread_t tid7;
	pthread_attr_t attr7;
	pthread_attr_init(&attr7);
	struct parameters data7;
	data7.row = 6;
	data7.col = 0;
	pthread_create(&tid7, &attr7, box, &data7);
	workers[6] = tid7;

	pthread_t tid8;
	pthread_attr_t attr8;
	pthread_attr_init(&attr8);
	struct parameters data8;
	data8.row = 6;
	data8.col = 3;
	pthread_create(&tid8, &attr8, box, &data8);
	workers[7] = tid8;

	pthread_t tid9;
	pthread_attr_t attr9;
	pthread_attr_init(&attr9);
	struct parameters data9;
	data9.row = 6;
	data9.col = 6;
	pthread_create(&tid9, &attr9, box, &data9);
	workers[8] = tid9;

	pthread_t tid10;
	pthread_attr_t attr10;
	pthread_attr_init(&attr10);
	pthread_create(&tid10, &attr10, rowCheck, NULL);
	workers[9] = tid10;

	pthread_t tid11;
	pthread_attr_t attr11;
	pthread_attr_init(&attr11);
	pthread_create(&tid11, &attr11, colCheck, NULL);
	workers[10] = tid11;

	for(int i = 0; i < 11; i++) {
		pthread_join(workers[i], NULL);
	}
}	

void *box(void *param) {
	struct parameters* p = (struct parameters*) param;
	int r = p->row;
	int c = p->col;
	set<int> nums;
	for(int i = r; i < r+3; i++) {
		for(int k = c; k < c+3; k++) {
			if(nums.find(grid[i][k]) != nums.end()) {
				cout << "it failed because of the square " << r << "," << c << endl;
			} else {
				nums.insert(grid[i][k]);
			}
		}	
	}
	pthread_exit(0);
}

void *rowCheck(void *param) {
	for(int i = 0; i < 9; i++) {
		set<int> nums;
		for(int k = 0; k < 9; k++) {
			if(nums.find(grid[i][k]) != nums.end()) {
				cout << "it failed because of the row " << (i + 1) << " digit " << grid[i][k] << " is already in the line" << endl;
			} else {
				nums.insert(grid[i][k]);
			}
		}
	}
	pthread_exit(0);
}

void *colCheck(void *param) {
	for(int i = 0; i < 9; i++) {
		set<int> nums;
		for(int k = 0; k < 9; k++) {
			if(nums.find(grid[k][i]) != nums.end()) {
				cout << "it failed because of the column " << i+1 << " value is already in the column "<< grid[k][i] << endl;
			} else {
				nums.insert(grid[k][i]);
			}
		}
	}
	
	pthread_exit(0);
}
