#include "logtest.h"
#include <cstdarg>

/**
 * @brief Returns the time since program start in the format of 
 *      minutes:seconds.milliseconds as a formatted string
 * NOTE: This will be moves to a future macros file, for now it is here
 */
std::string getTimeStamp_str(int inptime) {
    int milliseconds = inptime;
    
    std::stringstream ss;
    ss << "[" << std::setfill('0') << 
        std::setw(2) << (milliseconds / (60 * 1000)) << ":" << 
        std::setw(2) << ((milliseconds / 1000) % 60) << "." << 
        std::setw(3) << (milliseconds % 1000) << "]" << "\t";
    return ss.str();
}

Logger::Logger(std::string file_name, bool overwrite, bool append, bool log_timer) {
    this->log_time = log_timer;
    file_mode = "a"; // default to append as to not lose logs

    if (!overwrite) {
        if (append) {
            file_mode = "a";
            appending = true;
        }
        else {
            appending = false;
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
    {
        file_mode = "w";
        appending = false;
    }

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
    cout << "Changing file mode to " << new_mode << endl;
    this->file_mode = new_mode;
    if (logFile)
    {
        fclose(logFile);
        isFileOpen = false;
    }
    logFile = fopen(file_name.c_str(), file_mode.c_str());
    // if(logFile)
    //     isFileOpen = true;
}

void Logger::logStringMessage(std::string message) {
    if(!isFileOpen) // Failsafe
        logFile = fopen(file_name.c_str(), file_mode.c_str());

    if(appending && file_mode != "a")
        changeFileMode("a");
    else if(!appending && file_mode != "w")
        changeFileMode("w");

    if (logFile) {
        if (log_time) {
            message.insert(0, getTimeStamp_str((rand() % 100000)+100));
        }
        
        fwrite(message.c_str(), sizeof(char), message.length(), logFile);
        fwrite("\n", sizeof(char), 1, logFile);
    }
}

void Logger::logCharMessage(const char* message, ...)
{
    if(!isFileOpen) // Failsafe
        logFile = fopen(file_name.c_str(), file_mode.c_str());

    if(appending && file_mode != "a")
        changeFileMode("a");
    else if(!appending && file_mode != "w")
        changeFileMode("w");

    if (logFile) {
        std::string logMessage = "";
        if (log_time) {
            logMessage += getTimeStamp_str((rand() % 100000)+100);
        }
        va_list args;
        va_start(args, message);
        vfprintf(logFile, (logMessage + message).c_str(), args);
        va_end(args);
        fwrite("\n", sizeof(char), 1, logFile);
    }
}

int main()
{
    srand(time(NULL));
    Logger logBuild("C:\\Ani Code\\Robotics Code Repo\\Other Test Code\\logFile.txt", false, false, true);
    logBuild.logStringMessage("Hello World");
    logBuild.logStringMessage("This is cool");
    logBuild.logStringMessage("Rishi is black");
    int x = 5;
    int y = 10;
    double z = 3.14159265358;
    logBuild.logCharMessage("x: %d, y: %d, z: %f", x, y, z);
    
    logBuild.~Logger();
    return 0;
}