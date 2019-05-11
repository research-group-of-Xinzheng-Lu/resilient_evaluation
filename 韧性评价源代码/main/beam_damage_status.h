#pragma once
#include <algorithm>
#include <math.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include "Stat.h"
#include <iostream>
#include <fstream>
using namespace std;
class beam_damage_status
{
public:
	beam_damage_status(void);
	double **beam_damage(double ****member_realization,double **member_vol,double ***member_mp,double **member_height,double **member_EI,int number_of_floors,int number_of_realization,int number_of_beam_eachfloor,int num_beam_partition, int condition);
	~beam_damage_status(void);
private:
     
};

