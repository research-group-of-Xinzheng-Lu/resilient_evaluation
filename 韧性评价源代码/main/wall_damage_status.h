#pragma once
#include <algorithm>
#include <math.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include "Stat.h"
#include <iostream>
#include <fstream>
using namespace std;class wall_damage_status
{
public:
	wall_damage_status(void);
	double **wall_damage(double ****member_realization,double **member_vol,int number_of_floors,int number_of_realization,int number_of_couplingbeam_eachfloor);
	~wall_damage_status(void);
};

