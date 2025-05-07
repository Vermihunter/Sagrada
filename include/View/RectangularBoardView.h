#ifndef BOARD_VIEW_H
#define BOARD_VIEW_H

#define _USE_MATH_DEFINES

#include "Attributes.h"
#include "BoardContext.h"
#include "BoardFieldView.h"
#include "Constants.h"
#include "Game.h"
#include "IBoardViewSignallable.h"
#include "ISignaller.h"
#include "PlayerInformation.h"
#include "RectangularBoardViewConstants.h"
#include "Typedefs.h"

#include <array>
#include <cmath>
#include <gtkmm/drawingarea.h>
#include <memory>
#include <queue>

class GamePlayingPage;

/**
 * @brief Represents the class responsible for displaying a WPC
 * Stores the board the content of which is being displayed.
 *
 */
class RectangularBoardView : public Gtk::DrawingArea, public ISignaller<IBoardViewSignallable> {
	static constexpr size_t titleFontSizeInPixels = 14;

	static constexpr size_t lowerRowFromBottomInPixels = 20;
	static constexpr size_t lineWidthInPixels          = 10;

	static constexpr size_t distanceXFromBorderInPixels       = 10;
	static constexpr size_t distanceYFromBorderInPixels       = 10;
	static constexpr size_t distanceBetweenRectanglesInPixels = 6;

  public:
	static constexpr size_t spaceForPlayerNameInPixels = 20;

	RectangularBoardView(const BoardContext& _boardCtx, const SizeInPixels& _size,
	                     const PlayerInformation* _playerInfo = nullptr);

	virtual ~RectangularBoardView() = default;

	void connect_to_player(const PlayerInformation* _playerInfo);

	const PlayerInformation* get_player_info() { return playerInfo; }

	void select();
	void unselect();

	void set_selected(const BoardIndex& ind, Color* color);

	void select_fields(const board_ind_c& indices, Color* color);
	void unselect_all_fields();

	auto& operator[](const BoardIndex& ind) { return fields[ind.row][ind.col]; }

	const auto& get_ctx() { return boardCtx; }
	const auto& get_size() { return size; }

	// protected:
	/**
	 * @brief Gtk event that is called for drawing
	 * Draws the WPC according to its state.
	 *
	 * @param cr context to draw with
	 * @return true always
	 * @return false never
	 */
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

	/**
	 * @brief Gtk event that is called when the user clicks inside the WPC
	 * Handles differently according to the type:
	 * - players move
	 * - human player choosing WPC
	 * - constructing a new WPC
	 *
	 * @param event event struct that contains the x,y coordinates of the button
	 * press
	 * @return true always
	 * @return false never
	 */
	virtual bool on_button_press_event(GdkEventButton* event);

  private:
	const BoardContext&      boardCtx;
	SizeInPixels             size;
	const PlayerInformation* playerInfo;

	SizeInPixels rectangleSize;
	bool         isSelected;

	std::vector<std::vector<std::unique_ptr<BoardFieldView>>> fields;

	//  std::vector<IWPCSignallable*> observers;

	void handle_attribute_placing_click(GdkEventButton* event);

	void initialize_rectangles();

	/** Drawing divided into smaller functonalities */
	void draw_base(const Cairo::RefPtr<Cairo::Context>& cr);
	void draw_fields(const Cairo::RefPtr<Cairo::Context>& cr);
	void draw_title(const Cairo::RefPtr<Cairo::Context>& cr);
	void draw_hardness_circles(const Cairo::RefPtr<Cairo::Context>& cr);
	void draw_player_name(const Cairo::RefPtr<Cairo::Context>& cr);

	void signal_field_observers(const BoardIndex& ind);
	void signal_click();
};

#endif // BOARD_VIEW_H
