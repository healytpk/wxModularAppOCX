set (SRCS
	wxModularCore.cpp
	wxModularCoreSettings.cpp
	wxGuiPluginBase.cpp
	wxGuiPluginWindowBase.cpp)

set (HEADERS
	wxModularCore.h
	wxModularCoreSettings.h
	Declarations.h
	wxGuiPluginBase.h
	wxGuiPluginWindowBase.h
	host_interaction.hpp
	Auto.h)
	
set(LIBRARY_NAME wxModularCore)

if(WIN32)
	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/D__STDC_CONSTANT_MACROS;/DDEMO_PLUGIN_EXPORTS)
endif(WIN32)

set(SRCS ${SRCS} ${HEADERS})

add_definitions(${PREPROCESSOR_DEFINITIONS})

include_directories(${INCLUDE_DIRECTORIES} ${BASE_INCLUDE_DIRECTORIES})
	
link_directories(${LINK_DIRECTORIES})

add_library(${LIBRARY_NAME} STATIC ${SRCS})

set(DLL_DIR bin)
set(TARGET_LOCATION ${PROJECT_SOURCE_DIR}/${DLL_DIR}/${CMAKE_CFG_INTDIR})
set_target_properties(${LIBRARY_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TARGET_LOCATION})
