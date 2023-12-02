#include "cata.h"
#include "comp_15/comp15_includeList.h"


int cata_load_voltage = 50;     // can be adjusted - controls power when catapult is loading/priming, adjusting basically changes speed
int cata_launch_voltage = 50;       // can be adjusted - doesn't seem to have much of an effect but this can be tested more
CompetitionCatapult* competition_catapult_instance = CompetitionCatapult::createInstance(Cata, Cata_limit, cata_load_voltage, cata_launch_voltage);
