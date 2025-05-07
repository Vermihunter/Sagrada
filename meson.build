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

executable('tournament', 
    ['src/Tournament/main.cpp'],
    include_directories: [game_utils_inc, utils_inc, ai_inc, model_inc, controller_inc, tournament_inc, sagrada_json_inc, signallers_inc],
    dependencies: [json_dep, argparse_dep],
    link_with: [model_lib, game_utils_lib, tournament_lib, ai_lib, utils_lib, controller_lib]
)

executable('sagrada', 
    [view_src, 'src/View/main.cpp'], 
    include_directories: [view_inc, tournament_inc,  signallers_inc, game_utils_inc,  model_inc, utils_inc, sagrada_json_inc, controller_inc, ai_inc], 
    dependencies: [gtkdep, gtkmmdep, json_dep, argparse_dep],
    link_with: [model_lib, game_utils_lib, tournament_lib, ai_lib, utils_lib, controller_lib]
)


e = executable('tests_sagrada',
    include_directories: [test_inc, utils_inc, ai_inc, model_inc, controller_inc, sagrada_json_inc, signallers_inc, game_utils_inc], 
    sources: ['tests/main.cpp'],
    dependencies: [gtest_dep, gmock_dep, json_dep, argparse_dep],
    link_with: [model_lib, utils_lib, game_utils_lib, controller_lib])    

test('gtest tests', e)
