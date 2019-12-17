test_array = [
  ['tape_test','tape_exe','tape.cpp'],
  ['ad_test','ad_exe','ad.cpp'],
  ['tape_vector_test','tape_vector_exe','tape_vector.cpp'],
  ['derivatives_test','derivatives_exe','derivatives.cpp'],
  ['nested_2nd_order_test','nested_2nd_order_exe','nested_2nd_order.cpp'],
]

foreach test : test_array
  test(test.get(0),
       executable(test.get(1),
		  test.get(2),
		  dependencies
 		  : [ AutoDiffCpp_dep, gtest_dep ]))
endforeach
