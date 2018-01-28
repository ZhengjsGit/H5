#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<stdio.h>
#include<mpi.h>


double ** AllocM2(double * in1d,int M, int N);
void FreeM2(double **A, int N);
double SumArray(double * A, int len);
int main(int argc,char* argv[]){


	int Mcol, Nrow;
	Mcol=1000;
	Nrow=1000;
	int my_rank, rank_size;
	/*
	{
		int i = 1;
		char hostname[256];
		gethostname(hostname, sizeof(hostname));
		printf("PID %d on %s ready for attach\n", getpid(), hostname);
		fflush(stdout);
		while (0 == i)
			sleep(5);
	}
*/	
	MPI_Status Status;	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &rank_size);
	//all threads have allocated buffer, so buffer is private for each threads??? 
	//buffer  should be a void pointer, and it is a one dimensional block structure. 
	double start, end;
	MPI_Barrier(MPI_COMM_WORLD);
	start = MPI_Wtime();
	double ** A;
	double ** B;
	double *A_con;
	double *B_con;
	B_con = (double *)malloc(sizeof(double)*Mcol*Nrow);
	A_con = (double *)malloc(sizeof(double)*Mcol*Nrow);
	if( 0 == my_rank ){
		//A_con is private of thread 0
		A = AllocM2(A_con,Mcol, Nrow);
		B = AllocM2(B_con,Mcol, Nrow);
		//B matrix should bcast to other threads. That's true way.

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
		/*
			   for(int i=0; i<Mcol; i++){
			   MPI_Send( *(A+i), Nrow, MPI_DOUBLE, i%rank_size, i, MPI_COMM_WORLD);
			   }
			   */


		printf("init pass\n");
	}
	MPI_Bcast( B_con,Mcol*Nrow, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast( A_con,Mcol*Nrow, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	/*
	   double * bufferA;
	   bufferA = (double *)malloc(sizeof(double)*Mcol/rank_size*Nrow);
	   */
	double localRes[Mcol/rank_size][Nrow];
	for(int i=0;i<Mcol/rank_size;++i)
		for(int j=0;j<Nrow;++j)
			localRes[i][j] = 0;
	/*
	   int tag = my_rank;
	   for (int i=0;i< Mcol/rank_size; i++){
	   MPI_Recv( &bufferA[i*Nrow], Nrow, MPI_DOUBLE, 0, tag ,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	   tag = tag + rank_size;

	   }
	   */
	int tag = my_rank;
	for(int i=0;i<Mcol/rank_size;i++){
		for(int j=0;j<Nrow;j++){
			for(int k=0;k<Nrow;k++){
				localRes[i][j] = localRes[i][j] + *(A_con+tag*Nrow+k) * *(B_con+tag*Nrow+k);
				//localRes[i][j] =  *(bufferA+i*Nrow+k) * *(B_con+tag*Nrow+k);
			}
		}
		tag = tag + rank_size;
	}

	printf("%d:::::\n",my_rank);
	for(int i=0;i<Mcol/rank_size;i++){
		for(int j=0;j<Nrow;j++){
			//printf("i_row%d,j_col%d=%.2f\n",i,j,localRes[i][j]);
		}
	}

	printf("\n");
	end = MPI_Wtime();
	MPI_Barrier(MPI_COMM_WORLD);
	if(0 == my_rank){
		printf("time = %f.",end - start);
	}
	MPI_Finalize();
	//	while(1);
	return 0;
}
/*
 * older method for alloc 2-d matrix memory
 * memory is not continueous
 double ** AllocM2(int M, int N){
 double **A;
 A = (double **)malloc(sizeof(double *) * M );
 for(int i=0; i < M; i++){

 *(A+i) = (double *)malloc(sizeof(double)*N);

 }
 return A;

 }
 void FreeM2(double **A, int N){
 for(int i=0;i<N;i++){
 free( A[i] );
 }
 free(A);
 }
 */
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

