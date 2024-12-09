game_utils_src = files(
    'GameCache.cpp',
    'GameCacheInitializer.cpp',
    'GameContextBuilder.cpp'
)


game_utils_lib = static_library('game_utils',
    [game_utils_src],
    include_directories: [game_utils_inc, model_inc, utils_inc, controller_inc, sagrada_json_inc],
    dependencies: [json_dep]
)