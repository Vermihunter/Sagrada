#ifndef TOOL_CARD_VIEW_H
#define TOOL_CARD_VIEW_H

#include "BlockedUserInterraction.h"
#include "Deallocator.h"
#include "Game.h"
#include "IBoardViewSignallable.h"
#include "IDieClickSignallable.h"
#include "IRoundTrackClickSignallable.h"
#include "ISignaller.h"
#include "IToolCardViewSignalProcessor.h"
#include "Image.h"
#include "ToolCard.h"
#include "ToolCardPathsByType.h"
#include "ToolCardViewContainer.h"
#include "Typedefs.h"

#include <cmath>
#include <gtkmm.h>
#include <memory>

class ToolCardView : public Gtk::Fixed,
                     public IDieClickSignallable,
                     public IBoardViewSignallable,
                     public IRoundTrackClickSignallable {
	static constexpr size_t lineWidth = 3;

  public:
	void set_signal_receiver(IToolCardViewSignalProcessor* _signalProcessor);

	/**
	 * @brief Set the selected object
	 * Marked virtual because if the tool card consists of a single move,
	 * it won't be selected, but used instantly.
	 *
	 * @param _isSelected
	 */
	virtual bool set_selected(bool _isSelected);

	// Some tool cards require to draw additional things for use - this is done
	// by overriding these functions that draw to the common Gtk::DrawingArea
	virtual void on_common_help_visualizer_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	virtual void on_common_help_visualizer_click(GdkEventButton* event);

	// Not forcing each tool card to override, but give the opportunity
	virtual void receive_die_click_signal(Die*) override;
	virtual void receive_round_track_die_click_signal(Die*) override;
	virtual void reset();

	// Factory method
	static tc_view_t create(ToolCardViewContainer& parentContainer, Game& game, ToolCard& tc,
	                        const SizeInPixels& size, size_t ind);

  protected:
	ToolCardView(ToolCardViewContainer& parentContainer, Game& g, ToolCard& tc,
	             const SizeInPixels& size, size_t _ind);

	virtual bool on_tc_image_click(GdkEventButton* button_event);
	bool         on_cost_visualizer_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	void         initialize_cost_visualizer();
	bool         on_selection_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	move_c       collect_possible_moves(ToolCard& tc, tool_card_move_t prevMove = nullptr);

	ToolCardViewContainer&            parentContainer;
	Game&                             game;
	ToolCard&                         tc;
	const size_t                      ind;
	SizeInPixels                      selectionSize;
	SizeInPixels                      costSize;
	Gtk::DrawingArea                  da_SelectionArea;
	std::unique_ptr<Gtk::DrawingArea> da_CostVisualizer;
	Gtk::EventBox                     eb_Image;
	Image                             image;
	bool                              isSelectable;
	bool                              isSelected;
	IToolCardViewSignalProcessor*     signalProcessor;
};

#endif // TOOL_CARD_VIEW_H