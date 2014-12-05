#include "process_pose_data.h"
#include <sstream>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;
using std::ostringstream;
using std::stringstream;

void position_data_write(float x_coord, float y_coord, int t)
{

  // Definitions for writing data
  int index = 0;
  string output;
  std::ostringstream x_string;
  x_string << x_coord;
  std::ostringstream y_string;
  y_string << y_coord;
  std::ostringstream t_string;
  t_string << t;

  // Preparing database text file.  Each time pose_data_write() is called,
  // new readings will be appended.
  std::ofstream log("pose_database.txt", std::ios_base::app | std::ios_base::out);

  // The execution of writing manipulated data
      // The output variable is a semi-colon separated concatenation of the results. Each function extracts information
      // from the vector pose_data_vec. output is then appended to the end of the pose_database.txt
      // file.
      output = x_string.str() + ";" + y_string.str() + ";" + t_string.str()
      cout << output << endl;
      log <<  output << "\n";
}
