////////////////////////////////////////
///         Vermes �kos � 2022       ///
///                                  ///
///              Date.cpp            ///
////////////////////////////////////////

#include "Date.h"

std::string Date::dateCache {};

/**
 * @brief Generates current local date and returns it in given format.
 * The defined format is yyyy_m_d, it is generated with c++20 chrono library.
 *
 * @return const s The formated date
 */
const std::string& Date::get_curr_date()
{
    // C++ ain't no ready for me!!
    // auto now{
    // std::chrono::current_zone()->to_local(std::chrono::system_clock::now())
    // }; const std::chrono::year_month_day ymd{
    // std::chrono::floor<std::chrono::days>(now) };

    if (!dateCache.empty())
        return dateCache;

    const auto today =
        std::chrono::sys_days {std::chrono::floor<std::chrono::days>(
            std::chrono::system_clock::now())};

    const std::chrono::year_month_day ymd {today};

    dateCache = std::to_string(static_cast<int>(ymd.year())) + "_" +
                std::to_string(static_cast<unsigned>(ymd.month())) + "_" +
                std::to_string(static_cast<unsigned>(ymd.day()));

    return dateCache;
}

/**
 * @brief Generates current local time and returns it in given format.
 * The defined format is hh_mm_ss, it is generated with c++20 chrono library.
 *
 * @return const s The formated local time
 */
const std::string Date::get_curr_time()
{
    // C++ ain't no ready for me!!
    // auto now{
    // std::chrono::current_zone()->to_local(std::chrono::system_clock::now())
    // };

    time_t curr_time;
    curr_time = time(NULL);

    tm* tm_local = localtime(&curr_time);
    const int hours = tm_local->tm_hour;
    const int minutes = tm_local->tm_min;
    const int seconds = tm_local->tm_sec;

    return ((hours < 10) ? "0" : "") + std::to_string(hours) + "_" +
           ((minutes < 10) ? "0" : "") + std::to_string(minutes) + "_" +
           ((seconds < 10) ? "0" : "") + std::to_string(seconds);

    /*
    std::chrono::zoned_time now{ std::chrono::current_zone(),
                                                             std::chrono::system_clock::now()
    };

    auto now_local = now.get_local_time();

    auto today = std::chrono::floor<std::chrono::days>( now_local);
    const std::chrono::hh_mm_ss runTime{ std::chrono::duration<int,
    std::ratio<1,1> > (std::chrono::duration_cast<std::chrono::seconds>(
    now_local - today)) };

    const int hours = runTime.hours().count();
    const int minutes = runTime.minutes().count();
    const int seconds = runTime.seconds().count();

    return  hours < 10 ? "0" : "" + std::to_string(hours) + "_"
            + ((minutes < 10) ? "0" : "") + std::to_string(minutes) + "_"
            + ((seconds < 10) ? "0" : "") + std::to_string(seconds);
    */

    // C++ ain't no ready for me!!
    // return std::format("{:%H_%M_%S}", runTime);
}
