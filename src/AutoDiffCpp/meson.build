AutoDiffCpp_headers = [
  'tape.hpp',
  'ad_fwd.hpp',
  'ad.hpp',
  'identity.hpp',
  'type_traits.hpp',
  'tape_vector.hpp',
  'derivatives.hpp',
  'always.hpp',
  'final_value_type.hpp',

  'new_ad_fwd.hpp',
  'new_ad.hpp',
]

AutoDiffCpp_sources = ['tape.cpp']

AutoDiffCpp_lib = library('AutoDiffCpp',
			  include_directories : inc,
			  install : true,
			  sources: [AutoDiffCpp_headers,AutoDiffCpp_sources])

AutoDiffCpp_dep = declare_dependency(include_directories : inc,
				     link_with : AutoDiffCpp_lib)

install_headers(AutoDiffCpp_headers,
		subdir : 'AutoDiffCpp')

