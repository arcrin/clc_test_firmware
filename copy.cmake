if(EXISTS "${source}")
    configure_file("${source}" "${destination}" COPYONLY)
endif()
