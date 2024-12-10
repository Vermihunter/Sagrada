# Dependencies
# GTK dependencies - GUI
gtkdep = dependency('gtk+-3.0')
gtkmmdep = dependency('gtkmm-3.0')

subdir('Pages')
subdir('Images')
subdir('ToolCards')

view_src = [
    page_src, 
    tc_view_src, 
    image_src, 
    files(
        'Image.cpp',
        'RoundTrackView.cpp',
        'ViewContext.cpp',
        'SagradaWindow.cpp',
        'BoardFieldView.cpp',
        'Digits.cpp',
        'RoundNumberView.cpp',
        'ScoreBoard.cpp',
        'PlayerOrderDisplayer.cpp',
        'RectangularBoardView.cpp',
    )
]

executable('sagrada', 
    [view_src, 'main.cpp'], 
    include_directories: [view_inc, tournament_inc,  signallers_inc, game_utils_inc,  model_inc, utils_inc, sagrada_json_inc, controller_inc, ai_inc], 
    dependencies: [gtkdep, gtkmmdep, json_dep, argparse_dep],
    link_with: [model_lib, game_utils_lib, tournament_lib, ai_lib, utils_lib, controller_lib]
)


executable('wpc_saver', 
    [view_src, 'wpc_png_saver_main.cpp'], 
    include_directories: [view_inc, model_inc, utils_inc, sagrada_json_inc, tournament_inc, controller_inc, ai_inc, game_utils_inc, signallers_inc], 
    dependencies: [gtkdep, gtkmmdep, json_dep, argparse_dep],
    link_with: [model_lib, utils_lib, controller_lib, ai_lib, game_utils_lib, tournament_lib],
    build_by_default: false
)
