#ifndef PLAYER_ORDER_DISPLAYER_H
#define PLAYER_ORDER_DISPLAYER_H

#include "Attributes.h"
#include "GameState.h"
#include "PlayerInformation.h"
#include "SizeInPixels.h"
#include "Typedefs.h"

#include <gtkmm.h>

class PlayerOrderDisplayer : public Gtk::DrawingArea {
  public:
	PlayerOrderDisplayer(const GameState& _gameState, const player_c& _players,
	                     const SizeInPixels _size);

  protected:
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

  private:
	const GameState& gameState;
	const player_c&  players;
	SizeInPixels     size;
	const Color&     defaultNameColor;
	const Color&     playerOnMoveNameColor;
};

#endif // PLAYER_ORDER_DISPLAYER_H