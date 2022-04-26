# PLUGINS
set(PLUGINS
    RemoveChannels
    )

# add nuke plugin linked to ddimage lib
function(add_nuke_plugin PLUGIN_NAME)
    add_library(${PLUGIN_NAME} MODULE ${ARGN})
    add_library(NukePlugins::${PLUGIN_NAME} ALIAS ${PLUGIN_NAME})
    target_link_libraries(${PLUGIN_NAME} PRIVATE ${NUKE_DDIMAGE_LIBRARY})
    set_target_properties(${PLUGIN_NAME} PROPERTIES PREFIX "")
    if (APPLE)
        set_target_properties(${PLUGIN_NAME} PROPERTIES SUFFIX ".dylib")
    endif()
endfunction()

# include directories
include_directories(${NUKE_INCLUDE_DIRS})

# WRAPPER LIBRARIES

# add configuration 
foreach(PLUGIN_NAME ${PLUGINS})
    add_nuke_plugin(${PLUGIN_NAME} ${PLUGIN_NAME}.cpp)
endforeach()

# install files
install(TARGETS 
        ${PLUGINS} 
        DESTINATION .)
