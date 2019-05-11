#include <iostream>
#include <array>
#include <fstream>
#include "mpi.h"

using namespace std;

int main(int argc, const char *argv[]){

	if (argc < 2) {
		printf("No file specified \n");
		return 1;
	}

	double start, end;
	printf("Starting programme\n");
	MPI_Init(NULL,NULL);
	start = MPI_Wtime();

	int my_rank, world_size;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Datatype type;

	MPI_File readfile, writefile;
	MPI_Status status;
	int rc1 = MPI_File_open(MPI_COMM_WORLD, argv[1], MPI_MODE_RDWR, MPI_INFO_NULL, &readfile);
	if(rc1){
		printf("file read error\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
	int rc2 = MPI_File_open(MPI_COMM_WORLD, "output", MPI_MODE_RDWR | MPI_MODE_CREATE, MPI_INFO_NULL, &writefile);	
	if(rc2){
		printf("file write error opening\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}			    
	int dim = 0;
	MPI_File_read(readfile, &dim, 1, MPI_INT, &status);
	if(dim%(world_size - 1) != 0){
		printf("Number of ranks invalid \n");
		MPI_Abort(MPI_COMM_WORLD, 2);
	}
	int blockSize = dim/(world_size - 1);
	int block[blockSize*blockSize];
	int* matrix = new int[dim*dim];
	if(my_rank == 0){
		MPI_File_read_at(readfile, sizeof(int), matrix, dim*dim, MPI_INT, &status);
	}
	for(int i = 0; i < (world_size - 1); i++){
		if(my_rank == 0){
			MPI_Type_vector(blockSize, blockSize, dim, MPI_INT, &type);
			MPI_Type_commit(&type);
			for(int j = 0; j < (world_size - 1); j++){
				MPI_Send(&matrix[(blockSize*dim*j)+(blockSize*i)], 1, type, (j + 1), 0, MPI_COMM_WORLD);
			}
			MPI_Type_free(&type);
		}else{
			MPI_Recv(&block, blockSize*blockSize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for(int k = 0; k < blockSize; k++){
				for(int l = k; l < blockSize; l++){
					if(k != l){
						block[(blockSize*k) + l] = block[(blockSize*k) + l] + block[(blockSize*l) + k];
						block[(blockSize*l) + k] = block[(blockSize*k) + l] - block[(blockSize*l) + k];
						block[(blockSize*k) + l] = block[(blockSize*k) + l] - block[(blockSize*l) + k];
					}
				}
			}
			MPI_Send(&block, blockSize*blockSize, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
		if(my_rank == 0){
			for(int j = 0; j < (world_size - 1); j++){
				MPI_Recv(&block, blockSize*blockSize, MPI_INT, (j + 1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				for(int k = 0; k < blockSize; k++){
					for(int l = 0; l < blockSize; l++){
						int a = block[k*blockSize + l];
						MPI_File_write_at(writefile, sizeof(int)*(i*dim*blockSize + blockSize*j + dim*k + l) , &a, 1, MPI_INT, &status);
					}
				}
			}	
		}
	}
	delete [] matrix;
	MPI_File_close(&readfile);
	MPI_File_close(&writefile);
	end = MPI_Wtime();
	MPI_Finalize();
	printf("Completed programme. Time taken %f \n", end-start);
}
