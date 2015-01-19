//the following function writes laserscan data to a text file
//Author: Group03

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

void laserscan_data_write(vector<float> data)
{

  int length=data.size(); //get length of data array
  
  // Definitions for writing data
  int i = 0;
  string output; 

  // Preparing database text file.  Each time laserscan_data_write() is called,
  // new readings will be appended.
  ofstream log("scan_database.txt", ios_base::app | ios_base::out); //generate a connection to the text file

  // The execution of writing manipulated data
      // The output variable is a semi-colon separated concatenation of the results. Each function extracts information
      // from the data vector. output is then appended to the end of the laserscan_database.txt
      // file.
      
      //first datum in a new row
      ostringstream datastr;
      datastr<<data[0];
      output=datastr.str();

      
      for (i=1;i<length;i++){
      
      //each iteration appends one datum to the output string
      ostringstream datastr;
      datastr<<data[i];
      output = output + ";" + datastr.str();
      }
      
      log <<  output << endl; //write to file
      cout << output << endl; //display results
}

