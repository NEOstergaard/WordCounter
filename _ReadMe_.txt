Design notes
============

How to build the solution
-------------------------

* Load the WordCounter\WordCounter.sln into Visual Studio
  I have been using Visual Studio 2019

* Compile the solution

How to test
-----------

This is an example of a test run

> cd WordCounter\Testing
>..\x64\Debug\WordCounter.exe
Type in data directory (type '-' to stop): .\data
Data processed
Type in data directory (type '-' to stop): -

>dir
 Volume in drive C has no label.
 Volume Serial Number is 5E63-7A40

 Directory of C:\Users\neo\source\repos\WordCounter\Testing

09-09-2020  18:44    <DIR>          .
09-09-2020  18:44    <DIR>          ..
07-09-2020  16:46    <DIR>          data
09-09-2020  18:33                77 exclude.txt
09-09-2020  18:44               106 excluded_words_count.txt
09-09-2020  18:44               147 FILE_A.txt
09-09-2020  18:44                25 FILE_B.txt
09-09-2020  18:44               145 FILE_C.txt
09-09-2020  18:44               102 FILE_D.txt
09-09-2020  18:44               140 FILE_E.txt
09-09-2020  18:44                98 FILE_F.txt
09-09-2020  18:44                12 FILE_G.txt
09-09-2020  18:44                52 FILE_H.txt
09-09-2020  18:44                88 FILE_I.txt
09-09-2020  18:44                10 FILE_J.txt
09-09-2020  18:44               111 FILE_L.txt
09-09-2020  18:44               147 FILE_M.txt
09-09-2020  18:44               125 FILE_N.txt
09-09-2020  18:44                29 FILE_O.txt
09-09-2020  18:44               216 FILE_P.txt
09-09-2020  18:44                30 FILE_Q.txt
09-09-2020  18:44                46 FILE_R.txt
09-09-2020  18:44               158 FILE_S.txt
09-09-2020  18:44               105 FILE_T.txt
09-09-2020  18:44                59 FILE_U.txt
09-09-2020  18:44               105 FILE_V.txt
              23 File(s)          2.133 bytes
               3 Dir(s)  110.265.749.504 bytes free

Decisions taken
---------------

* I have used C++17 as this gave access to std::filesystem for handling of files and folders.

* The solution only uses standard STL library calls and is not dependent on other libraries.

* Wide characters are not supported.

* The data directory that the user types in will be scanned for files and all regular files will be used
  to count words

* The file with excluded words 'exclude.txt' must be in the current directory where the program is executed
  from. The requirements did not specify exactly where it should be located.

* The excluded words in 'exclude.txt' must be typed in with one word per line and terminated with a newline.
  Eg.
    horse
    dog
    cat

* All generated output files are written to the current directory where the program is executed from.
  The requirements did not specify exactly where they should be located.

* When the user must type in the data directory, I have added the option to type '-' to exit the program.
  Otherwise it only exits in case of errors.
  
Design notes
------------

Class Main (Main.h/cpp) :
.........................

This is the entrance to the program. It orchestrates all the steps in the computation process.

Class JobExecuter  (JobExecuter.h/cpp)
Class JobBase      (JobBase.h)
Class WordCountJob (WordCountJob.h/cpp) :
.........................................

The 'JobExecuter' is a class where jobs to be executed in parallel can be added using add_job().
When all jobs has been added they can be started by a call to start(). The start() call will first
return when all jobs have finished executing.
To reduce CPU load and memory load the 'JobExecuter' takes a parameter 'no_parallel_jobs', that tells 
how many threads that must max run in parallel at one time. The start() function will execute jobs in bundles
of max 'no_parallel_jobs' jobs at one time. When all jobs in a bundle have finished, then the next bundle is
started.

'JobBase' is an abstract base class that all jobs must inherit from. Child classes must implement the
execute() function with the job functionality.

'WordCountJob' is a concrete implementation of a job. This is given a single file as input, which it will 
scan for words and count them.
Performance notes:
* The input file is read and scanned line by line so only a limited part of the file is in memory at a time.
* std::Regex is used for scanning a line for words. It could be investigated what the speed overhead is
  for this usage. As we do not need the full power of std::Regex a faster solution might be found.
* All data is stored in std::map<std::string, uint32_t> (word -> word count). 
  So memory footprint is determined by the number of different words.

Class Results (Results.h/cpp) : 
...............................

One object is created from this class and it is given along to each job executed. When a job has finished
it can store its word counts in this object. For this reason all member functions have been mutex protected.
The class will aggregate all results. It also supports various functions to do computations on the aggregated
data, like removing and counting excluded words. These are only called after all jobs have finished.

Class ExcludedWords(ExcludedWords.h/cpp) :
..........................................

This class will read a file with excluded words upon creation. It is used by the class Results to remove and
count excluded words in the aggregated results. Note that data is not modified after creation and class Results
only reads from it. So the class has not mutex protected functions, as they only read from the object.

(Utility.h/cpp) :
.................

A few utility functions used through out the code.


Missing stuff and improvements
------------------------------

* No unit tests have been implemented due to limited time.
  Many of the functions and classes would be fairly easy to test in some unit tests.

* The 'JobExecuter' was intended to be fairly generic, so it could execute different kinds of jobs.
  But the following call
      virtual bool execute(const Results::WPtr& results) = 0;
  depends on class Results which is very much tied up to the functionality of 'WordCountJob'.
  So this could be improved. But I did not have the time to look into it.
  
* The 'JobExecuter' executes jobs in parallel in bundles of max 'no_parallel_jobs' threads.
  So the longest running thread in the bundle determines when the next bundle can be executed.
  An improvement would be to start a new thread when ever a thread finishes. But with the requirement
  that max 'no_parallel_jobs' threads runs at one time.
  I did not have time to look into this.
