#include "main.h"
// Add a check for a existing file, if it exists, add a number to the end of the file name
// Currently it requires us to pass in the file name, so given the name, check if its the same
// give the file List. If it is, then get current number, add 1, and append that to the file name

/**
 * @brief Returns the time since program start in the format of 
 *      minutes:seconds.milliseconds as a formatted string
 * NOTE: This will be moves to a future macros file, for now it is here
 */
std::string getTimeStamp_str() {
    u_int32_t milliseconds = pros::millis();
    
    std::stringstream ss;
    ss << std::setfill('0') << 
        std::setw(2) << (milliseconds / (60 * 1000)) << ":" << 
        std::setw(2) << ((milliseconds / 1000) % 60) << "." << 
        std::setw(3) << (milliseconds % 1000) << "\t";
    return ss.str();
}

 /**
  * @brief Creates the single instance of the class. This is done here to ensure
  * the object is created before main() is called allowing the autoLogger to function
  * properly.
  */
AutoLogger* autoLogger = AutoLogger::createInstance();

/**
 * The autoLogger class is a singleton class which automatically logs important
 * information about the robot during competition. This includes the time, battery,
 * device status, initialization status, and more. This class is automatically created
 * when the robot is connected to the field network or competition switch, or otherwise
 * is paused.
 * 
 * It can be manually started and stopped using the startAutoLog() and stopAutoLog()
 * methods via the GUI
 */
AutoLogger::AutoLogger() {
    FILE* autoLogWrite = fopen(auto_log_file_name.c_str(), "a");
    const char* break_message = "---------------------------\nStart of new log\n---------------------------\n";
    if (autoLogWrite)
        fwrite(break_message, sizeof(char), strlen(break_message), autoLogWrite);
    fclose(autoLogWrite);
}
AutoLogger::~AutoLogger() {
    if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }
}

AutoLogger* AutoLogger::createInstance() {
    if (!instance_ || instance_ == nullptr)
        instance_ = new AutoLogger();

    return instance_;
}

AutoLogger* AutoLogger::getInstance() {
    if (instance_ == nullptr)
        throw std::runtime_error("AutoLogger: Attempting to getInstance while instance_ is nullptr");

    return instance_;
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////              Logger Class           ///////////////////////
///////////////////////////////////////////////////////////////////////////////////


Logger::Logger(std::string file_name, bool overwrite, bool append) {
    file_mode = "a"; // default to append as to not lose logs

    if (!overwrite) {
        if (append) {
            file_mode = "a";
            appending = true;
        }
        else {
            if (file_name.find_first_of("_") != std::string::npos) {
                std::string file_name_copy = file_name.substr(0, file_name.find_first_of("_"));
                std::string file_name_number = file_name.substr(file_name.find_first_of("_") + 1,
                    file_name.find_first_of(".") - file_name.find_first_of("_") - 1);

                short int file_name_number_int = std::stoi(file_name_number);
                file_name_number_int++;
                file_name = file_name_copy + "_" + std::to_string(file_name_number_int) + file_name.substr(file_name.find_first_of("."));
            }
            else
                file_name = file_name.substr(0, file_name.find_first_of(".")) + "_1" + file_name.substr(file_name.find_first_of("."));
        }
    }
    else
        file_mode = "w";

    FILE* logFileName = fopen(list_file.c_str(), "a");
    if (logFileName) {
        fwrite(file_name.c_str(), sizeof(char), file_name.length(), logFileName);
        fwrite("\n", sizeof(char), 1, logFileName);
        fclose(logFileName);
    }
    this->file_name = file_name;

    logFile = fopen(file_name.c_str(), file_mode.c_str());
    if(logFile)
        isFileOpen = true;

    const char* break_message = "---------------------------\nStart of new log\n---------------------------\n";
    if (logFile)
        fwrite(break_message, sizeof(char), strlen(break_message), logFile);
}

Logger::~Logger() {
    if(isFileOpen)
    {
        fclose(logFile);
        isFileOpen = false;
    }
}

void Logger::changeFileMode(std::string new_mode) {
    if (new_mode != "w" && new_mode != "a" && new_mode != "wb" && new_mode != "ab")
        throw std::runtime_error("Logger: Invalid file mode");
        // return; // Don't error if the mode is invalid, just don't change it

    this->file_mode = new_mode;
    if (isFileOpen && logFile)
    {
        fclose(logFile);
        isFileOpen = false;
    }
    logFile = fopen(file_name.c_str(), file_mode.c_str());
    if(logFile)
        isFileOpen = true;
}

void Logger::logMessage(std::string message) {
    if(!isFileOpen) // Failsafe
        logFile = fopen(file_name.c_str(), file_mode.c_str());

    if(appending && file_mode != "a")
        changeFileMode("a");
    else if(!appending && file_mode != "w")
        changeFileMode("w");

    if (logFile) {   
        fwrite(message.c_str(), sizeof(char), message.length(), logFile);
        fwrite("\n", sizeof(char), 1, logFile);
    }
}