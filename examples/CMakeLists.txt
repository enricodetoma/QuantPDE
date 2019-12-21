ADD_EXECUTABLE(tutorial tutorial.cpp)

IF(JSONCPP_FOUND)
	ADD_EXECUTABLE(jump_diffusion jump_diffusion.cpp)
	ADD_EXECUTABLE(unequal_borrowing_lending_rates unequal_borrowing_lending_rates.cpp)
	ADD_EXECUTABLE(vanilla_options vanilla_options.cpp)

	TARGET_LINK_LIBRARIES(jump_diffusion
		debug ${JSONCPP_LIBRARY_DEBUG}
		optimized ${JSONCPP_LIBRARY_RELEASE})
	TARGET_LINK_LIBRARIES(unequal_borrowing_lending_rates
		debug ${JSONCPP_LIBRARY_DEBUG}
		optimized ${JSONCPP_LIBRARY_RELEASE})
	TARGET_LINK_LIBRARIES(vanilla_options
		debug ${JSONCPP_LIBRARY_DEBUG}
		optimized ${JSONCPP_LIBRARY_RELEASE})
ENDIF()
