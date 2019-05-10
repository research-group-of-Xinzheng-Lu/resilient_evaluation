#pragma once
class couplingbeam
{
public:
	couplingbeam(void);
	double **couplingbeam_damage(double ****member_realization,double **member_vol,double ***member_mp,double **column_axial,double **member_height,double **member_EI,int number_of_floors,int number_of_realization,int number_of_column_eachfloor,int num_column_partition);
	~couplingbeam(void);
};

