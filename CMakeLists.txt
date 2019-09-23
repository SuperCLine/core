file(GLOB SOURCES src/*.cpp)

if (BUILD_CORE_STATIC)
    add_library(core STATIC ${SOURCES})
else ()
    add_library(core SHARED ${SOURCES})
endif ()

add_definitions(-DCORE_API=__DLL_EXPORT)
if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

target_include_directories(core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/inc/private"
    "${CMAKE_CURRENT_SOURCE_DIR}/inc/public"
)

set_target_properties(core PROPERTIES
    SOVERSION ${SOVERSION}
    VERSION ${VERSION}
    COTIRE_CXX_PREFIX_HEADER_INIT "${CMAKE_CURRENT_SOURCE_DIR}/inc/private/stdafx.h"
)
cotire(core)

install(TARGETS core
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    COMPONENT library
)

file(GLOB INCLUDES inc/public/*.h)
install(FILES ${INCLUDES} DESTINATION include)