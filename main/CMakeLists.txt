idf_component_register(SRCS "main.c" "gaga.cpp"
                    INCLUDE_DIRS "."
                    EMBED_TXTFILES "sample.xml")


option(BUILD_SHARED_LIBS OFF)
option(BUILD_TESTING OFF)


function(install)
endfunction()

function(export)
endfunction()


add_subdirectory(lib/tinyxml2)


target_link_libraries(${COMPONENT_LIB} PUBLIC tinyxml2)
