cmake_minimum_required(VERSION 3.10)
project(MysqlConn)


file(GLOB_RECURSE
    SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp
)
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ./main.cpp
)


target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_std_11
)
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/MysqlConn
)
target_link_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/lib
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        mysql
)