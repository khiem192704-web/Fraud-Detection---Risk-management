# CMake generated Testfile for 
# Source directory: D:/study/courses in university/ki 3/PBL 2/Electronic-Payment-Fraud-Detection---Risk-management (1)
# Build directory: D:/study/courses in university/ki 3/PBL 2/Electronic-Payment-Fraud-Detection---Risk-management (1)/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(AllTests "D:/study/courses in university/ki 3/PBL 2/Electronic-Payment-Fraud-Detection---Risk-management (1)/build/Debug/epfd_tests.exe")
  set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "D:/study/courses in university/ki 3/PBL 2/Electronic-Payment-Fraud-Detection---Risk-management (1)/CMakeLists.txt;32;add_test;D:/study/courses in university/ki 3/PBL 2/Electronic-Payment-Fraud-Detection---Risk-management (1)/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(AllTests "D:/study/courses in university/ki 3/PBL 2/Electronic-Payment-Fraud-Detection---Risk-management (1)/build/Release/epfd_tests.exe")
  set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "D:/study/courses in university/ki 3/PBL 2/Electronic-Payment-Fraud-Detection---Risk-management (1)/CMakeLists.txt;32;add_test;D:/study/courses in university/ki 3/PBL 2/Electronic-Payment-Fraud-Detection---Risk-management (1)/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(AllTests "D:/study/courses in university/ki 3/PBL 2/Electronic-Payment-Fraud-Detection---Risk-management (1)/build/MinSizeRel/epfd_tests.exe")
  set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "D:/study/courses in university/ki 3/PBL 2/Electronic-Payment-Fraud-Detection---Risk-management (1)/CMakeLists.txt;32;add_test;D:/study/courses in university/ki 3/PBL 2/Electronic-Payment-Fraud-Detection---Risk-management (1)/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(AllTests "D:/study/courses in university/ki 3/PBL 2/Electronic-Payment-Fraud-Detection---Risk-management (1)/build/RelWithDebInfo/epfd_tests.exe")
  set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "D:/study/courses in university/ki 3/PBL 2/Electronic-Payment-Fraud-Detection---Risk-management (1)/CMakeLists.txt;32;add_test;D:/study/courses in university/ki 3/PBL 2/Electronic-Payment-Fraud-Detection---Risk-management (1)/CMakeLists.txt;0;")
else()
  add_test(AllTests NOT_AVAILABLE)
endif()
