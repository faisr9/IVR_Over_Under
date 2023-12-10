#include <fstream>
#include <iostream>
#include <string.h>
#include <sstream>
#include <iomanip>

using namespace std;

class Logger {
    private:
        inline static const std::string list_file = "fileLists.txt";
        FILE* logFile;
        std::string file_name;
        std::string file_mode;
        bool log_time;
        bool isFileOpen = false;    
        void changeFileMode(std::string new_mode);
        bool appending = false;
    
    public:
        /**
         * \brief Creates a logger object which logs data to a specified file
         * 
         * \param file_name The name of the file to log to
         * \param overwrite Whether or not to overwrite the file if it already exists. (ignores append parameter)
         * \param append Whether or not to append to the file if it already exists
         * \param log_time Whether or not to log the time of each log message
        */
        Logger(std::string file_name, bool overwrite=false, bool append=false, bool log_timer=true);
        ~Logger();

        void logStringMessage(std::string message);
        void logCharMessage(const char* message, ...);
        void logArray(std::string array_name, int* array, int array_length);
};