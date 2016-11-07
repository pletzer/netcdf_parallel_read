#include <netcdf.h>
#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {

	MPI_Init(&argc, &argv);

	int nproc, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

	if (argc < 3) {
		if (rank == 0) {
			std::cerr << "ERROR: need to provide file name and variable name\n";
			std::cerr << argv[0] << " filename varname\n";
		}
		MPI_Finalize();
		return 1;
	}
	char* filename = argv[1];
	char* varname = argv[2];
	MPI_Finalize();
	return 0;
}