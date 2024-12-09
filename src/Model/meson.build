
subdir('Board')
subdir('ToolCards')
subdir('PublicObjectiveCards')

model_src = [ 
    board_src, 
    puoc_src, 
    tc_src,
    files(
    'BasicMoveHandler.cpp', 
    'Attributes.cpp', 
    'Moves/RelocateDieMove.cpp', 
    'DiceSupply.cpp', 
    'Die.cpp', 
    'Game/Game.cpp',
    'Game/GameState.cpp',
    'Game/GameContext.cpp',
    'Game/GameInfo.cpp', 
    'RoundTrack.cpp', 
    'EvalState.cpp' 
)]


# Model shared library
model_lib = static_library('model', 
    [model_src],
    include_directories: [model_inc, utils_inc] 
)