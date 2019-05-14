#include "beam_damage_status.h"


beam_damage_status::beam_damage_status(void)
{
}

double **beam_damage_status::beam_damage(double ****member_realization,double **member_vol,double ***member_mp,double **member_height,double **member_EI,int number_of_floors,int number_of_realization,int number_of_beam_eachfloor, int num_beam_partition, int condition)
{
	double **floor_damage_proportion;
	int ****member_status;
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
	double theta_y_default_value=0.004;
	ofstream output;
	string temp;                     //一个临时变量
	Stat S;
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


	member_status = new int ***[number_of_floors] ;								//申请扩充EDPs矩阵内存
	for( int i = 0 ; i < number_of_floors; i ++ )
		member_status[i] = new int **[number_of_beam_eachfloor];
	for( int i = 0 ; i < number_of_floors; i ++ )
		for( int j = 0 ; j < number_of_beam_eachfloor ; j ++ )
		member_status[i][j] = new int *[num_beam_partition] ;
	for( int i = 0 ; i < number_of_floors; i ++ )
		for( int j = 0 ; j < number_of_beam_eachfloor ; j ++ )
			for( int k = 0 ; k < num_beam_partition ; k ++ )
				member_status[i][j][k] = new int [number_of_realization] ;	



	member_damage_proportion = new double **[number_of_floors];										//申请矩阵内存
	for( int i = 0 ; i < number_of_floors; i ++ )
		member_damage_proportion[i] = new double *[number_of_beam_eachfloor] ;
	for ( int i = 0 ; i < number_of_floors; i ++ )
		for( int j = 0 ; j < number_of_beam_eachfloor ; j ++ )
		member_damage_proportion[i][j] = new double [6] ;

	for (int flrs=0; flrs<number_of_floors;flrs++)													//初始化矩阵											
	{
		for (int member=0;member<(number_of_beam_eachfloor); member++)
		{
			for (int num=0; num<6; num++)
			{
				member_damage_proportion[flrs][member][num]=0;
			}
		}
	}


///////////////////////////初始化/////////////////////////////

	overall_volume=0;
	for (int num=0; num<number_of_realization; num++)
	{
	R=S.random(0,1);

	for (int flrs=0; flrs<number_of_floors; flrs++)
	{
		for (int member=0; member<(number_of_beam_eachfloor); member++)
		{
			for (int ele=0; ele<num_beam_partition;ele++)
			{
				if (condition==1)
				theta_y=0.004;
				else
				theta_y = 3*member_mp[flrs][member][ele]*member_height[flrs][member]/member_EI[flrs][member];

				std_dev_y = 0.4;
				theta_p=0.01;
				std_dev_p=0.4;
				theta_IO = 0.5*(theta_y+theta_p);
				std_dev_IO=0.4;
				theta_LS=0.02;
				std_dev_LS=0.4;
				theta_u=0.03;
				std_dev_u=0.4;
			

					if(R<S.CDF_normal(log(member_realization[flrs][member][ele][num]/theta_y)/std_dev_y,0,1))
					{
											
						if(R<S.CDF_normal(log(member_realization[flrs][member][ele][num]/theta_IO)/std_dev_IO,0,1))
						{
							if(R<S.CDF_normal(log(member_realization[flrs][member][ele][num]/theta_p)/std_dev_p,0,1))
							{
								if(R<S.CDF_normal(log(member_realization[flrs][member][ele][num]/theta_LS)/std_dev_LS,0,1))
								{
									if(R<S.CDF_normal(log(member_realization[flrs][member][ele][num]/theta_u)/std_dev_u,0,1))
									{
										member_status[flrs][member][ele][num] = 5;	
									}
									else
									{
										member_status[flrs][member][ele][num] = 4;
									}																	
								}
								else
								{
									member_status[flrs][member][ele][num] = 3;	
								}
							}
							else
							{
								member_status[flrs][member][ele][num] = 2;	
							}
						}
						else
						{
							member_status[flrs][member][ele][num] = 1;	
						}
					}
					else
					{
						member_status[flrs][member][ele][num] = 0;	
					}
				}
			}
		}
	}
	
	for (int flrs=0; flrs<number_of_floors;flrs++)
	{
		for (int member = 0; member<(number_of_beam_eachfloor); member++)
		{
			for (int num=0; num<number_of_realization; num++)
			{
				for (int ele=0; ele<num_beam_partition-1; ele++)
				{
					if (member_status[flrs][member][0][num]<member_status[flrs][member][ele+1][num])
					{
						member_status[flrs][member][0][num]=member_status[flrs][member][ele+1][num];
					}

				}

			}

		}

	}
	//计算构件每个状态的损伤比例
	for (int num=0; num<number_of_realization; num++)
	{
	for (int flrs=0; flrs<number_of_floors;flrs++)
	{
		for (int member=0; member<(number_of_beam_eachfloor); member++)
		{
			
				if(member_status[flrs][member][0][num]==5)
				{
					member_damage_proportion[flrs][member][5]=member_damage_proportion[flrs][member][5]+1;
				}
				if(member_status[flrs][member][0][num]==4)
				{
					member_damage_proportion[flrs][member][4]=member_damage_proportion[flrs][member][4]+1;
				}
				if(member_status[flrs][member][0][num]==3)
				{
					member_damage_proportion[flrs][member][3]=member_damage_proportion[flrs][member][3]+1;
				}
				if(member_status[flrs][member][0][num]==2)
				{
					member_damage_proportion[flrs][member][2]=member_damage_proportion[flrs][member][2]+1;
				}
				if(member_status[flrs][member][0][num]==1)
				{
					member_damage_proportion[flrs][member][1]=member_damage_proportion[flrs][member][1]+1;
				}
				if(member_status[flrs][member][0][num]==0)
				{
					member_damage_proportion[flrs][member][0]=member_damage_proportion[flrs][member][0]+1;
				}
			}
		}
	} 

	for (int flrs=0; flrs<number_of_floors;flrs++)
	{
		for (int member=0; member<(number_of_beam_eachfloor); member++)
		{
			for(int i=0; i<6; i++)
			{
				member_damage_proportion[flrs][member][i] = member_damage_proportion[flrs][member][i]/number_of_realization;
			}			
		}
	}
	
	output.open("result\\structural component damage\\beam_damage_status.txt",ios::out);
	output<<"ID"<<"	"<<"DS1"<<"	"<<"DS2"<<"	"<<"DS3"<<"	"<<"DS4"<<"	"<<"DS5"<<"	"<<"DS6"<<"\n";
	for (int flrs=0; flrs<number_of_floors;flrs++)
	{
		for (int member=0; member<(number_of_beam_eachfloor); member++)
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

	//计算各个楼层的损伤状态比例

	for (int flrs=0; flrs<number_of_floors; flrs++)
	{
		for (int member=0;member<(number_of_beam_eachfloor); member++)
		{
			overall_volume = overall_volume+member_vol[flrs][member];
		}		
	}

	for (int flrs=0; flrs<number_of_floors; flrs++)
	{
		for (int i=0; i<6; i++)
		{
			for (int member=0; member<(number_of_beam_eachfloor); member++)
			{
				floor_damage_proportion[flrs][i]=floor_damage_proportion[flrs][i]+member_damage_proportion[flrs][member][i]*member_vol[flrs][member]/overall_volume;
			}
		}
	}
	return floor_damage_proportion;              
}
beam_damage_status::~beam_damage_status(void)
{
}
