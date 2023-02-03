cmake_minimum_required(VERSION 3.6)

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--hash-style=gun")




add_library( # Sets the name of the library.
        mycpp-lib

         # Sets the library as a shared library.
         SHARED

         # Provides a relative path to your source file(s).
        Mycpp.cpp)



find_library( # Sets the name of the path variable. this is NDK's library
          log-lib

          # Specifies the name of the NDK library that
          # you want CMake to locate.
          log )

target_link_libraries( # Specifies the target library.
                   mycpp-lib

                   # Links the target library to the log library
                   # included in the NDK.
                   ${log-lib} )