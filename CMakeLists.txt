project (client)
cmake_minimum_required(VERSION 3.5.1)
option(JS_ONLY "Compiles to native JS (No WASM)" OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")    

include_directories(include)

file(GLOB SOURCES src/*.cpp)
file(GLOB EXTERNAL_SOURCES "src/ed25519/*.c" )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/public")
add_executable(a ${SOURCES} ${EXTERNAL_SOURCES})

target_link_libraries(a ${CMAKE_CURRENT_SOURCE_DIR}/lib/libcrypto.a ${CMAKE_CURRENT_SOURCE_DIR}/lib/libssl.a)

if(JS_ONLY)
	message(STATUS "Setting compilation target to native JavaScript")
	set(CMAKE_EXECUTABLE_SUFFIX ".js")
	set_target_properties(a PROPERTIES LINK_FLAGS "-s 'EXPORTED_RUNTIME_METHODS=[\"ccall\", \"cwrap\", \"intArrayFromString\", \"allocate\"]' -s WASM=0 -s EXPORTED_FUNCTIONS='[_main]'")
else(JS_ONLY)
	message(STATUS "Setting compilation target to WASM")
	set(CMAKE_EXECUTABLE_SUFFIX ".wasm.js")
	set_target_properties(a PROPERTIES LINK_FLAGS "-s 'EXPORTED_RUNTIME_METHODS=[\"ccall\", \"cwrap\", \"intArrayFromString\", \"allocate\"]' -s WASM=1 -s EXPORTED_FUNCTIONS='[_main]'")
endif(JS_ONLY)