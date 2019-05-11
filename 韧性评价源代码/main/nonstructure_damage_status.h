#pragma once
#include <algorithm>
#include <math.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include "Stat.h"
#include <fstream>
using namespace std;
class nonstructure_damage_status
{
public:
	nonstructure_damage_status(void);
	double ***nonstructural_damage(double ****member_realization,int number_of_floors,int number_of_realization, int number_of_component_type, double ***component_properties);
	~nonstructure_damage_status(void);
};

