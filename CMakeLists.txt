cmake_minimum_required ( VERSION 2.6 )

project ( calc )

set ( APP_NAME calc )

set ( SRC_DIR D:/infa/labINFA2/calc )
set( CMAKE_BUILD_TYPE DEBUG )
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

list(APPEND SRC ${SRC_DIR}/main.cpp )
list(APPEND SRC ${SRC_DIR}/BET.cpp )
list(APPEND SRC ${SRC_DIR}/calculator.cpp )
list(APPEND SRC ${SRC_DIR}/dictionary.cpp )
list(APPEND SRC ${SRC_DIR}/expression.cpp )
list(APPEND SRC ${SRC_DIR}/function.cpp )
list(APPEND SRC ${SRC_DIR}/input.cpp )
list(APPEND SRC ${SRC_DIR}/number.cpp )
list(APPEND SRC ${SRC_DIR}/output.cpp )
list(APPEND SRC ${SRC_DIR}/symbol.cpp )
list(APPEND SRC ${SRC_DIR}/util.cpp )
add_executable ( ${APP_NAME} ${SRC} )
