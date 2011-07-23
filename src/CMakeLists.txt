add_library(setikit ${LIB_TYPE}
                    sq_utils.c
                    sq_dsp.c
                    sq_imaging.c
                    sq_signals.c
                    sq_windows.c
                    )

set_target_properties(setikit PROPERTIES LINKER_LANGUAGE "C")
set_target_properties(setikit PROPERTIES OUTPUT_NAME "setikit")
set_target_properties(setikit PROPERTIES VERSION ${setikit_VERSION})
set_target_properties(setikit PROPERTIES SOVERSION ${setikit_VERSION_MAJOR})

set(CORELIBS ${FFTW_LIBRARIES} ${GSL_LIBRARY} m)
target_link_libraries(setikit ${CORELIBS})

INSTALL(TARGETS
    setikit
    DESTINATION lib
)
INSTALL(FILES
    sq_constants.h
    sq_dsp.h
    sq_imaging.h
    sq_signals.h
    sq_utils.h
    sq_windows.h
    DESTINATION include/${PROJECT_NAME}
)

add_subdirectory(blocks)
add_subdirectory(tests)