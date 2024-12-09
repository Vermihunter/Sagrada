tournament_src = files(
    'Tournament.cpp', 
    'PlayerStatistics.cpp', 
    'DefaultPointMapper.cpp'
)

tournament_lib = static_library('tournament_lib',
    [tournament_src],
    include_directories: [tournament_inc, model_inc, game_utils_inc, utils_inc, ai_inc, controller_inc]
)

executable('tournament', 
    ['main.cpp'],
    include_directories: [game_utils_inc, utils_inc, ai_inc, model_inc, controller_inc, tournament_inc, sagrada_json_inc, signallers_inc],
    dependencies: [json_dep, argparse_dep],
    link_with: [model_lib, game_utils_lib, tournament_lib, ai_lib, utils_lib, controller_lib]
)