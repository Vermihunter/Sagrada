
controller_src = files(
    'InputHandlers/BoardTextFileReader.cpp', 
    'SagradaGameCMDLineParser.cpp', 
    'LocalPlayerController.cpp', 
    'OnlyAIPlayerController.cpp'
)

controller_lib = static_library('controller_lib',
    [controller_src],
    include_directories: [tournament_inc, model_inc, game_utils_inc, utils_inc, ai_inc, controller_inc],
    dependencies: [argparse_dep]
)

