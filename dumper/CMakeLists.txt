# CMakeList.txt : CMake project for dumper, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# The following folder will be included
include_directories("${PROJECT_SOURCE_DIR}/dumper")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
set(LINK_CPP_PATH ${PROJECT_SOURCE_DIR}/dumper)
FILE(GLOB_RECURSE APP_SOURCES "*.cpp" "*.h" "*.hpp" "*.c")

function(group_by_dir src_dir)
  foreach(FILE ${ARGN})
      # Nhận đường dẫn tệp tuyệt đối
      get_filename_component(FULL_NAME "${FILE}" ABSOLUTE)
      # Nhận đường dẫn gốc của tệp
      get_filename_component(PARENT_DIR "${FULL_NAME}" PATH)
      # Loại bỏ đường dẫn gốc của mã nguồn trong đường dẫn chính
      string(REPLACE "${ARGV0}" "" GROUP "${PARENT_DIR}")
      # Đảm bảo rằng đường dẫn được sử dụng trên windows
      string(REPLACE "/" "\\" GROUP "${GROUP}")
      # Nhóm các tệp thành "Source Files" 和 "Header Files"
      #if("${FILE}" MATCHES ".*\\.h")
      #  set(GROUP "Header Files${GROUP}")
      #else()
      #  set(GROUP "Source Files${GROUP}")
      #endif()
      message(${FILE})
      source_group("${GROUP}" FILES "${FILE}")
  endforeach()
endfunction(group_by_dir)
group_by_dir(${LINK_CPP_PATH} ${APP_SOURCES})
# Add source to this project's executable.

# add_executable(dumper ${APP_SOURCES})
add_library(dumper SHARED ${APP_SOURCES})

# Set the output folder where your program will be created
#set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
# SET(CMAKE_EXE_LINKER_FLAGS  "/FORCE")
# TODO: Add tests and install targets if needed.
set(LINK_LIB_PATH ${PROJECT_SOURCE_DIR}/dumper/lib)

set(LINK_LIB_COMMON ${LINK_LIB_PATH}/detours.lib
)

target_link_options(${CMAKE_PROJECT_NAME} PUBLIC "$<$<C_COMPILER_ID:MSVC>:-SAFESEH:NO>")
TARGET_LINK_LIBRARIES(dumper ${LINK_LIB_COMMON} Threads::Threads)
if(WIN32)
  TARGET_LINK_LIBRARIES(dumper ws2_32.lib
                                wldap32.lib
                                advapi32.lib
                                kernel32.lib
                                comdlg32.lib
                                crypt32.lib
                                normaliz.lib
                                legacy_stdio_definitions.lib
  )
endif()