
# GoogleTest - Unit testing
gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gmock_dep = gtest_proj.get_variable('gmock_dep')


# Unit tests
test_inc = ['GameCloneTests','ToolCardTests', '.']

e = executable('tests',
    include_directories: [test_inc, utils_inc, ai_inc, model_inc, controller_inc, sagrada_json_inc, signallers_inc, game_utils_inc], 
    sources: ['main.cpp'],
    dependencies: [gtest_dep, gmock_dep, json_dep, argparse_dep],
    link_with: [model_lib, utils_lib, game_utils_lib, controller_lib])    

test('gtest tests', e)
