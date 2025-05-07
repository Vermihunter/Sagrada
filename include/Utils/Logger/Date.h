
////////////////////////////////////////
///         Vermes �kos � 2022       ///
///                                  ///
///               Date.h             ///
////////////////////////////////////////

#ifndef DATE_H
#define DATE_H

#define _CRT_SECURE_NO_WARNINGS

#include <chrono>
#include <iostream>

/**
 * @brief Class with static methods to generate current date and time.
 */
class Date {
  public:
	/**
	 * @brief Generates current local time
	 *
	 * The string format is "hh_mm_ss". In cases when
	 * a part has value less than 10, a 0 is prepended.
	 */
	static const std::string get_curr_time();

	/**
	 * @brief Generates
	 */
	static const std::string& get_curr_date();

  private:
	/** Cache for date - each day it has to be computed only once - may be
	 * deleted */
	static std::string dateCache;
};

#endif // !DATE_H
