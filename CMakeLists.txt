# if you don't specify a minumum version a warning will be printed
cmake_minimum_required (VERSION 2.8)

enable_language(CXX)

find_package(MPI)

#
# Find the netcdf library and its dependencies by keying off from the nc-config
# command. 
#
set(NETCDF_INCLUDE_DIR "" CACHE PATH "Directory containing the netCDF include and module files")
set(NETCDF_LIBRARIES "" CACHE PATH "List of netCDF libraries")

if (NETCDF_INCLUDE_DIR STREQUAL "")
    execute_process(COMMAND "nf-config" "--includedir"
                    OUTPUT_VARIABLE NETCDF_INCLUDE_DIR)
endif()

if (NETCDF_LIBRARIES STREQUAL "")
    execute_process(COMMAND "nf-config" "--flibs"
                    OUTPUT_VARIABLE NETCDF_Fortran_LIBRARIES)
    string(STRIP "${NETCDF_Fortran_LIBRARIES}" NETCDF_Fortran_LIBRARIES)
    execute_process(COMMAND "nc-config" "--libs"
                    OUTPUT_VARIABLE NETCDF_C_LIBRARIES)
    string(STRIP "${NETCDF_C_LIBRARIES}" NETCDF_C_LIBRARIES)
    set(NETCDF_LIBRARIES "${NETCDF_Fortran_LIBRARIES} ${NETCDF_C_LIBRARIES}")
endif()

# remove \n at the end of the above variables
string(STRIP "${NETCDF_INCLUDE_DIR}" NETCDF_INCLUDE_DIR)
string(STRIP "${NETCDF_LIBRARIES}" NETCDF_LIBRARIES)

set(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})
#set(CMAKE_CXX_FLAGS -I${NETCDF_INCLUDE_DIR})

# create an executable from the given source
add_executable(test1 test1.cxx)
