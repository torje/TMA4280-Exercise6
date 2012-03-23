PROJECT(poisson)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# comment if you only going to write fortran code
ENABLE_LANGUAGE(C)

# needed no matter since the fst is written in fortran
ENABLE_LANGUAGE(Fortran)

# replace poisson.c with poisson.f if you want to compile the fortran version
FILE(GLOB_RECURSE SRCS poisson_opprydning.c utils.c fst.f)

ADD_EXECUTABLE(poisson ${SRCS})
add_definitions(-std=c99 -fopenmp)
SET_TARGET_PROPERTIES(poisson PROPERTIES LINKER_LANGUAGE C)
