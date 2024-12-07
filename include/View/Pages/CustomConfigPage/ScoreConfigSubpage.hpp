#ifndef SCORECONFIGSUBPAGE_HPP
#define SCORECONFIGSUBPAGE_HPP

#include "CustomConfigPageSubpage.h"
#include "LabeledWidget.h"
#include "NumberEntry.h"
#include "ScoreContext.h"

class ScoreConfigSubpage : public CustomConfigPageSubpage
{
  public:
    ScoreConfigSubpage(const ViewContext& ctx)
        : CustomConfigPageSubpage(ctx)
        , lEntry_unusedFavorTokenPoints("Points for each unused favor token")
        , lEntry_minusPointsPerEmptyField("Minus points for each empty field")
        , lEntry_procValuePoints(
              "Points for each die value of Private objective card color")
        , vbox_ScoreSettings(true)
    {
        vbox_ScoreSettings.pack_start(lEntry_unusedFavorTokenPoints);
        vbox_ScoreSettings.pack_start(lEntry_minusPointsPerEmptyField);
        vbox_ScoreSettings.pack_start(lEntry_procValuePoints);

        auto defaultScoreCtx = DefaultGameCreator::get_default_score_ctx();
        lEntry_unusedFavorTokenPoints.widget.set_text(
            std::to_string(defaultScoreCtx->pointsPerUnusedFavorToken));
        lEntry_minusPointsPerEmptyField.widget.set_text(
            std::to_string(defaultScoreCtx->minusPointsPerEmptyField));
        lEntry_procValuePoints.widget.set_text(
            std::to_string(defaultScoreCtx->pointsPerProcValue));

        vbox_ScoreSettings.set_size_request(400, 200);
        put(vbox_ScoreSettings, (ctx.size.width - 400) / 2, 80);
    }

    score_ctx_t get_entered_score_config()
    {
        return std::make_unique<ScoreContext>(
            lEntry_unusedFavorTokenPoints.widget.get_underlying_number(
                "Unused favor token points"),
            lEntry_minusPointsPerEmptyField.widget.get_underlying_number(
                "Minus points for empty fields"),
            lEntry_procValuePoints.widget.get_underlying_number(
                "PROC value points"));
    }

  private:
    LabeledWidget<NumberEntry> lEntry_unusedFavorTokenPoints;
    LabeledWidget<NumberEntry> lEntry_minusPointsPerEmptyField;
    LabeledWidget<NumberEntry> lEntry_procValuePoints;

    Gtk::VBox vbox_ScoreSettings;
};

#endif // SCORECONFIGSUBPAGE_HPP