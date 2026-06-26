add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/include/libtokenizer
        COMMENT "Copying headers to build directory..."
)
