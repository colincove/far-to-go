if (Test-Path -Path build) 
{
    "Path exists!"
} 
else 
{
    mkdir build
}

#CMake Error at Physics/bullet3/CMakeLists.txt:1 (cmake_minimum_required):
#Compatibility with CMake < 3.5 has been removed from CMake.

#Update the VERSION argument <min> value.  Or, use the <min>...<max> syntax
#to tell CMake that the project requires at least <min> but has been updated
#to work with policies introduced by <max> or earlier.

#Or, add -DCMAKE_POLICY_VERSION_MINIMUM=3.5 to try configuring anyway.


#CMake Error at Physics/bullet3/test/gtest-1.7.0/CMakeLists.txt:16 (ADD_LIBRARY):
#  ADD_LIBRARY cannot create target "gtest" because another target with the
#  same name already exists.  The existing target is a static library created
#  in source directory
#  "C:/Users/colin/Documents/far-to-go/build/_deps/googletest-src/googletest".
#  See documentation for policy CMP0002 for more details.
cmake .\ -B .\build -DCMAKE_POLICY_VERSION_MINIMUM="3.5" -DBUILD_UNIT_TESTS=OFF