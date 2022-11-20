set(target ${MP2_LIBRARY_FORMULA})

file(GLOB hdrs "*.h*" "${MP2_INCLUDE_STACK}/*.h*")
file(GLOB srcs "*.cpp")

add_library(${target} STATIC ${srcs} ${hdrs})
target_link_libraries(${target} ${LIBRARY_DEPS})