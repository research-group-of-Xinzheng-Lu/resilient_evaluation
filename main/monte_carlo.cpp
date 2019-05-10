#include "monte_carlo.h"
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
#include <iostream>
#include <fstream>
#include <random>
using namespace Eigen; 
using namespace std;

monte_carlo::monte_carlo(void)
{
}


double **monte_carlo::monte_carlo_sim(double **EDPs, int GMs, int number_of_edps, int number_of_realization,string filename)
{
double **realization = new double *[number_of_edps];
for (int i=0; i<number_of_edps; i++)
	realization[i]= new double [number_of_realization];

int lnEDPs_rank;
int lnEDPs_cov_rank;
ofstream output;
double max_mean_error=0;
double average_mean_error=0;
double max_cov_error=0;
double average_cov_error=0;
stringstream file;
string ss1,ss2;

MatrixXd lnEDPs(GMs,(number_of_edps));

for (int i=0; i<GMs;i++)
{
	for (int num=0; num<number_of_edps; num++)
	{
		lnEDPs(i,num) = log(EDPs[i][num]);
	}
}
RowVectorXd lnEDPs_mean = lnEDPs.colwise().mean();

FullPivLU<MatrixXd> lnEDPs_lu_decomp(lnEDPs);
lnEDPs_rank= lnEDPs_lu_decomp.rank();

MatrixXd centered = lnEDPs.rowwise() - lnEDPs_mean;
MatrixXd lnEDPs_cov = (centered.adjoint() * centered) / double(lnEDPs.rows() - 1);



FullPivLU<MatrixXd> lnEDPs_cov_lu_decomp(lnEDPs_cov);	
lnEDPs_cov_rank = lnEDPs_cov_lu_decomp.rank();

MatrixXd U(lnEDPs_cov_rank,number_of_realization);


default_random_engine generator;
normal_distribution<double> randn(0,1);

if (lnEDPs_cov_rank >= number_of_edps)
{
	for (int i=0; i<number_of_edps;i++)
	{
		for (int j=0; j<number_of_realization; j++)
		{
			U(i,j)=randn(generator);
		}
	}
}
else
{
	for (int i=0; i<lnEDPs_cov_rank;i++)
	{
		for (int j=0; j<number_of_realization; j++)
		{
			U(i,j)=randn(generator);
		}
	}
}


lnEDPs_cov=lnEDPs_cov+0.000001*MatrixXd::Identity(number_of_edps,number_of_edps);			
LLT<MatrixXd> Lof_lnEDPs_cov(lnEDPs_cov);
MatrixXd BB= Lof_lnEDPs_cov.matrixL().adjoint();

MatrixXd L(number_of_edps,lnEDPs_cov_rank);
if (lnEDPs_cov_rank<number_of_edps)
L=BB.topRows(lnEDPs_cov_rank).transpose();
else
L=BB.transpose();



MatrixXd Z = L * U + lnEDPs_mean.transpose() * MatrixXd::Ones(1,number_of_realization);


RowVectorXd lnEDPs_sim_mean = Z.transpose().colwise().mean();
MatrixXd centered_Z = Z.transpose().rowwise() - lnEDPs_sim_mean;
MatrixXd lnEDPs_sim_cov = (centered_Z.adjoint() * centered_Z) / double(Z.transpose().rows() - 1);

MatrixXd mean_error = lnEDPs_sim_mean.array()/lnEDPs_mean.array();
MatrixXd cov_error = lnEDPs_sim_cov.array()/lnEDPs_cov.array();



for (int i=0; i<mean_error.cols();i++)
{
	if(max_mean_error<abs(mean_error(0,i)-1))
	{
		max_mean_error = abs(mean_error(0,i)-1);
	}
	average_mean_error =average_mean_error+abs(mean_error(0,i)-1);
}
average_mean_error = average_mean_error/mean_error.cols();

file.clear();
file.str("");
file<<"result\\error\\mean_error_of_"<<filename<<".csv";
ss1=file.str();

output.open(ss1,ios::out);
for (int i=0; i<mean_error.cols();i++)
{
	output<<i+1<<",";
}
output<<"\n";
for (int i=0; i<mean_error.cols();i++)
{
	output<<mean_error(i)<<",";
}
output.close();



for (int i=0; i<cov_error.rows();i++)
{
	for (int j=0; j<cov_error.cols(); j++)
	{
		if(max_cov_error<abs(cov_error(i,j)-1))
		{
			max_cov_error = abs(cov_error(i,j)-1);
		}	
	average_cov_error = average_cov_error+abs(cov_error(i,j)-1);	
	}
}
average_cov_error = average_cov_error/(cov_error.cols()*cov_error.rows());

file.clear();
file.str("");
file<<"result\\error\\cov_error_of_"<<filename<<".csv";
ss2=file.str();
output.open(ss2,ios::out);
for (int i=0; i<cov_error.cols();i++)
{
	output<<","<<i+1;
}
output<<"\n";
for (int i=0; i<cov_error.rows();i++)
{output<<i+1<<",";
	for (int j=0;j<cov_error.cols();j++)
	{
		output<<cov_error(i,j)<<",";
	}
	output<<"\n";
}
output.close();

cout<<"Maximum Value of Mean error: "<<max_mean_error*100<<"%"<<"\n";
cout<<"Average Value of Mean error: "<<average_mean_error*100<<"%"<<"\n";
cout<<"Maximum Value of Covariance error: "<<max_cov_error*100<<"%"<<"\n";
cout<<"Average Value of Covariance error: "<<average_cov_error*100<<"%"<<"\n";

MatrixXd W(Z.rows(),Z.cols());
for (int i=0; i<Z.rows(); i++)
{
	for (int j=0; j<Z.cols(); j++)
	{
		W(i,j) = exp(Z(i,j));
	}
}

for (int i=0; i<Z.rows(); i++)
{
	for (int j=0; j<Z.cols(); j++)
	{
		realization[i][j] = W(i,j);
	}
}

return realization;
}

monte_carlo::~monte_carlo(void)
{
}
