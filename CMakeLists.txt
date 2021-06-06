cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)


# You need the cmake modules MYSQL and SQLite3 in
# https://github.com/matt-42/lithium/tree/master/cmake
# for this to work

find_package(CURL REQUIRED)
find_package(Threads REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED context)


include_directories(${CURL_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${PostgreSQL_INCLUDE_DIRS})

set(LIBS ${CURL_LIBRARIES} 
          ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT}
          ${PostgreSQL_LIBRARIES} ${OPENSSL_LIBRARIES})

# Directory where lithium headers are located if not in the default include path.
include_directories($ENV{HOME}/local/include)

add_custom_target(symbols_generation COMMAND li_symbol_generator ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(lithium-realworld-example-app conduit.cc)
add_dependencies(lithium-realworld-example-app symbols_generation)
target_link_libraries(lithium-realworld-example-app ${LIBS})
