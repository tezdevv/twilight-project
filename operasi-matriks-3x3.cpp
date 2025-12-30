#include <iostream>
#include <cmath>
using namespace std;

int main() {
    int M = 3;
    char mode = 'x';
    
    int A[M][M] = {{1, 2, 3}, {1, 2, 3}, {1, 2, 3}};
    int B[M][M] = {{1, 2, 3}, {1, 2, 3}, {1, 2, 3}};
    int C[M][M] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    
    for (int i = 0; i < M; i++) {
		for (int j = 0; j < M; j++) {
		    cout << A[i][j] << " " ;
	    }
	    cout << endl;
    }
    
	cout << endl;
    cout << mode << endl;
	cout << endl;
    
    for (int i = 0; i < M; i++) {
		for (int j = 0; j < M; j++) {
		    cout << B[i][j] << " " ;
	    }
	    cout << endl;
    }
    
	cout << endl;
    
    if (mode == '+') {
        for (int i = 0; i < M; i++) {
		    for (int j = 0; j < M; j++) {
			    C[i][j] = A[i][j] + B[i][j];
		    }
        }
	} else if (mode == '-') {
        for (int i = 0; i < M; i++) {
		    for (int j = 0; j < M; j++) {
			    C[i][j] = A[i][j] - B[i][j];
		    }
        }
    } else if (mode == 'x') {
        for (int i = 0; i < M; i++) {
		    for (int j = 0; j < M; j++) {
		        for (int k = 0; k < M; k++) {
			        C[i][j] += A[i][k] * B[k][j];
		        }
		    }
        }
    } else {
        cout << "operasi tidak ada" << endl;
    }
    
    for (int i = 0; i < M; i++) {
		for (int j = 0; j < M; j++) {
		    cout << C[i][j] << " " ;
	    }
	    cout << endl;
    }
    
}
