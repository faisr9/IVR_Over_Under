#include "main.h"
// Add a check for a existing file, if it exists, add a number to the end of the file name
// Currently it requires us to pass in the file name, so given the name, check if its the same
// give the file List. If it is, then get current number, add 1, and append that to the file name

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
AutoLogger::AutoLogger() : Logger(auto_log_file_name, false, true) {
    Logger::closeLogFile();

    FILE* autoLogRead = fopen(auto_log_file_name.c_str(), "rb");
    if(fread(&lastIteration, sizeof(int), 1, autoLogRead) != EOF)
        lastIteration++;   
    else
        lastIteration = 0;

    fclose(autoLogRead);
    FILE* autoLogWrite = fopen(auto_log_file_name.c_str(), "wb");
    fwrite(&lastIteration, sizeof(int), 1, autoLogWrite);
    fwrite("\n", sizeof(char), 1, autoLogWrite);
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
    if(file_name.length() > 64)
        throw std::runtime_error("Logger: File name is too long, keep under 64 characters");

    if (!overwrite) {
        if (append) {
            file_mode = "a";
            appending = true;
        }
        else {
            appending = false;
            char buff[64];
            std::vector<std::string> fileNames;
            std::string file_name_copy;
            std::string file_name_number;
            short int file_name_number_int;
            short int highestFileNumber = 0;

            FILE* readFileName = fopen(list_file.c_str(), "r");
            while(fgets(buff, 64, readFileName))
            {
                fileNames.push_back(buff);
            }

            for (int i=0;i<fileNames.size();i++)
            {
                if(fileNames[i].find_first_of("_") != std::string::npos)
                {
                    file_name_copy = file_name.substr(0, file_name.find_first_of("_"));
                    file_name_number = fileNames[i].substr(fileNames[i].find_first_of("_") + 1,
                        fileNames[i].find_first_of(".") - fileNames[i].find_first_of("_") - 1);

                    file_name_number_int = std::stoi(file_name_number);
                }
                else
                    file_name_number_int = 0;
                
                if(file_name_number_int > highestFileNumber)
                    highestFileNumber = file_name_number_int;
            }
            highestFileNumber++;
            if(file_name_copy.find_first_of("_") == std::string::npos)
                file_name_copy = file_name.substr(0, file_name.find_first_of("."));

            file_name = file_name_copy + "_" + std::to_string(highestFileNumber) + 
                            file_name.substr(file_name.find_first_of("."));

            fclose(readFileName);
        }
    }
    else {
        file_mode = "w";
        appending = false;
    }
    
    this->file_name = file_name;
    FILE* logFileName = fopen(list_file.c_str(), "a");
    if (logFileName) {
        fwrite(file_name.c_str(), sizeof(char), file_name.length(), logFileName);
        fwrite("\n", sizeof(char), 1, logFileName); // Tructates the line for readback
        fclose(logFileName);
    }

    logFile = fopen(file_name.c_str(), file_mode.c_str());

    std::string break_message = "---------------------------\nStart of new log\n---------------------------\n";
    if (logFile)
        fwrite(break_message.c_str(), sizeof(char), break_message.length(), logFile);
}

Logger::~Logger() {
    if(logFile)
        fclose(logFile);
}

FILE* Logger::getLogFile() {
    return logFile;
} 

FILE* Logger::closeLogFile() {
    if(logFile) {
        fclose(logFile);
        logFile = nullptr;
    }
    return logFile;
}

/**
 * @brief Returns the time since program start in the format of 
 *      minutes:seconds.milliseconds as a formatted string
 */
std::string Logger::getTimeStamp_str() {
    u_int32_t milliseconds = pros::millis();
    
    std::stringstream ss;
    ss << std::setfill('0') << 
        std::setw(2) << (milliseconds / (60 * 1000)) << ":" << 
        std::setw(2) << ((milliseconds / 1000) % 60) << "." << 
        std::setw(3) << (milliseconds % 1000) << "\t";
    return ss.str();
}

void Logger::logStringMessage(std::string message) {
    if(!logFile) // Failsafe
        logFile = fopen(file_name.c_str(), file_mode.c_str());

    if (logFile) {
        message.insert(0, getTimeStamp_str());
        
        fwrite(message.c_str(), sizeof(char), message.length(), logFile);
        fwrite("\n", sizeof(char), 1, logFile);
    }
}

void Logger::logCharMessage(const char* message, ...)
{
    if(!logFile) // Failsafe
        logFile = fopen(file_name.c_str(), file_mode.c_str());

    if (logFile) {
        std::string logMessage = "";
        logMessage = getTimeStamp_str();

        va_list args;
        va_start(args, message);
        vfprintf(logFile, (logMessage + message).c_str(), args);
        va_end(args);
        fwrite("\n", sizeof(char), 1, logFile);
    }
}

template<typename T>
void Logger::logVarible(std::string var_name, T var) {
    if(!logFile) // Failsafe
        logFile = fopen(file_name.c_str(), file_mode.c_str());

    if (logFile) {
        var_name = "VAR: " + var_name + " = ";
        var_name.insert(0, getTimeStamp_str());
        fwrite(var_name.c_str(), sizeof(char), var_name.length(), logFile);
        fwrite(std::to_string(var).c_str(), sizeof(char), std::to_string(var).length(), logFile);
        fwrite("\n", sizeof(char), 1, logFile);
    }
}

template<>
void Logger::logVarible<std::string>(std::string var_name, std::string var) {
    if(!logFile) // Failsafe
        logFile = fopen(file_name.c_str(), file_mode.c_str());

    if (logFile) {
        var_name = "VAR: " + var_name + " = ";
        var_name.insert(0, getTimeStamp_str());
        fwrite(var_name.c_str(), sizeof(char), var_name.length(), logFile);
        fwrite(var.c_str(), sizeof(char), var.length(), logFile);
        fwrite("\n", sizeof(char), 1, logFile);
    }
}

template<typename T>
void Logger::logArray(std::string array_name, T* array, int array_length) {
    if(!logFile) // Failsafe
        logFile = fopen(file_name.c_str(), file_mode.c_str());

    if (logFile) {
        array_name = "ARRAY: " + array_name + "[" + std::to_string(array_length) + "] = ";
        array_name.insert(0, getTimeStamp_str());
        fwrite(array_name.c_str(), sizeof(char), array_name.length(), logFile);
        fwrite("{", sizeof(char), 1, logFile);
        for (int i = 0; i < array_length; i++) {
            fwrite(std::to_string(array[i]).c_str(), sizeof(char), std::to_string(array[i]).length(), logFile);
            if (i != array_length - 1)
                fwrite(", ", sizeof(char), 2, logFile);
        }
        fwrite("}\n", sizeof(char), 2, logFile);
    }
}

template<>
void Logger::logArray<std::string>(std::string array_name, std::string* array, int array_length) {
    if(!logFile) // Failsafe
        logFile = fopen(file_name.c_str(), file_mode.c_str());

    if (logFile) {
        array_name = "ARRAY: " + array_name + "[" + std::to_string(array_length) + "] = ";
        array_name.insert(0, getTimeStamp_str());
        fwrite(array_name.c_str(), sizeof(char), array_name.length(), logFile);
        fwrite("{", sizeof(char), 1, logFile);
        for (int i = 0; i < array_length; i++) {
            fwrite(array[i].c_str(), sizeof(char), array[i].length(), logFile);
            if (i != array_length - 1)
                fwrite(", ", sizeof(char), 2, logFile);
        }
        fwrite("}\n", sizeof(char), 2, logFile);
    }
}