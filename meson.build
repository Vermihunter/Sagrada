project('sagrada', 'cpp', default_options : ['cpp_std=c++20'])
CXX='gcc'

# JSON dependencies - Serialization - used in multiple targets
json_proj = subproject('nlohmann_json')
json_dep = json_proj.get_variable('nlohmann_json_dep')


# CMD line argument parsing - used in multiple targets
argparse_proj = subproject('argparse')
argparse_dep = argparse_proj.get_variable('argparse_dep')

subdir('include')
subdir('src')
subdir('tests')