#include<stdio.h>
#include<time.h>
#include<stdlib.h>

#define Mcol 1000
#define Nrow 1000


double ** AllocM2(double *in1d, int M, int N);
int main(void){
	
	clock_t start, finish;
	start = clock();
	double ** A;
	double ** B;
	double *A_con;
	double *B_con;
	B_con = (double *)malloc(sizeof(double)*Mcol*Nrow);
	A_con = (double *)malloc(sizeof(double)*Mcol*Nrow);
	A = AllocM2(A_con,Mcol, Nrow);
	B = AllocM2(B_con,Mcol, Nrow);

	for(int i=0;i<Mcol;i++){
		srand((unsigned) time(NULL));
		for(int j =0;j<Nrow;j++){
			*(*(A+i)+j) = rand() % 101;
		}
	}
	for(int i=0;i<Mcol;i++){
		srand((unsigned) time(NULL));
		for(int j=0;j<Nrow;j++){
			*(*(B+i)+j) = rand() % 101;
		}
	}

	double C[Mcol][Nrow];
	for(int i=0;i<Mcol;i++)
		for(int j=0;j<Nrow;j++)
			C[i][j] =0;
	for(int i=0;i<Mcol;i++){
		for(int j=0;j<Nrow;j++){
			for(int k=0;k<Mcol;k++){
				C[i][j] = C[i][j] + *(*(A+i)+k) * *(*(B+k)+j);	
			}
	//		printf("i_row%d,j_col%d=%.2f\n",i,j,C[i][j]);
		}
	}
	finish = clock();
	double duration;
	duration = (finish - start) *1.0 / CLOCKS_PER_SEC;
	printf("%f seconds\n", duration);
	return 0;	
}


double ** AllocM2(double *in1d, int M, int N){
	double **A;
	A = (double **)malloc(sizeof(double *) * M );
	for(int i=0; i < M; i++){

		*(A+i) = in1d+i*N;

	}
	return A;

}
void FreeM2(double **A, int N){
	for(int i=0;i<N;i++){
		free( A[i] );
	}
	free(A);
}

