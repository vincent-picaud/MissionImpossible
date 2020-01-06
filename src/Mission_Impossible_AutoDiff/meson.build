Mission_Impossible_AutoDiff_headers = [
  'tape.hpp',
  'ad_fwd.hpp',
  'ad.hpp',
  'identity.hpp',
  'tape_vector.hpp',
  'derivatives.hpp',
  'always.hpp',
  'final_value_type.hpp',
  'mission_impossible_tape.hpp',
  # main header 
  'Mission_Impossible.hpp',
  'functions.hpp',
]

Mission_Impossible_AutoDiff_sources = ['tape.cpp']

Mission_Impossible_AutoDiff_lib = library('Mission_Impossible_AutoDiff',
			  include_directories : inc,
			  install : true,
			  sources: [Mission_Impossible_AutoDiff_headers,Mission_Impossible_AutoDiff_sources])

Mission_Impossible_AutoDiff_dep = declare_dependency(include_directories : inc,
				     link_with : Mission_Impossible_AutoDiff_lib)

install_headers(Mission_Impossible_AutoDiff_headers,
		subdir : 'Mission_Impossible_AutoDiff')

