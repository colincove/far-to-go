# CMake generated Testfile for 
# Source directory: C:/Users/colin/Documents/far-to-go
# Build directory: C:/Users/colin/Documents/far-to-go
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(Runs "C:/Users/colin/Documents/far-to-go/Debug/FarToGo.exe")
  set_tests_properties(Runs PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/colin/Documents/far-to-go/CMakeLists.txt;75;add_test;C:/Users/colin/Documents/far-to-go/CMakeLists.txt;0;")
endif()
subdirs("DirectX-Headers")
subdirs("Math")
subdirs("Metrics")
subdirs("MetricsTests")
subdirs("Physics")
subdirs("MathTests")
subdirs("Scripting")
subdirs("DX12")
subdirs("DX12Demo")
subdirs("DX12Tests")
subdirs("Graphics")
subdirs("GraphicsTests")
subdirs("GraphicsDemoDX12")
subdirs("RapidXML")
subdirs("imgui")
subdirs("Core")
