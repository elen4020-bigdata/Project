#include <iostream>
#include <array>
#include <fstream>
#include "mpi.h"

using namespace std;

int main(){

	MPI_Init(NULL,NULL);

	int my_rank, world_size;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Datatype type;

	MPI_File writefile;
    fstream file;
    file.open("inputMatrix.txt");
	MPI_Status status;
	int rc2 = MPI_File_open(MPI_COMM_WORLD, "input", MPI_MODE_RDWR | MPI_MODE_CREATE, MPI_INFO_NULL, &writefile);	
	if(rc2){
		printf("file write error opening\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}			    
	int dim = 0;
	file >> dim;
	int* matrix = new int[dim*dim+1];
    matrix[0] = dim;
		int a;
        for(int i = 0; i < dim*dim; i++){
            file >> a;
            matrix[i + 1] = a;
        }
        MPI_File_write(writefile, matrix, dim*dim+1, MPI_INT, &status);

	delete [] matrix;
	MPI_File_close(&writefile);
    file.close();
	MPI_Finalize();
}
