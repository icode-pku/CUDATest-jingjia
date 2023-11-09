project(cuda_api_test)
set(CMAKE_CURRENT_SOURCE_DIR ".")
set(3rdPARTYLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdpartylib)
set(CUDA_ROOT_DIR "/share/software/cuda-11.7")
#benchMark
set(benchMark_INCLUDE_DIR ${3rdPARTYLIB_DIR}/libbenchmark/include/)
set(benchMark_LIBRARY_DIR ${3rdPARTYLIB_DIR}/libbenchmark/lib)
file(GLOB banchmark_LIBS ${benchMark_LIBRARY_DIR}/*.a)
include_directories(${benchMark_INCLUDE_DIR})
link_directories(${benchMark_LIBRARY_DIR})
MESSAGE(" --benchMark_INCLUDE_DIR: " ${benchMark_INCLUDE_DIR})
MESSAGE(" --benchMark_LIBRARY_DIR: " ${${benchMark_LIBRARY_DIR}})
MESSAGE(" --banchmark_LIBS: " ${banchmark_LIBS})
#gtest
set(GTest_INCLUDE_DIR ${3rdPARTYLIB_DIR}/libGtest/include/)
set(GTest_LIBRARY_DIR ${3rdPARTYLIB_DIR}/libGtest/lib)
file(GLOB GTest_LIBS ${GTest_LIBRARY_DIR}/*.a)
#cuda
set(CUDA_ENABLE true)
if(CUDA_ENABLE)
    enable_language(CUDA)
endif()
add_compile_options(-fPIC)
set(CUDA_INCLUDE_DIR ${CUDA_ROOT_DIR}/include/)
set(CUDA_LIBRARY_DIR ${CUDA_ROOT_DIR}/lib64)
set(CUDA_RUNTIME_INCLUDE_DIR ${CUDA_ROOT_DIR}/targets/x86_64-linux/include)
set(CUDA_RUNTIME_LIBRARY_DIR ${CUDA_ROOT_DIR}/targets/x86_64-linux/lib)
file(GLOB CUDA_RUNTIME_LIBS ${CUDA_RUNTIME_LIBRARY_DIR}/*.so)
file(GLOB CUDA_LIBS ${CUDA_LIBRARY_DIR}/*.so)
include_directories(${CUDA_INCLUDE_DIR})
link_directories(${CUDA_LIBRARY_DIR})
MESSAGE("--CUDA_INCLUDE_DIR: " ${CUDA_INCLUDE_DIR})
MESSAGE("--CUDA_LIBRARY_DIR: " ${CUDA_LIBRARY_DIR})
MESSAGE("--CUDA_LIBS: " ${CUDA_LIBS})
MESSAGE("--CUDA_RUNTIME_LIBS :" ${CUDA_RUNTIME_LIBS})

#protobuf
set(protobuf_INCLUDE_DIR ${3rdPARTYLIB_DIR}/libprotobuf/include/)
set(protobuf_LIBRARY_DIR ${3rdPARTYLIB_DIR}/libprotobuf/lib)
file(GLOB protobuf_LIBS ${protobuf_LIBRARY_DIR}/*.a)
include_directories(${protobuf_INCLUDE_DIR})
link_directories(${protobuf_LIBRARY_DIR})



set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/)

file(GLOB ${PROJECT_NAME}_SRC_LIST 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cu
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

MESSAGE(" --SOURCES : " ${SOURCES})
MESSAGE(" --${PROJECT_NAME}_SRC_LIST  : " ${${PROJECT_NAME}_SRC_LIST})
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${CUDA_LIBS} ${banchmark_LIBS} ${GTest_LIBS} ${protobuf_LIBS} ${CUDA_RUNTIME_LIBS})

include_directories(${CUDA_INCLUDE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/include/)
file(GLOB test_SRC_LIST 
${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/*.h
${CMAKE_CURRENT_SOURCE_DIR}/*.cu)
MESSAGE("--test_SRC_LIST: " ${test_SRC_LIST})

add_executable(test ${test_SRC_LIST})
target_link_libraries(test ${CUDA_LIBS} ${CUDA_RUNTIME_LIBS} ${PROJECT_NAME})
