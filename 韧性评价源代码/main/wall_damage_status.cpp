#include "wall_damage_status.h"


wall_damage_status::wall_damage_status(void)
{
}

double **wall_damage_status::wall_damage(double ****member_realization,double **member_vol,int number_of_floors,int number_of_realization,int number_of_wall_eachfloor)
{
	double **floor_damage_proportion;
	int ***member_status;
	double ***member_damage_proportion; //构件对应6个损伤状态比例
	double R;                                  //1-0随机数
	double theta_y=0;                          //初始化每个破坏状态的中位置和标准值
	double theta_IO=0;
	double theta_p=0;
	double theta_LS=0;
	double theta_u=0;
	double std_dev_y=0;
	double std_dev_IO=0;
	double std_dev_p=0;
	double std_dev_LS=0;
	double std_dev_u=0;
	double overall_volume=0;
	string temp;                     //一个临时变量
	Stat S;
	ofstream output;
	int id=1;  

	floor_damage_proportion = new double *[number_of_floors] ;				//申请矩阵
	for( int i = 0 ; i < number_of_floors ; i ++ )
		 floor_damage_proportion[i] = new double [6];

	for (int flrs=0; flrs<number_of_floors; flrs++)							//初始化矩阵
	{
		for (int i=0; i<6; i++)
		{
			floor_damage_proportion[flrs][i]=0;
		}
	}
	
	member_status = new int **[number_of_floors] ;								//申请扩充EDPs矩阵内存
	for( int i = 0 ; i < number_of_floors; i ++ )
		member_status[i] = new int *[number_of_wall_eachfloor];
	for( int i = 0 ; i < number_of_floors; i ++ )
		for( int j = 0 ; j < number_of_wall_eachfloor ; j ++ )
		member_status[i][j] = new int [number_of_realization] ;


	member_damage_proportion = new double **[number_of_floors];										//申请矩阵内存
	for( int i = 0 ; i < number_of_floors; i ++ )
		member_damage_proportion[i] = new double *[number_of_wall_eachfloor] ;
	for ( int i = 0 ; i < number_of_floors; i ++ )
		for( int j = 0 ; j < number_of_wall_eachfloor ; j ++ )
		member_damage_proportion[i][j] = new double [6] ;

	for (int flrs=0; flrs<number_of_floors;flrs++)													//初始化矩阵											
	{
		for (int member=0;member<(number_of_wall_eachfloor); member++)
		{
			for (int num=0; num<6; num++)
			{
				member_damage_proportion[flrs][member][num]=0;
			}
		}
	}
	overall_volume=0;
	theta_y = 0.0035;
	std_dev_y = 0.36;
	theta_IO = 0.005;
	std_dev_IO=0.3;
	theta_p=0.008;
	std_dev_p=0.27;
	theta_LS=0.01;
	std_dev_LS=0.21;
	theta_u=0.013;
	std_dev_u=0.3;
	for (int num=0; num<number_of_realization; num++)
	{
		R=S.random(0,1);
	for (int flrs=0; flrs<number_of_floors; flrs++)
	{
		for (int member=0; member<number_of_wall_eachfloor; member++)
		{
			
				if(R<S.CDF_normal(log(member_realization[flrs][member][0][num]/theta_y)/std_dev_y,0,1))
				{											
					if(R<S.CDF_normal(log(member_realization[flrs][member][0][num]/theta_IO)/std_dev_IO,0,1))
					{
						if(R<S.CDF_normal(log(member_realization[flrs][member][0][num]/theta_p)/std_dev_p,0,1))
						{
							if(R<S.CDF_normal(log(member_realization[flrs][member][0][num]/theta_LS)/std_dev_LS,0,1))
							{
								if(R<S.CDF_normal(log(member_realization[flrs][member][0][num]/theta_u)/std_dev_u,0,1))
								{
									member_status[flrs][member][num] = 5;	
								}
								else
								{
									member_status[flrs][member][num] = 4;
								}																	
							}
							else
							{
								member_status[flrs][member][num] = 3;	
							}
						}
						else
						{
							member_status[flrs][member][num] = 2;	
						}
					}
					else
					{
						member_status[flrs][member][num] = 1;	
					}
				}
				else
				{
					member_status[flrs][member][num] = 0;	
				}
			}
		}
	}
	
	//calculating the damage status of each member
	for (int flrs=0; flrs<number_of_floors;flrs++)
	{
		for (int member=0;member<number_of_wall_eachfloor; member++)
		{
			for (int num=0; num<6; num++)
			{
				member_damage_proportion[flrs][member][num]=0;
			}

		}
	}
	for (int flrs=0; flrs<number_of_floors;flrs++)
	{
		for (int member=0; member<number_of_wall_eachfloor; member++)
		{
			for (int num=0; num<number_of_realization; num++)
			{
				if(member_status[flrs][member][num]==5)
				{
					member_damage_proportion[flrs][member][5]=member_damage_proportion[flrs][member][5]+1;
				}
				if(member_status[flrs][member][num]==4)
				{
					member_damage_proportion[flrs][member][4]=member_damage_proportion[flrs][member][4]+1;
				}
				if(member_status[flrs][member][num]==3)
				{
					member_damage_proportion[flrs][member][3]=member_damage_proportion[flrs][member][3]+1;
				}
				if(member_status[flrs][member][num]==2)
				{
					member_damage_proportion[flrs][member][2]=member_damage_proportion[flrs][member][2]+1;
				}
				if(member_status[flrs][member][num]==1)
				{
					member_damage_proportion[flrs][member][1]=member_damage_proportion[flrs][member][1]+1;
				}
				if(member_status[flrs][member][num]==0)
				{
					member_damage_proportion[flrs][member][0]=member_damage_proportion[flrs][member][0]+1;
				}
			}
		}
	} 
	for (int flrs=0; flrs<number_of_floors;flrs++)
	{
		for (int member=0; member<number_of_wall_eachfloor; member++)
		{
			for(int i=0; i<6; i++)
			{
				member_damage_proportion[flrs][member][i] = member_damage_proportion[flrs][member][i]/number_of_realization;
			}			
		}
	}

	output.open("result\\structural component damage\\wall_damage_status.csv",ios::out);
	output<<"ID"<<"	"<<"DS1"<<"	"<<"DS2"<<"	"<<"DS3"<<"	"<<"DS4"<<"	"<<"DS5"<<"	"<<"DS6"<<"\n";
	for (int flrs=0; flrs<number_of_floors;flrs++)
	{
		for (int member=0; member<(number_of_wall_eachfloor); member++)
		{output<<id<<"	";
			for(int i=0; i<6; i++)
			{
				output<<member_damage_proportion[flrs][member][i]<<"	";
			}
			output<<"\n";	
			id++;		
		}
	}
	output.close();
	//calculating the damage status of each floor
	for (int flrs=0; flrs<number_of_floors; flrs++)
	{
		for (int i=0; i<6; i++)
		{
			floor_damage_proportion[flrs][i]=0;
		}
	}
	overall_volume=0;
	for (int flrs=0; flrs<number_of_floors; flrs++)
	{
		for (int member=0; member<number_of_wall_eachfloor;member++)
		{
			overall_volume = overall_volume+member_vol[flrs][member];
		}
	}

	for (int flrs=0; flrs<number_of_floors; flrs++)
	{
		for (int i=0; i<6; i++)
		{
			for (int member=0; member<number_of_wall_eachfloor; member++)
			{
				floor_damage_proportion[flrs][i]=floor_damage_proportion[flrs][i]+member_damage_proportion[flrs][member][i]*member_vol[flrs][member]/overall_volume;
			}
		}
	}
	return floor_damage_proportion;
}


wall_damage_status::~wall_damage_status(void)
{
}
