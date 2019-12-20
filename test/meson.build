test_array = [
  ['tape_test','tape_exe','tape.cpp'],
  ['ad_test','ad_exe','ad.cpp'],
  ['tape_vector_test','tape_vector_exe','tape_vector.cpp'],
  ['derivatives_test','derivatives_exe','derivatives.cpp'],
  ['final_value_type_test','final_value_type_exe','final_value_type.cpp'],
  ['nested_2nd_order_test','nested_2nd_order_exe','nested_2nd_order.cpp'],

  ['ad_new_test','ad_new_exe','ad_new.cpp'],
]

foreach test : test_array
  test(test.get(0),
       executable(test.get(1),
		  test.get(2),
		  dependencies
 		  : [ AutoDiffCpp_dep, gtest_dep ]))
endforeach
