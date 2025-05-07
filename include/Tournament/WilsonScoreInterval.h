#ifndef WILSON_SCORE_INTERVAL_H
#define WILSON_SCORE_INTERVAL_H

/**
 * @brief Data representing the Wilson score interval
 */
struct WilsonScoreInterval {
	double confidence;
	double z_value;
	double interval_low;
	double interval_high;
};

#endif // WILSON_SCORE_INTERVAL_H