project('sagrada', 'cpp', default_options : ['cpp_std=c++20'])
CXX='gcc'

# Dependencies
# GTK dependencies - GUI
gtkdep = dependency('gtk+-3.0')
gtkmmdep = dependency('gtkmm-3.0')

# JSON dependencies - Serialization
json_proj = subproject('nlohmann_json')
json_dep = json_proj.get_variable('nlohmann_json_dep')

# GoogleTest - Unit testing
gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gmock_dep = gtest_proj.get_variable('gmock_dep')

# CMD line argument parsing
argparse_proj = subproject('argparse')
argparse_dep = argparse_proj.get_variable('argparse_dep')


subdir('src')
subdir('include')


sagrada_src = [view_src, tournament_src]
sagrada_inc = [view_inc, tournament_inc,  signallers_inc]

# Model shared library
model_lib = static_library('model', 
    [model_src, utils_src],
    include_directories: [model_inc, utils_inc] 
)

executable('sagrada', 
    [sagrada_src, 'src/View/main.cpp', game_utils_src, ai_src, utils_src, controller_src], 
    include_directories: [game_utils_inc, sagrada_inc, model_inc, utils_inc, sagrada_json_inc, controller_inc, ai_inc], 
    dependencies: [gtkdep, gtkmmdep, json_dep, argparse_dep],
    link_with: model_lib
)

# executable('wpc_saver', 
#     [sagrada_src, 'src/View/wpc_png_saver_main.cpp'], 
#     include_directories: [sagrada_inc, model_inc, utils_inc, sagrada_json_inc, controller_inc, ai_inc], 
#     dependencies: [gtkdep, gtkmmdep, json_dep, argparse_dep],
#     link_with: model_lib
# )

# Tournament exe
all_tournament_src = [game_utils_src, utils_src, ai_src, controller_src, tournament_src, 'src/Tournament/main.cpp']
executable('tournament', 
    all_tournament_src, 
    include_directories: [game_utils_inc, utils_inc, ai_inc, model_inc, controller_inc, tournament_inc, sagrada_json_inc, signallers_inc],
    dependencies: [json_dep, argparse_dep],
    link_with: model_lib
)


# Unit tests
test_inc = ['tests/GameCloneTests','tests/ToolCardTests', 'tests']

# e = executable('tests',
#     include_directories: [test_inc, utils_inc, ai_inc, model_inc, controller_inc, sagrada_json_inc, signallers_inc], 
#     sources: ['tests/main.cpp'],
#     dependencies: [gtest_dep, gmock_dep, json_dep, argparse_dep],
#     link_with: model_lib)    

# test('gtest tests', e)
