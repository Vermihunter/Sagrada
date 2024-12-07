#ifndef LOCAL_GAME_PLAYING_PAGE_h
#define LOCAL_GAME_PLAYING_PAGE_h

#include "BlockedUserInterraction.h"
#include "GamePlayingPage.h"
#include "GamePlayingPageState.h"
#include "IBoardViewSignallable.h"
#include "IDieClickSignallable.h"
#include "IRoundTrackClickSignallable.h"
#include "IToolCardViewSignalProcessor.h"
#include "LocalGamePlayingPageContext.h"
#include "LocalPlayerController.h"

class LocalGamePlayingPage : public GamePlayingPage,
                             public IRoundTrackClickSignallable,
                             public IDieClickSignallable,
                             public IBoardViewSignallable,
                             public IToolCardViewSignalProcessor
{
  public:
    LocalGamePlayingPage(const ViewContext& ctx,
                         local_player_controller_t _controller,
                         std::unique_ptr<CommonGameElements> commonGameElements,
                         game_t _game);

    virtual void receive_round_track_die_click_signal(Die* die) override;
    virtual void receive_die_click_signal(Die* die) override;
    virtual void receive_wpc_click_signal(RectangularBoardView*) override;
    virtual void receive_board_field_click_signal(
        RectangularBoardView* board, const BoardIndex& ind) override;
    virtual void tool_card_clicked(ToolCardView* clickedTcView,
                                   bool forceSelect = false) override;
    virtual bool send_move_request(move_t m,
                                   const MoveResultHandlingPolicy&) override;
    virtual void show_error_message(const std::string& msg) override;
    virtual void highlight_basic_moves_for_die(
        Die* die, ID_t playerID,
        dtfm_mode_mask_t mode = DTFM_MODE_DEFAULT) override;
    virtual void highlight_fields(const board_ind_c& indices,
                                  ID_t playerID) override;
    virtual void redraw_curr_rounds_dice() override;
    virtual void block_features(
        const BlockedUserInterraction& newState) override;
    virtual void restore_default_blocking_state() override;
    virtual void signal_new_round_start() override;
    virtual void signal_game_end() override;

  private:
    LocalGamePlayingPageContext pageCtx;
    GamePlayingPageState currState;

    local_player_controller_t controller;
    game_t game;

    sigc::connection timeoutHandler;

    void pass_move_button_clicked();
    void quit_move_button_clicked();
    bool on_timeout();
    void set_timeout();
    void deselect_selected_fields();
    RectangularBoardView& board_by_player_id(ID_t playerID);

    constexpr static const BlockedUserInterraction defaultState {
        .blockedTcClick = false,
        .blockedRoundTrackClick = false,
        .blockedDieClick = false,
        .blockedBoardClick = false,
        .blockedPassMove = false};

    constexpr static const MoveResultHandlingPolicy passMoveResultHandlingInfo {
        .deselectOnFailure = true,
        .deselectOnSuccess = true};

    constexpr static const MoveResultHandlingPolicy dtfmResultHandlingInfo {
        .deselectOnFailure = false,
        .deselectOnSuccess = true};
};

#endif // LOCAL_GAME_PLAYING_PAGE_h