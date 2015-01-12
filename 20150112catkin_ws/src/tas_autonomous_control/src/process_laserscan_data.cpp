#include "process_laserscan_data.h"
#include <sstream>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;
using std::ostringstream;
using std::stringstream;

using namespace std;

void laserscan_data_write(vector<float> data, int length)
{

  // Definitions for writing data
  int i = 0;
  string output;

  // Preparing database text file.  Each time pose_data_write() is called,
  // new readings will be appended.
  ofstream log("scan_database.txt", ios_base::app | ios_base::out);

  // The execution of writing manipulated data
      // The output variable is a semi-colon separated concatenation of the results. Each function extracts information
      // from the vector pose_data_vec. output is then appended to the end of the pose_database.txt
      // file.
      
      ostringstream datastr;
      datastr<<data[0];
      output=datastr.str();

      
      for (i=1;i<length;i++){
      
      ostringstream datastr;
      datastr<<data[i];
      output = output + ";" + datastr.str();
      }
      
      log <<  output << endl;
      cout << output << endl;
}

