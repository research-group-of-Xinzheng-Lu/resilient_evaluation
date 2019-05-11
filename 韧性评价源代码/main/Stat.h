#ifndef STAT_H
#define STAT_H

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <numeric>

class Stat
{
public:
    Stat();

    void SetSeed(int seed);
    double random(double min=0.0, double max=1.0);
    int round(double r);    //round to an integer

    template <typename T> T getMedian(std::vector<T> v)
    {
        int size=v.size();
        sort(v.begin(),v.end());
        int middle=size/2;
        if(size%2!=0||middle==0)
        {
            return v[middle];
        }

        return (T)((v[middle-1]+v[middle])/2);
    }
    template <typename T> T getPercentile(std::vector<T> v,double percentile)   // 0<=percentile<=100
    {
        int size=v.size();
        sort(v.begin(),v.end());
        int index=ceil((double)size*percentile/100.0)-1;
        if (index<0) index=0;
        return v[index];
    }

    template <typename T> T getMean(std::vector<T> v)
    {
        if(v.size()==0) return (T)0;
        T sum = accumulate(v.begin(), v.end(), 0.0);
        return sum / v.size();
    }

    template <typename T> T getStd(std::vector<T> v)
    {
        if(v.size()<=1) return (T)0;

        T sum = accumulate(v.begin(), v.end(), 0.0);
        T mean =  sum / v.size();
        T accum  = 0.0;
        std::for_each (v.begin(), v.end(), [&](const T d) {
            accum  += (d-mean)*(d-mean);
        });
        T stdev = sqrt(accum/(v.size()-1));
        return stdev;
    }

    //Cumulative Distribution Function of Normal Distrubution. F(0)=0.5; F(4)=1.0
    double CDF_normal(double x,double median,double std);

    //Generate random numbers obeying normal distribution
    double gaussrand(double median, double std);




private:
    int _seed;
};

#endif // STAT_H