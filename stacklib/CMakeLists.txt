set(target ${MP2_LIBRARY_STACK})

file(GLOB hdrs "*.h*")
file(GLOB srcs "*.cpp")

add_library(${target} STATIC ${srcs} ${hdrs})
target_link_libraries(${target} ${LIBRARY_DEPS})