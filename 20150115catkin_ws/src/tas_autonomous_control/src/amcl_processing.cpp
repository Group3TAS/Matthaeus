#include "amcl_processing.h"
#include <cmath>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

int covariance_processing(vector<float> vec)
{
    float cov_x = vec[0];
    float cov_y = vec[7];
    float cov_w = vec[35];

    vector<float> std;

    std.push_back(sqrt(cov_x));
    std.push_back(sqrt(cov_y));
    std.push_back(sqrt(cov_w));
    cout << sqrt(cov_w) << endl;
    cout << sqrt(cov_x) << endl;
    cout << sqrt(cov_y) << endl;


    for (vector<float>::size_type i = 0 ; i < 3 ; ++i)
    {
        float temp = std[i];
        if (temp > .05)
            return 0;
    }
    return 1;
}
