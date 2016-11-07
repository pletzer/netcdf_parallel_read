# if you don't specify a minumum version a warning will be printed
cmake_minimum_required (VERSION 2.8)

enable_language(CXX)

find_package(MPI)

#
# Find the netcdf library and its dependencies by keying off from the nc-config
# command. 
#
set(NETCDF_INCLUDE_FLAG "" CACHE PATH "Directory containing the netCDF include and module files")
set(NETCDF_LIBRARIES "" CACHE PATH "List of netCDF libraries")

if (NETCDF_INCLUDE_DIR STREQUAL "")
    execute_process(COMMAND "nf-config" "--includedir"
                    OUTPUT_VARIABLE NETCDF_INCLUDE_DIR)
endif()

if (NETCDF_LIBRARIES STREQUAL "")
    execute_process(COMMAND "nc-config" "--libs"
                    OUTPUT_VARIABLE NETCDF_LIBRARIES)
    execute_process(COMMAND "nc-config" "--cflags"
                    OUTPUT_VARIABLE NETCDF_INCLUDE_FLAG)
endif()

# remove \n at the end of the above variables
string(STRIP "${NETCDF_INCLUDE_FLAG}" NETCDF_INCLUDE_FLAG)
string(STRIP "${NETCDF_LIBRARIES}" NETCDF_LIBRARIES)

message(STATUS "Netcdf include flag: ${NETCDF_INCLUDE_FLAG}")
message(STATUS "Netcdf libraries: ${NETCDF_LIBRARIES}")

set(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})
set(CMAKE_CXX_FLAGS ${NETCDF_INCLUDE_FLAG})

# create an executable from the given source
add_executable(test1 test1.cxx)
target_link_libraries(test1 ${NETCDF_LIBRARIES})

