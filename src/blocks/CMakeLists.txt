# Compile each standalone program

INCLUDE_DIRECTORIES(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/../) 

set(PROGRAMS 
             sqabs
	     sqascii
             sqbin
	     sqchop
             sqconjugate 
             sqcrossmultiply
             sqedgechop
             sqgetimgtfp
             sqimag 
	     sqfft 
             sqgensine
             sqmix
             sqoffset 
	     sqoverlap
	     sqpad
	     sqphase
             sqpnm
             sqpower 
             sqread 
             sqreal 
             sqsample 
             sqscaleandrotate 
             sqsum 
             sqwindow 
             sqwola
             sqzoom
   )

set(SCRIPTS sqautocorr
            sqcrosscorr
            sqconvolution
            sqtfp
            sqwaterfalls
   )


foreach(program ${PROGRAMS})
    add_executable(${program} ${program}.c)
    target_link_libraries(${program} setikit)
    install(TARGETS ${program} EXPORT setikit DESTINATION bin)
endforeach(program)

foreach(script ${SCRIPTS})
    install(PROGRAMS ${script}.sh DESTINATION bin)
endforeach(script)


