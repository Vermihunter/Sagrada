#include "SagradaWindow.h"

SagradaWindow::SagradaWindow(const SizeInPixels& _size, game_mode_t gameMode,
                             tournament_config_t tournamentConfig)
    : homePage(ViewContext{_size, Point{0, 0}, *this} /*, gameMode, std::move(tournamentConfig)*/),
      sagradaLogo(v_constants::sagradaLogo::path, v_constants::sagradaLogo::size), size(_size)
{
	set_size_request(size.width, size.height);

	fixed_Logo.set_size_request(size.width, 100);
	fixed_Logo.put(sagradaLogo, (size.width - v_constants::sagradaLogo::size.width) / 2, 10);
	fixed.put(fixed_Logo, 0, 0);

	add_page(homePage);
	currPage = &homePage;
	show_home_page();

	add(fixed);
	show_all_children();

	if (gameMode == game_mode_t::SIMULATION) {
		homePage.show_simulation(std::move(tournamentConfig));
	}
	/*
	if(gameMode == game_mode_t::NONE) {
	    currPage = &homePage;
	    show_home_page();
	}
	else {
	    homePage.hide();
	    fixed_Logo.hide();
	}
	*/
}

void SagradaWindow::show_home_page()
{
	show_page(homePage, true);
}

void SagradaWindow::show_page(Page& page, bool withSagradaLogo)
{
	if (currPage) {
		currPage->hide();
	}
	currPage = &page;
	currPage->show();

	fixed_Logo.set_visible(withSagradaLogo);
}

void SagradaWindow::show_custom_config_page()
{
	if (!customConfigPage) {
		customConfigPage =
		    std::make_unique<CustomConfigPage>(ViewContext{size, Point{0, 0}, *this});
		add_page(*customConfigPage);
	}

	show_page(*customConfigPage);
}

void SagradaWindow::add_page(Page& newPage, const Point& pos)
{
	fixed.put(newPage, pos.x, pos.y);
}

void SagradaWindow::remove_page(Page& page)
{
	fixed.remove(page);
}