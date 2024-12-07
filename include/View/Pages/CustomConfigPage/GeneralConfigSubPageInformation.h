#ifndef GENERAL_CONFIG_SUBPAGE_INFORMATION_H
#define GENERAL_CONFIG_SUBPAGE_INFORMATION_H

#include <string>

struct GeneralConfigSubpageInformation
{
    std::string contextName;
    size_t numberOfRounds;
    size_t wpcPerPlayer;
};

#endif // GENERAL_CONFIG_SUBPAGE_INFORMATION_H