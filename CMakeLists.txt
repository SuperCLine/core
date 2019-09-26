file(GLOB SOURCES src/*.cpp)

if (BUILD_CORE_STATIC)
    add_library(core STATIC ${SOURCES})
else ()
    add_library(core SHARED ${SOURCES})
endif ()

add_library(SuperCLine::core ALIAS core)

add_definitions(-DCORE_API=__DLL_EXPORT)
if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_REQUIRED_FLAGS -std=c++17)
# set_property(TARGET core PROPERTY CXX_STANDARD 17)

target_include_directories(core 
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/inc/private"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/inc/public"
)

message(STATUS "ver:${SOVERSION}-${VERSION}")
set_target_properties(core PROPERTIES
    SOVERSION ${SOVERSION}
    VERSION ${VERSION}
    COTIRE_CXX_PREFIX_HEADER_INIT "${CMAKE_CURRENT_SOURCE_DIR}/inc/private/stdafx.h"
)
cotire(core)

install(TARGETS core
    ARCHIVE DESTINATION lib/core
    LIBRARY DESTINATION lib/core
	RUNTIME DESTINATION bin
    COMPONENT library
)

file(GLOB INCLUDES inc/public/*.h)
install(FILES ${INCLUDES} DESTINATION include/core)