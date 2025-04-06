# NOTES FOR BUILDS (PROBABLY BELONGS SOMEWHERE ELSE)
# After adding DirectXShaderCompiler as a submodule I needed to install Python and add to path. 
# I needed to modify build steps. We had to add precompile path to build script. -C .\DirectXShaderCompiler\cmake\caches\PredefinedParams.cmake
# https://github.com/microsoft/DirectXShaderCompiler/blob/main/docs/BuildingAndTestingDXC.rst#basic-cmake-usage
# Also, we can add it into the Cmake file. But I have not verified if that is going to work. 
# include(${CMAKE_CURRENT_SOURCE_DIR}/DirectXShaderCompiler/cmake/caches/PredefinedParams.cmake)
# Also, I needed to call 'git submodule update --init' inside of the DirectXShaderCompiler folder.

# llvm-build: error: invalid target to enable: 'AMDGPU' (not in project)
# https://github.com/microsoft/DirectXShaderCompiler/issues/4480

# https://github.com/microsoft/DirectXShaderCompiler/issues/5081

# Unable to find TAEF binaries.
# We need to install TEAF
# https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk

# C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.41.34120/bin/Hostx64/x64/cl.exe
# is not a full path to an existing compiler tool.
# this is refering to clang c++ compile tools
# I am going to try to install clang build toolset in visual studio. 
# this worked

# at some points, I had this project builds as part of the overall cmake project. But it was conflicting on the version of gtest included in bullet physics. 
# instead of trying to resolve this, I just broke it off into it's own build process. At the end of the day I don't expect to build this often. 
# only 1 time, in fact. And to just use the build output for shader compiling afterwards. 

# C:\Users\colin\Documents\far-to-go\shader-compiler-build\Debug\bin
# add to path so that we can call the tool. 
# "dxc.exe, a command-line tool that can compile HLSL programs for shader model 6.0 or higher"

if (Test-Path -Path shader-compiler-build) 
{
    "Path exists!"
} 
else 
{
    mkdir shader-compiler-build
}

cmake .\DirectXShaderCompiler -C .\DirectXShaderCompiler\cmake\caches\PredefinedParams.cmake -B .\shader-compiler-build