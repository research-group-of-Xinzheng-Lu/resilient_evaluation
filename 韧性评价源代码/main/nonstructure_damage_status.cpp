#include "nonstructure_damage_status.h"


nonstructure_damage_status::nonstructure_damage_status(void)
{
}

double ***nonstructure_damage_status::nonstructural_damage(double ****member_realization,int number_of_floors,int number_of_realization, int number_of_component_type, double ***component_properties)
{
	double **floor_damage_proportion;
	int ***component_status;
	double ***component_damage_proportion; //构件对应6个损伤状态比例
	double R;                                  //1-0随机数
	double theta_1=0;
	double theta_2=0;
	double theta_3=0;
	double std_dev_1=0;
	double std_dev_2=0;
	double std_dev_3=0;
	string temp;                     //一个临时变量
	Stat S;
	ofstream output;
	
	component_status = new int **[number_of_component_type] ;								//申请扩充EDPs矩阵内存
	for( int i = 0 ; i < number_of_component_type; i ++ )
		component_status[i] = new int *[number_of_floors];
	for( int i = 0 ; i < number_of_component_type; i ++ )
		for( int j = 0 ; j < number_of_floors ; j ++ )
		component_status[i][j] = new int [number_of_realization] ;

	component_damage_proportion = new double **[number_of_component_type];										//申请矩阵内存
	for( int i = 0 ; i < number_of_component_type; i ++ )
		component_damage_proportion[i] = new double *[number_of_floors] ;
	for ( int i = 0 ; i < number_of_component_type; i ++ )
		for( int j = 0 ; j < number_of_floors ; j ++ )
		component_damage_proportion[i][j] = new double [4] ;

	for (int flrs=0; flrs<number_of_component_type;flrs++)													//初始化矩阵											
	{
		for (int member=0;member<number_of_floors; member++)
		{
			for (int num=0; num<4; num++)
			{
				component_damage_proportion[flrs][member][num]=0;
			}
		}
	}

		for (int type=0; type<number_of_component_type; type++)
	{
		theta_1=component_properties[type][0][0];
		std_dev_1=component_properties[type][1][0];
		theta_2=component_properties[type][0][1];
		std_dev_2=component_properties[type][1][1];
		theta_3=component_properties[type][0][2];
		std_dev_3=component_properties[type][1][2];
		for (int num=0; num<number_of_realization; num++)
		{
			R=S.random(0,1);
		for (int flrs=0; flrs<number_of_floors; flrs++)
		{
			
				if(R<S.CDF_normal(log(member_realization[flrs][0][0][num]/theta_1)/std_dev_1,0,1))
				{	
					if (theta_2!=0)
					{								
						if(R<S.CDF_normal(log(member_realization[flrs][0][0][num]/theta_2)/std_dev_2,0,1))
						{
							if (theta_3!=0)
							{
								if(R<S.CDF_normal(log(member_realization[flrs][0][0][num]/theta_3)/std_dev_3,0,1))
								{
									component_status[type][flrs][num] = 3;	
								}
								else
								{
									component_status[type][flrs][num] = 2;
								}
							}
							else 
							{
								component_status[type][flrs][num] = 2;
							}
						}
						else
						{
							component_status[type][flrs][num] = 1;	
						}
					}
					else 
					{
						component_status[type][flrs][num] = 1;
					}
				}
				else
				{
					component_status[type][flrs][num] = 0;	
				}
			}
		}
	}

	//calculating the damage status of each member
	for (int type=0; type<number_of_component_type; type++)
	{
		for (int flrs=0; flrs<number_of_floors;flrs++)
		{
			for (int i=0; i<4; i++)
			{
				component_damage_proportion[type][flrs][i]=0;
			}
		}
	}

	for (int type=0; type<number_of_component_type;type++)
	{
		for (int flrs=0; flrs<number_of_floors; flrs++)
		{
			for (int num=0; num<number_of_realization; num++)
			{
				if(component_status[type][flrs][num]==3)
				{
					component_damage_proportion[type][flrs][3]=component_damage_proportion[type][flrs][3]+1;
				}
				if(component_status[type][flrs][num]==2)
				{
					component_damage_proportion[type][flrs][2]=component_damage_proportion[type][flrs][2]+1;
				}
				if(component_status[type][flrs][num]==1)
				{
					component_damage_proportion[type][flrs][1]=component_damage_proportion[type][flrs][1]+1;
				}
				if(component_status[type][flrs][num]==0)
				{
					component_damage_proportion[type][flrs][0]=component_damage_proportion[type][flrs][0]+1;
				}
			}
		}
	} 

	for (int type=0; type<number_of_component_type;type++)
	{
		for (int flrs=0; flrs<number_of_floors; flrs++)
		{
			for(int i=0; i<4; i++)
			{
				component_damage_proportion[type][flrs][i] = component_damage_proportion[type][flrs][i]/number_of_realization;
			}			
		}
	}


return component_damage_proportion;
}



nonstructure_damage_status::~nonstructure_damage_status(void)
{
}
