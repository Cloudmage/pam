cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(lineup LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../../pambase/")
include_directories("../../pamfft/")
include_directories("../../external/kiss_fft130")
include_directories("../../external/kiss_fft130/tools")

link_directories("../../lib")


file(GLOB SOURCES "*.cpp")

add_library(lineup MODULE ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(lineup PRIVATE ${flags})
target_compile_definitions(lineup PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ _test_ _MSL_STDINT_H wxDEBUG_LEVEL=0)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})


target_link_libraries(lineup ${wxWidgets_LIBRARIES})
target_link_libraries(lineup pambase)
target_link_libraries(lineup pamfft)

set_target_properties(lineup PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/test")


#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS lineup LIBRARY DESTINATION /usr/local/lib/pam2/test)
endif()	
