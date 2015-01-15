#include "process_pwm_signals.h"
#include <sstream>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;
using std::ostringstream;
using std::stringstream;

using namespace std;

void pwm_data_write(int t,int speed,int angle)
{

  // Definitions for writing data
  string output;

  // Preparing database text file.  Each time pose_data_write() is called,
  // new readings will be appended.
  ofstream log("pwm_database.txt", ios_base::app | ios_base::out);

  // The execution of writing manipulated data
      // The output variable is a semi-colon separated concatenation of the results. Each function extracts information
      // from the vector pose_data_vec. output is then appended to the end of the pose_database.txt
      // file.
      
      ostringstream speedstr;
      speedstr<<speed;
      
      ostringstream anglestr;
      anglestr<<angle;
      
      ostringstream timestr;
      timestr<<t;
      
      output=speedstr.str()+";"+anglestr.str()+";"+timestr.str();
      
      log <<  output << endl;
      cout << output << endl;
}

