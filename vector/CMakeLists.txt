cmake_minimum_required (VERSION 2.8.11)

project (DDCD-vector-container)

set(SOURCES
        app/src/kernel/Server.cpp
        app/src/utils.cpp
        app/src/handlers/mainHandler.cpp
        app/src/handlers/additionHandler.cpp
        )

set(TARGET app/src/app.cpp)

add_executable(app ${TARGET} ${SOURCES})

target_include_directories(app PRIVATE ${PROJECT_SOURCE_DIR}/app/include)

find_package(Threads REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(app Threads::Threads)

#target_compile_definitions (app PRIVATE -DDEBUG)
