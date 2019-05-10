#pragma once
#include <algorithm>
#include <math.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include "Stat.h"
#include <fstream>
using namespace std;
class couplingbeam_damage_status
{
public:
	couplingbeam_damage_status(void);
	double **couplingbeam_damage(double ****member_realization,double **member_vol,int number_of_floors,int number_of_realization,int number_of_couplingbeam_eachfloor,int num_couplingbeam_partition);
	~couplingbeam_damage_status(void);
};

