#include "PlayerOrderDisplayer.h"

#include "GameInfo.h"

PlayerOrderDisplayer::PlayerOrderDisplayer(const GameState& _gameState,
                                           const player_c& _players,
                                           const SizeInPixels _size)
    : gameState(_gameState)
    , players(_players)
    , size(_size)
    , defaultNameColor(black)
    , playerOnMoveNameColor(red)
{
    set_size_request(size.width, size.height);
}

bool PlayerOrderDisplayer::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    std::string firstMoveNames;
    std::string secondMoveNames;

    // First calculate the size that will be required for the player names
    size_t ind = 0;
    for (auto&& turnInfo : gameState.currRoundInfo->turnInfos) {
        const auto& player = GameInfo::get_player_with_id_from_players(
            players, turnInfo.playerId);
        std::string* moveName =
            (ind < (gameState.currRoundInfo->turnInfos.size() / 2))
                ? &firstMoveNames
                : &secondMoveNames;
        moveName->append(player->name + " - ");

        ++ind;
    }

    firstMoveNames.pop_back();
    firstMoveNames.pop_back();
    firstMoveNames.pop_back();

    secondMoveNames.pop_back();
    secondMoveNames.pop_back();
    secondMoveNames.pop_back();

    cr->select_font_face("Georgia", Cairo::FontSlant::FONT_SLANT_NORMAL,
                         Cairo::FontWeight::FONT_WEIGHT_BOLD);
    cr->set_font_size(25);

    Cairo::TextExtents teFirstNames;
    cr->get_text_extents(firstMoveNames, teFirstNames);
    cr->move_to((size.width / 2) - (150 / 2) - 10 - teFirstNames.x_bearing -
                    teFirstNames.width,
                size.height / 2 - teFirstNames.y_bearing - teFirstNames.height);

    ind = 0; // size_t
    bool firstName = true;
    // After that, show the player names separated with '-' and highlighted the
    // player that is on move
    for (auto&& turnInfo : gameState.currRoundInfo->turnInfos) {
        const auto& player = GameInfo::get_player_with_id_from_players(
            players, turnInfo.playerId);

        if (!firstName) {
            cr->set_source_rgb(defaultNameColor.r, defaultNameColor.g,
                               defaultNameColor.b);
            cr->show_text(" - ");
        }
        const Color& nameColor = (ind == gameState.currPlayerIndex)
                                     ? playerOnMoveNameColor
                                     : defaultNameColor;
        cr->set_source_rgb(nameColor.r, nameColor.g, nameColor.b);

        firstName = false;
        cr->show_text(player->name);

        if (ind == players.size() - 1) { //
            // cr->move_to( size.width / 2 + (size.width / 4) + (150 / 2)  -
            // teFirstNames.x_bearing
            // - teFirstNames.width / 2, size.height / 2 -
            // teFirstNames.y_bearing - teFirstNames.height);
            cr->move_to(size.width / 2 + (150 / 2) + 10,
                        size.height / 2 - teFirstNames.y_bearing -
                            teFirstNames.height);
            firstName = true;
        }
        ++ind;
    }

    return true;
}
