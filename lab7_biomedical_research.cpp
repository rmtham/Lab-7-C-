/******************************************************************************/
// Filename:      lab_7_biomedical_research
//
// Author:        Michelle Tham
//
// Date:          2019.08.08
//
// Modifications: Michelle Tham - 2019.08.09
//
// Description:   This program analyzes a subset of data from a study on the
//                strain and effects of bicycling and walking commuting trips. 
//                First, it takes in data from the file, "HR.txt", which 
//                contains the id number, clinically measured maximum heart 
//                rate, and age of each subject in addition to the subject's
//                daily commuting heart rate, maximum commuting heart rate, and
//                exercise heart rate over the course of five days. The program
//                then processes this data to find the weekly average of the 
//                daily commuting heart rate, number of days the subject 
//                exercised independently, estimated maximum heart rate, ratio
//                of measured maximum heart rate to estimated maximum heart rate
//                and the ratio of highest commuting heart rate to measured 
//                maximum heart rate. Finally, the program outputs this data, 
//                along with the subject's id number, to the file
//                "subject_data_summary.txt".
//
/******************************************************************************/

/**************************/
// Preprocessor Directives
/**************************/
#include <iostream>
#include <fstream>
#include <cstdlib> // Needed for the exit function
#include <iomanip>

/**************************/
//       namespace
/**************************/
using namespace std;

/**************************/
//   Function Prototypes
/**************************/
void get_subjects_data(ifstream &, int, int, int, double, int, double,
   int &, int[], int[], int[], double[], double[], double[]);

double calc_weekly_ave(double, int);
int calc_est_max_HR(int);
double calc_ratio(double, int);

void selection_sort_subs_data(int[], double[], int[], int[], double[], double[],
   int);
void swap(int &, int &); // overloading function, some arrays contain int 
                         //elements
void swap(double &, double &); // overloading function, some arrays contain 
                               //double elements

void output_header(ofstream &);
void output_subjects_data(ofstream &, int[], double[], int[], int[], double[],
   double[], int);

/*****************************************************************************/
//                                 main()
/*****************************************************************************/
/*
*******************
File Stream Objects
*******************

ifstream inputFile; file stream object that will be linked to the file “HR.txt”,
                    which is where the program will read data from
ofstream outputFile; file stream object that will be linked to the file 
                     “subject_data_summary.txt”, which is where the program will
                     write the processed subject data to

*****************
   Constants
*****************

const int SIZE; size declarator of the six arrays that hold participant data

*****************
    Variables
*****************

int id_num; id number of subject
int clinical_max_HR; clinically measured maximum heart rate of subject
int age; age of subject
int max_CHR; maximum commuting heart rate of subject
int subject_count; tracker for total number of subjects

double day_ave_CHR; daily average commuting heart rate of subject
double excer_HR; exercise heart rate of subject

*****************
     Arrays
*****************

int subs_id_num[]; stores the id number for all subjects
int subs_num_days_excer[]; stores the number of days exercised independently for
                           all subjects
int subs_est_max_HR[]; stores the estimated maximum heart rate for all subjects
double subs_weekly_ave_CHR[]; stores the weekly average of the average daily 
                              commuting heart rates for all subjects
double subs_max_HR_to_est_HR[]; stores the ratio of the measured maximum heart
                                rate to estimated maximum heart rate for all 
                                subjects
double subs_max_CHR_to_max_HR[]; stores the ratio of the highest commuting heart
                                 rate of the week to measured maximum heart rate
                                 for all subjects

*****************
Invoked Functions
*****************

1a. “get_subjects_data” inputs the data from “HR.txt” to:
    a) id_num
    b) clinical_max_HR
    c) age
    d) day_ave_CHR
    e) max_CHR
    f) exer_HR
1b. “get_subjects_data” also processes the input data and stores the results in:
    a) subs_id_num
    b) subs_num_days_excer
    c) subs_est_max_HR
    d) subs_weekly_ave_CHR
    e) subs_max_HR_to_est_HR
    f) subs_max_CHR_to_max_HR

2. “selection_sort_subs_data” sorts the subject id numbers, which determines the 
   element order of the following arrays:
   a) subs_id_num
   b) subs_weekly_ave_CHR
   c) subs_num_days_excer
   d) subs_est_max_HR
   e) subs_max_HR_to_est_HR
   f) subs_max_CHR_to_max_HR

3. “output_header” writes the header of the Commuting and Exercise Heart Rate
   Summary

4. “output_subjects_data” writes each of the subject’s data stored in the
   following arrays:
   a) subs_id_num
   b) subs_weekly_ave_CHR
   c) subs_num_days_excer
   d) subs_est_max_HR
   e) subs_max_HR_to_est_HR
   f) subs_max_CHR_to_max_HR

/*****************************************************************************/
int main()
{
   ifstream inputFile;
   ofstream outputFile;

   // Oversizes the array since we "do not know" how many subjects there are
   const int SIZE = 20;

   // Declares and initializes the variables used for initial data collection
   int id_num = 0, clinical_max_HR = 0, age = 0, max_CHR = 0, subject_count = 0;
   double day_ave_CHR = 0, exer_HR = 0;

   // Declares the arrays that will hold all of the subjects' numeric data
   int subs_id_num[SIZE], subs_num_days_excer[SIZE], subs_est_max_HR[SIZE];
   double subs_weekly_ave_CHR[SIZE], subs_max_HR_to_est_HR[SIZE],
      subs_max_CHR_to_max_HR[SIZE];

   /*******************************************************************************
                                   INPUT & CALCULATE
   *******************************************************************************/
   inputFile.open("HR.txt");

   // Checks whether there is an opening failure for input file. If so, the program
   // terminates prematurely. If not, the program proceeds to read in the subjects'
   // data from "HR.txt".
   if (inputFile.fail())
   {
      cout << "Failed to open file" << endl;
      exit(0);
   }
   else
   {
      get_subjects_data(inputFile, id_num, clinical_max_HR, age, day_ave_CHR, 
         max_CHR, exer_HR, subject_count, subs_id_num, subs_num_days_excer,
         subs_est_max_HR, subs_weekly_ave_CHR, subs_max_HR_to_est_HR,
         subs_max_CHR_to_max_HR);
   }
   
   // After the calculation and storage of values is finished, the elements in 
   // all of the arrays are sorted according to the order of the subs_id_num array
   // elements (which are sorted from low to high). This enables each "column" to 
   // represent each subject's numeric data
   selection_sort_subs_data(subs_id_num, subs_weekly_ave_CHR, 
      subs_num_days_excer, subs_est_max_HR, subs_max_HR_to_est_HR,
      subs_max_CHR_to_max_HR, subject_count);

   /*******************************************************************************
                                      OUTPUT
   *******************************************************************************/

   outputFile.open("subject_data_summary.txt", ios::app);

   // Checks whether there is an opening failure for the output file. If so, the 
   // program terminates prematurely. If not, the program proceeds to write to 
   // "subject_data_summary.txt" the header and then each subject's data
   if (outputFile.fail())
   {
      cout << "Failed to open file" << endl;
      exit(0);
   }
   else
   {
      output_header(outputFile);

      output_subjects_data(outputFile, subs_id_num, subs_weekly_ave_CHR,
         subs_num_days_excer, subs_est_max_HR, subs_max_HR_to_est_HR,
         subs_max_CHR_to_max_HR, subject_count);
   }

   inputFile.close();
   outputFile.close();

   return 0;
}
/*****************************************************************************/
//                               End of main()
/*****************************************************************************/


/*****************************************************************************/
//                           Function Definitions
/*****************************************************************************/

/******************************************************************************/
//
// Function:   double calc_weekly_ave(double sum_CHR, int valid_CHR_days)
//
// Parameters: double sum_CHR     - the sum of the subject’s daily commuting 
//  (Inputs)                        heart rates, in beats per minute
//             int valid_CHR_days - the number of days the heart rate monitor 
//                                  gave valid readings
//
// Outputs:    None
//
// Returns:    double - average_CHR, in beats per minute
//
// Author:     Michelle Tham
//
// Date:       2019.08.08
//
// Modifications: Michelle Tham - 2019.08.09
//
// Description: This function is provided the sum of the subject’s daily 
// commuting heart rates (bpm) as well as the number of days the heart rate
// monitor gave valid readings, and returns the weekly average of the daily 
// commuting heart rates (bpm). It uses the formula:
//
//                                          daily commuting heart rates sum
// weekly average of daily commuting HR =  ----------------------------------
//                                             days with valid readings
/******************************************************************************/
double calc_weekly_ave(double sum_CHR, int valid_CHR_days)
{
   double average_CHR;
   average_CHR = sum_CHR / valid_CHR_days;

   return average_CHR;
}

/******************************************************************************/
//
// Function:   int calc_est_max_HR(int age)
//
// Parameters: int age - age of subject, years
// (Inputs)    
//
// Outputs:    None
//
// Returns:    int - est_max_HR, in beats per minute
//
// Author:     Michelle Tham
//
// Date:       2019.08.08
//
// Modifications: Michelle Tham - 2019.08.09
//
// Description: This function is provided the age of the subject (years) and 
// returns the estimated maximum heart rate of the subject (bpm). It uses the
// formula:
//
//            estimated maximum heart rate = 220 - age of subject
//
/******************************************************************************/
int calc_est_max_HR(int age)
{
   int est_max_HR;
   est_max_HR = 220 - age;

   return est_max_HR;
}

/******************************************************************************/
//
// Function:   double calc_ratio(double HR1, int HR2)
//
// Parameters: double HR1 - first given heart rate, in beats per minute
// (Inputs)    int HR2    - second given heart rate, in beats per minute
//     
// Outputs:    None
//
// Returns:    double - ratio, as a percent
//
// Author:     Michelle Tham
//
// Date:       2019.08.08
//
// Modifications: Michelle Tham - 2019.08.09
//
// Description: This function is given the first given heart rate (bpm) as 
// well as the second given heart rate (bpm), and returns the ratio of these
// two given heart rates as a percent. It uses this formula:
//
//      Ratio = (first given heart rate / second given heart rate) * 100
//
// The result of the first given heart rate divided by the second given 
// heart rate is multiplied by 100 to convert it to a percent.
//
/******************************************************************************/
double calc_ratio(double HR1, int HR2)
{
   double ratio;
   ratio = (HR1 / HR2) * 100;

   return ratio;
}

/******************************************************************************/
//
// Function:   void get_subjects_data(ifstream &inFile, 
//                                    int id_num, 
//                                    int clinical_max_HR,
//                                    int age, 
//                                    double day_ave_CHR, 
//                                    int max_CHR, 
//                                    double exer_HR, 
//                                    int& sub_count,
//                                    int subs_id_num[], 
//                                    int subs_num_days_excer[], 
//                                    int subs_est_max_HR[],
//                                    double subs_weekly_ave_CHR[], 
//                                    double subs_max_HR_to_est_HR[],
//                                    double subs_max_CHR_to_max_HR[])
//
// Parameters: ifstream &inFile    - file stream object linked to “HR.txt”
// (Inputs)    int id_num          - id number of subject
//             int clinical_max_HR - clinically measured maximum heart rate of
//                                   subject, in beats per minute
//             int age             - age of subject, in years
//             double day_ave_CHR  - daily average commuting heart rate of
//                                   subject, in beats per minute
//             int max_CHR         - maximum commuting heart rate of subject, in
//                                   beats per minute
//             double exer_HR      - exercise heart rate of subject, in beats
//                                   per minute
//             int &sub_count      - tracker for total number of subjects
//
//             int subs_id_num[]               - stores the id number for all 
//                                               subjects
//             int subs_num_days_excer[]       - stores the number of days 
//                                               exercised independently for
//                                               all subjects
//             int subs_est_max_HR[]           - stores the estimated maximum 
//                                               heart rate for all subjects
//             double subs_weekly_ave_CHR[]    - stores the weekly average of 
//                                               the average daily commuting  
//                                               heart rates for all subjects                                           
//             double subs_max_HR_to_est_HR[]  - stores the ratio of the measured
//                                               maximum heart rate to estimated
//                                               maximum heart rate for all 
//                                               subjects, as a percent
//             double subs_max_CHR_to_max_HR[] - stores the ratio of the highest
//                                               commuting heart rate of the
//                                               week to measured maximum heart
//                                               rate for all subjects, as a 
//                                               percent
//
// Outputs:    None  
//
// Returns:    None
//
// Author:     Michelle Tham
//
// Date:       2019.08.08
//
// Modifications: Michelle Tham - 2019.08.09
//
// Description: This function reads in data from the file, “HR.txt,” and stores
// it in the following variables: id_num, clinical_max_HR, age, day_ave_CHR, 
// max_CHR, and exer_HR. Additional “processing variables” such as 
// num_valid_CHR_days, num_days_exer, highest_max_HR, temp_max_HR, and sum_CHR
// are declared and initialized in this function. These variables allow the 
// function to get the weekly average of the daily commuting heart rate, 
// number of days exercised independently, estimated maximum heart rate, ratio 
// of the maximum heart rate to estimated maximum heart rate, and the ratio of the
// highest commuting heart rate of the week to measured maximum heart rate. For 
// the data that requires further processing, the function calls 
// “calc_weekly_ave”, “calc_est_max_HR”, and “calc_ratio” and stores the results
// in the appropriate array elements.
//
/******************************************************************************/
void get_subjects_data(ifstream &inFile, int id_num, int clinical_max_HR,
   int age, double day_ave_CHR, int max_CHR, double exer_HR, int &sub_count,
   int subs_id_num[], int subs_num_days_excer[], int subs_est_max_HR[], 
   double subs_weekly_ave_CHR[], double subs_max_HR_to_est_HR[],
   double subs_max_CHR_to_max_HR[])
{
   while (!inFile.eof())
   {
      inFile >> id_num;
      inFile >> clinical_max_HR;
      inFile >> age;

      int num_valid_CHR_days = 0, num_days_exer = 0, highest_max_HR = 0,
         temp_max_HR = 0;
      double sum_CHR = 0;

      for (int idx = 0; idx < 5; idx++)
      {
         
         inFile >> day_ave_CHR;
         
         // If the heart rate monitor gives a false reading, nothing is added
         // to the current sum of the daily commuting heart rates. If there is
         // a valid reading, the value that is read in added to the sum and the
         // number of valid days increases by one
         if (day_ave_CHR == -1)
         {
            day_ave_CHR = 0;
         }
         else
         {
            sum_CHR += day_ave_CHR;
            num_valid_CHR_days++;
         }
         
         inFile >> temp_max_HR;

         // If the data that is read in is higher than the previous data read 
         // in, highest_max_HR is replaced by the more recent data read in
         if (temp_max_HR > highest_max_HR)
         {
            highest_max_HR = temp_max_HR;
         }

         inFile >> exer_HR;

         // If the heart monitor indicates that the subject's heart rate is 
         // greater than 0 (i.e., the subject exercised that day), the 
         // number of days that the subject exercised is increased by one
         if (exer_HR > 0)
         {
            num_days_exer++;
         }   
         
      }

      subs_id_num[sub_count] = id_num;

      subs_weekly_ave_CHR[sub_count] = calc_weekly_ave(sum_CHR,
         num_valid_CHR_days);

      subs_num_days_excer[sub_count] = num_days_exer;

      subs_est_max_HR[sub_count] = calc_est_max_HR(age);

      subs_max_HR_to_est_HR[sub_count] = calc_ratio(clinical_max_HR,
         subs_est_max_HR[sub_count]);

      subs_max_CHR_to_max_HR[sub_count] = calc_ratio(highest_max_HR,
         clinical_max_HR);

      sub_count++;
      
   }
}

/******************************************************************************/
//
// Function:   void selection_sort_subs_data(int id_num[], 
//                                           double weekly_ave_CHR[], 
//                                           int num_days_excer[], 
//                                           int est_max_HR[],
//                                           double max_HR_to_est_HR[], 
//                                           double max_CHR_to_max_HR[],
//                                           int sub_count)
//
// Parameters: int id_num[]               - stores the id number for all 
// (Inputs)                                 subjects
//             double weekly_ave_CHR[]    - stores the weekly average of the 
//                                          average daily commuting heart rates 
//                                          for all subjects
//             int num_days_excer[]       - stores the number of days exercised
//                                          independently for all subjects
//             int est_max_HR[]           - stores the estimated maximum heart 
//                                          rate for all subjects
//             double max_HR_to_est_HR[]  - stores the ratio of the measured
//                                          maximum heart rate to estimated 
//                                          maximum heart rate for all subjects
//             double max_CHR_to_max_HR[] - stores the ratio of the highest
//                                          commuting heart rate of the week to
//                                          the measured maximum heart rate for 
//                                          all subjects
//
//             int sub_count              - total number of subjects
//
// Outputs:    None
//
// Returns:    None
//
// Author:     Michelle Tham
//
// Date:       2019.08.08
//
// Modifications: Michelle Tham - 2019.08.09
//
// Description: This function uses selection sort to rearrange the id numbers 
// stored in the array id_num from low to high. It calls the swap function to 
// “swap” the starting value with the value that is lowest in relation to the
// starting value in id_num[]. The swap function is also called to swap the 
// elements in the arrays: weekly_ave_CHR[], num_days_excer[], est_max_HR[], 
// max_HR_to_est_HR[], and max_CHR_to_max_HR[], according to the order of the 
// elements of id_num[]. The allows each “column” of the arrays to represent the
// numeric data for each subject.
//
/******************************************************************************/
void selection_sort_subs_data(int id_num[], double weekly_ave_CHR[], 
   int num_days_excer[], int est_max_HR[], 
   double max_HR_to_est_HR[], double max_CHR_to_max_HR[], int sub_count)
{
   int min_index, min_value;

   for (int start = 0; start < (sub_count - 1); start++)
   {
      min_index = start;
      min_value = id_num[start];

      for (int idx = start + 1; idx < sub_count; idx++)
      {
         if (id_num[idx] < min_value)
         {
            min_value = id_num[idx];
            min_index = idx;
         }
      }
      swap(id_num[min_index], id_num[start]);
      swap(num_days_excer[min_index], num_days_excer[start]);
      swap(est_max_HR[min_index], est_max_HR[start]);
      swap(weekly_ave_CHR[min_index], weekly_ave_CHR[start]);
      swap(max_HR_to_est_HR[min_index], max_HR_to_est_HR[start]);
      swap(max_CHR_to_max_HR[min_index], max_CHR_to_max_HR[start]);
   }
}

/******************************************************************************/
//
// Function:   void swap(int &min_value, int &current_value)
//
// Parameters:  int &min_value     - the array element that corresponds to the 
// (Inputs)                          lowest id number value in id_num[] in 
//                                   relation to the current starting value 
//                                   (i.e., it has the same subscript as the 
//                                   array element containing the lowest id 
//                                   number value in relation to the current 
//                                   starting value)
//              int &current_value - the array element that corresponds to the 
//                                   starting value of id_num[] (i.e., it has 
//                                   the same subscript as current starting  
//                                   element of id_num[])     
//
// Outputs:    None
//
// Returns:    None
//
// Author:     Michelle Tham
//
// Date:       2019.08.08
//
// Modifications: Michelle Tham - 2019.08.09
//
// Description: This function is provided the element of an array that has the 
// same subscript as the lowest id number value in relation to the current 
// starting value in the array id_num[] and the element of an array that has 
// the same subscript as current starting element of id_num[]. The temporary
// variable, temp, is used to swap the values stored in the given array 
// elements. Since these array elements are passed by reference, this value swap
// alters the values stored in the array elements in “selection_sort_subs_data,” 
// which in turn modifies the array itself. This is because arrays are treated 
// as reference variables. This function shares the same name as 
// void swap(double &min_value, double &current_value), but its parameter data 
// types are different because some of the functions contain int elements.
//
/******************************************************************************/
void swap(int &min_value, int &current_value)
{
   int temp = min_value;
   min_value = current_value;
   current_value = temp;
}

/******************************************************************************/
//
// Function:   void swap(double &min_value, double &current_value)
//
// Parameters:  double &min_value     - the array element that corresponds to 
//                                      the lowest id number value in id_num[],
// (Inputs)                             in relation to the current starting
//                                      value (i.e., it has the same subscript
//                                      as the element with the lowest id number
//                                      value in relation to the current 
//                                      starting value)
//              double &current_value - the array element that corresponds to  
//                                      the starting value of id_num[] (i.e., 
//                                      it has the same subscript as current
//                                      starting element of id_num[])                                       
//
// Outputs:    None
//
// Returns:    None
//
// Author:     Michelle Tham
//
// Date:       2019.08.08
//
// Modifications: Michelle Tham - 2019.08.09
//
// Description: This function is provided the element of an array that has the 
// same subscript as the lowest id number value in relation to the current 
// starting value in the array id_num and the element of an array that has the
// same subscript as current starting element of array id_num. The temporary 
// variable, temp, is used to swap the values stored in the given array 
// elements. Since these array elements are passed by reference, this value swap
// alters the values stored in the array elements in “selection_sort_subs_data,”
// which in turn modifies the array itself. This is because arrays are treated 
// as reference variables. This function shares the same name as 
// void swap(int &min_value, int &current_value), but its parameter data 
// types are different because some of the functions contain double elements.
//
/******************************************************************************/
void swap(double &min_value, double &current_value)
{
   double temp = min_value;
   min_value = current_value;
   current_value = temp;
}

/******************************************************************************/
//
// Function:   void output_header(ofstream &outputFile)
//
// Parameters: ofstream outputFile - file stream object linked to the file 
// (Inputs)                          “subject_data_summary.txt”     
//
// Outputs:    The header of the Commuting and Exercise Heart Rate Summary
//
// Returns:    None
//
// Author:     Michelle Tham
//
// Date:       2019.08.08
//
// Modifications: Michelle Tham - 2019.08.09
//
// Description: This function writes the header of the Commuting and Exercise 
// Heart Rate Summary, which includes the: “SUBJECT NUMBER,” 
// “AVERAGE COMMUTING HR,” “DAYS EXERCISED,”  “ESTIMATED MAX HR,” 
// “%MEASURED TO ESTIMATED MAX HR,” and “%MAX COMMUTING HR TO MEASURED” written 
// in columns.
//
/******************************************************************************/
void output_header(ofstream &outputFile)
{
   const int COL1 = 11, COL2 = 8, SHIFT1 = 14, SHIFT2 = 3, SHIFT3 = 22,
      SHIFT4 = 44;

   // Writes the title of the text file
   outputFile << left << "COMMUTING AND EXERCISE HEART RATE SUMMARY" << endl
      << endl;

   // Writes the data header for the Commuting and Exercise Heart Rate
   // Summary

   // Writes the first line
   outputFile << left << setw(COL1) << "SUBJECT" << setw(COL1) << "AVERAGE"
      << setw(COL1) << "DAYS" << setw(COL1) << "ESTIMATED" << setw(COL1)
      << "%MEASURED" << setw(COL1) << "%MAX" << endl;

   // Writes the second line
   outputFile << left << setw(COL1) << "NUMBER" << setw(COL1) << "COMMUTING"
      << setw(COL1) << "EXERCISED" << setw(COL1) << "MAX HR" << setw(SHIFT2)
      << ' ' << setw(COL2) << "TO" << setw(COL1) << "COMMUTING" << endl;

   // Writes the third line
   outputFile << left << setw(COL1) << ' ' << setw(COL1) << "HR"
      << setw(SHIFT3) << ' ' << setw(COL1) << "ESTIMATED"
      << setw(COL1) << "HR TO" << endl;

   // Writes the fourth line
   outputFile << left << setw(SHIFT4) << ' ' << setw(COL1) << "MAX HR"
      << setw(COL1) << "MEASURED" << endl << endl;
}

/******************************************************************************/
//
// Function:   void output_subjects_data(ofstream &outputFile, 
//                                       int id_num[], 
//                                       double weekly_ave_CHR[], 
//                                       int num_days_excer[], 
//                                       int est_max_HR[],
//                                       double max_HR_to_est_HR[],
//                                       double max_CHR_to_max_HR[], 
//                                       int sub_count)
//
// Parameters: int id_num[]               - stores the id number for all 
// (Inputs)                                 subjects
//             double weekly_ave_CHR[]    - stores the weekly average of the 
//                                          average daily commuting heart rates 
//                                          for all subjects
//             int num_days_excer[]       - stores the number of days exercised
//                                          independently for all subjects
//             int est_max_HR[]           - stores the estimated maximum heart 
//                                          rate for all subjects
//             double max_HR_to_est_HR[]  - stores the ratio of the measured
//                                          maximum heart rate to estimated 
//                                          maximum heart rate for all subjects
//             double max_CHR_to_max_HR[] - stores the ratio of the highest
//                                          commuting heart rate of the week to
//                                          measured maximum heart rate for all
//                                          subjects
//
//             int sub_count              - total number of subjects
//
// Outputs:    int[]    - id_num 
//             double[] - weekly_ave_CHR
//             int[]    - num_days_excer
//             int[]    - est_max_HR
//             double[] - max_HR_to_est_HR
//             double[] - max_CHR_to_max_HR
//
// Returns:    None
//
// Author:     Michelle Tham
//
// Date:       2019.08.08
//
// Modifications: Michelle Tham - 2019.08.09
//
// Description: This function outputs the numeric data for each subject. The 
// number data includes: the subject’s id number, weekly average of the daily 
// commuting heart rates, number of days exercised independently, estimated 
// maximum heart rate, ratio of measured maximum heart rate to estimated maximum
// heart rate, and ratio of highest commuting heart rate of the week to measured
// maximum heart rate.
//
/******************************************************************************/
void output_subjects_data(ofstream &outputFile, int id_num[], 
   double weekly_ave_CHR[], int num_days_excer[], int est_max_HR[],
   double max_HR_to_est_HR[], double max_CHR_to_max_HR[], int sub_count)
{
   const int COL1 = 4, SHIFT1 = 7, COL2 = 5, SHIFT2 = 6, COL3 = 1, 
      SHIFT3 = 10, COL4 = 3, SHIFT4 = 8;

   // For each subject, writes a line containing all of the numeric data for
   // that subject
   for (int idx = 0; idx < sub_count; idx++)
   {
      outputFile << setw(COL1) << id_num[idx] << setw(SHIFT1) << ' ';

      outputFile << showpoint << setprecision(1) << fixed;
      outputFile << setw(COL2) << weekly_ave_CHR[idx] << setw(SHIFT2)
         << ' ';

      outputFile << setprecision(1);
      outputFile << setw(COL3) << num_days_excer[idx] << setw(SHIFT3)
         << ' ';

      outputFile << setprecision(3);
      outputFile << setw(COL4) << est_max_HR[idx] << setw(SHIFT4)
         << ' ';

      outputFile << showpoint << setprecision(1) << fixed;
      outputFile << setw(COL2) << max_HR_to_est_HR[idx] << setw(SHIFT2)
         << ' ';
      outputFile << setw(COL2) << max_CHR_to_max_HR[idx] << endl
         << endl;
   }
}