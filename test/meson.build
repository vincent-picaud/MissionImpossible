test_array = [['tape_test','tape_exe','tape.cpp']]

foreach test : test_array
  test(test.get(0),
       executable(test.get(1),
		  test.get(2),
		  dependencies
 		  : [ AutoDiffCpp_dep, gtest_dep ]))
endforeach
