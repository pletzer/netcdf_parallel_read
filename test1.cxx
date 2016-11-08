#include <netcdf.h>
#include <iostream>
#include <mpi.h>
#include <vector>

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

	// open the file
	int ncid;
	int ier = nc_open(filename, NC_NOWRITE, &ncid);
	if (ier != NC_NOERR) {
		if (rank == 0) std::cerr << "ERROR: Cannot open file " << filename << '\n';
		MPI_Finalize();
		return 2;
	}


	// read the array
	int varid;
	ier = nc_inq_varid(ncid, varname, &varid);
	if (ier != NC_NOERR) {
		if (rank == 0) std::cerr << "ERROR: Cannot find variable named " << varname << '\n';
		MPI_Finalize();
		return 3;		
	}
	nc_type xtype;
	ier = nc_inq_vartype(ncid, varid, &xtype);
	if (xtype != NC_FLOAT) {
		if (rank == 0) std::cerr << "ERROR: variable named " << varname << " is not float\n";
		MPI_Finalize();
		return 4;
	}
	int ndims;
	ier = nc_inq_varndims(ncid, varid, &ndims);
	int dimids[ndims];
	ier = nc_inq_vardimid(ncid, varid, dimids);
	size_t dims[ndims];
	for (int i = 0; i < ndims; ++i) {
		ier = nc_inq_dimlen(ncid, dimids[i], &dims[i]);
	}
	size_t start[ndims];
	size_t ntot = 1;
	for (int i = 0; i < ndims; ++i) {
		start[i] = 0;
		ntot *= dims[i];
	}
	std::vector<float> data(ntot);
	ier = nc_get_vara_float(ncid, varid, start, dims, &data[0]);

	// close
	ier = nc_close(ncid);

	// make sure each array is the same
        float chksum = 0;
	for (size_t i = 0; i < data.size(); ++i) {
	  chksum += data[i];
	}
	std::cout << "[" << rank << "] chksum = " << chksum << '\n';
	if (rank == 0) std::cout << "SUCCESS\n";

	MPI_Finalize();
	return 0;
}
