#pragma once
#include <algorithm>
#include <math.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <Eigen/Cholesky>
#include <Eigen/Core>
#include <Eigen/Eigenvalues>
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <fstream>


using namespace std;
class monte_carlo
{
public:
	monte_carlo(void);
	double **monte_carlo_sim(double **EDPs, int GMs, int number_of_edps, int number_of_realization, string filename);
	~monte_carlo(void);
};

