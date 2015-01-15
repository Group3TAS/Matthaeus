#include "laser_processing.h"
#include <vector>

using std::vector;

float min_right(vector<float>& vec)
{
    float minright=99999999999;




    vector<float>::size_type i;
    for (i = 0; i != 100 ; i++)
    {
        if (vec[i] < minright)
            minright=vec[i];
    }
    return minright;

}

float min_left(vector<float>& vec)
{
    float minleft=999999999999;
    vector<float>::size_type i;
    for (i = 619; i != vec.size(); i++)
    {
        if (vec[i] < minleft)
            minleft=vec[i];
    }
    return minleft;
}


float process_laser_data(vector<float> laser_vec)
{
  vector<float>::size_type x,i;
  x = laser_vec.size();

  float min_l = min_left(laser_vec);
  float min_r = min_right(laser_vec);

  for (i = 0; i!=x; i++)
    {
      if (laser_vec[i] < 0.4)
	{
	  return 0;
     }
    }
  if(min_l<min_r && min_r < 2)
  {
        //fahr nach rechts
      return 1;
  }
  else if(min_r<min_l && min_l < 2)
  {
      //links
      return 2;
  }else
  {
      return 3;
  }

}
