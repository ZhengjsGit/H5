#include<stdlib.h>
#include<stdio.h>
#include<mpi.h>

#define Mcol 10
#define Nrow 10

void AllocM2(double **A, int M, int N);
int main(int argc,char* argv[]){

	
    printf("fuck");
	int my_rank, rank_size;
	MPI_Status Status;	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &rank_size);
	double ** buffer;
	//AllocM2(buffer, Mcol/rank_size, Nrow);
//all threads have allocated buffer, so buffer is private for each threads??? 
//buffer  should be a void pointer, and it is a one dimensional block structure. 
	if( 0 == my_rank ){
		
		double ** A;
		double ** B;
		double ** C;

		AllocM2(A, Mcol, Nrow);
		AllocM2(B, Mcol, Nrow);
		AllocM2(C, Mcol, Nrow);


		for(int i=0;i<Mcol;i++)
			for(int j =0;j<Nrow;j++){
				*(*(A+i)+j) = 1;
			}
		for(int i=0;i<Mcol;i++)
			for(int j=0;j<Nrow;j++){
				*(*(B+i)+j) = 1;
			}
		for(int i=0; i<Mcol; i++){
			//MPI_Send( *(A + i), Nrow, MPI_DOUBLE, i%rank_size, i, MPI_COMM_WORLD);
		}
	//	for(int i=0; i<Mcol; i++){
	//		MPI_Send( *(B + i), Nrow, MPI_DOUBLE, i%rank_size, Mcol+i, MPI_COMM_WORLD);
	//	}	
	}
    
	for (int i=0;i< Mcol/rank_size; i++){
		//MPI_Recv( *(buffer + i), Nrow, MPI_DOUBLE, 0, tag ,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		//tag = tag + rank_size;
	}

	MPI_Finalize();
	while(1);
	return 0;
}

void AllocM2(double **A, int M, int N){

	A = (double **)malloc(sizeof(double *) * M );
	for(int i=0; i < M; i++){

		*(A+i) = (double *)malloc(sizeof(double)*N);

	}


}
