#pragma once
#include <algorithm>
#include <math.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include "Stat.h"
#include <fstream>
using namespace std;
class column_damage_status
{
public:
	column_damage_status(void);
	double **column_damage(double ****member_realization,int level_of_frame,double **member_vol,double ***member_mp,double **column_axial,double **member_height,double **member_EI,int number_of_floors,int number_of_realization,int number_of_column_eachfloor,int num_column_partition);
	~column_damage_status(void);
private:
     
};

