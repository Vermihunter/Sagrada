ai_src = files(
    'MCTSPlayer.cpp', 
    'MCTSNode.cpp', 
    'MCTSPlayerConfig.cpp',
    'RulesBasedPlayer.cpp', 
    'RulesBasedPlayerConfig.cpp',
    'FirstPlayer.cpp', 
    'RandomPlayer.cpp', 
    'AI_Player.cpp', 
    'DefinedAIPlayers.cpp',
    'MinimaxPlayer.cpp', 
    'MinimaxPlayerConfig.cpp'
)


ai_lib = static_library('ai_lib',
    [ai_src],
    include_directories: [tournament_inc, model_inc, game_utils_inc, utils_inc, ai_inc, controller_inc, sagrada_json_inc]
)

