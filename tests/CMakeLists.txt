
set(EIGEN_SAVE_BUILD_TESTING ${BUILD_TESTING})
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(../third_party/eigen ${CMAKE_CURRENT_BINARY_DIR}/eigen)
set(BUILD_TESTING ${EIGEN_SAVE_BUILD_TESTING} CACHE BOOL "" FORCE)


add_library(P2M
        P2M/KDTree.cpp
        P2M/KDTree.h
        P2M/Model.cpp
        P2M/Model.h
        P2M/RTree.cpp
        P2M/RTree.h
        P2M/tetgen.cpp
        P2M/tetgen.h
        P2M/VoronoiTetgen.cpp
        P2M/VoronoiTetgen.h
)

target_link_libraries(P2M PUBLIC Eigen3::Eigen)
target_include_directories(P2M PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/P2M")

add_executable(tests tests.cpp)

target_link_libraries(tests PRIVATE mantis P2M)
target_compile_definitions(tests PRIVATE ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../assets/")

add_test(NAME tests COMMAND tests)
