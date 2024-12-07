#include "LocalPlayerGameTypeChoosingSubpage.h"

#include "SagradaWindow.h"

LocalPlayerGameTypeChoosingSubpage::LocalPlayerGameTypeChoosingSubpage(
    const ViewContext& ctx)
    : Page(ctx), button_LocalPlayerVsAi("Computer"), button_Back("back")
{
    button_LocalPlayerVsAi.signal_clicked().connect(sigc::mem_fun(
        *this, &LocalPlayerGameTypeChoosingSubpage::computer_button_clicked));

    button_Back.signal_clicked().connect(
        sigc::mem_fun(ctx.mainWindow, &SagradaWindow::show_home_page));

    vbox_GameTypeOptions.pack_start(button_LocalPlayerVsAi);
    vbox_GameTypeOptions.pack_start(button_Back, Gtk::PACK_EXPAND_WIDGET, 20);

    vbox_GameTypeOptions.set_size_request(200, 80);

    fixed.put(vbox_GameTypeOptions, (ctx.size.width - 200) / 2,
              ctx.size.height / 2);
    add(fixed);
    show_all_children();
}

void LocalPlayerGameTypeChoosingSubpage::computer_button_clicked()
{
    if (!configChoosingPage) {
        configChoosingPage =
            std::make_unique<LocalPlayerGameConfigurationSubpage>(ctx);
        ctx.mainWindow.add_page(*configChoosingPage);
    }

    ctx.mainWindow.show_page(*configChoosingPage, true);
}
