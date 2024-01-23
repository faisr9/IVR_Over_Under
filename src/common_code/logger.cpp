#include "main.h"

// Logger Static Variables //
const std::string Logger::list_file = "fileLists.txt";

// AutoLogger Static Variables //
std::string AutoLogger::auto_log_file_name = "usd/autoLogData.txt";
const int AutoLogger::auto_log_delay = 1000; // milliseconds

AutoLogger* autoLogger = AutoLogger::createInstance();

AutoLogger::AutoLogger() : Logger(auto_log_file_name, false, false, false) {
    logNum = 0;
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

void AutoLogger::autoLogRunner()
{
    pros::Task autoLogTask{[=] {
    while(!terminate)
    {
        while(!paused)
        {
            Logger::logStringMessage("###########################################################");
            Logger::logStringMessage("HEADER");
            Logger::logStringMessage("-----------------------------------------------------------");
            // Need to add the getter functions for the robot states
            nextMessage = getTimeStamp_str() + "[comp_18] [COMP Disabled] [Log #" + std::to_string(logNum) + "]";
            Logger::logStringMessage(nextMessage);
            Logger::logStringMessage("-----------------------------------------------------------");
            Logger::logStringMessage("ROBOT SETTINGS");
            Logger::logStringMessage("-----------------------------------------------------------");
            // Waiting for releated methods to be made
            Logger::logStringMessage("Will log setting changes here");
            Logger::logStringMessage("-----------------------------------------------------------");
            Logger::logStringMessage("ROBOT DEVICES");
            Logger::logStringMessage("-----------------------------------------------------------");
            // Waiting for releated methods to be made
            Logger::logStringMessage("Will log device state changes here");
            Logger::logStringMessage("-----------------------------------------------------------");
            Logger::logStringMessage("IMPORTANT VARIABLES");
            Logger::logStringMessage("-----------------------------------------------------------");
            for(int i = 0; i < importantVaribles.size(); i++)
                Logger::logStringMessage(importantVaribles[i]);
            Logger::logStringMessage("-----------------------------------------------------------");
            Logger::logStringMessage("IMPORTANT EVENT MESSAGES");
            Logger::logStringMessage("-----------------------------------------------------------");
            for(int i = 0; i < importantMessages.size(); i++)
                Logger::logStringMessage(importantMessages[i]);            
            Logger::logStringMessage("###########################################################");
            importantVaribles.clear();
            importantMessages.clear();
            logNum++;

            pros::Task::delay(auto_log_delay);
        }
        pros::Task::delay(auto_log_delay);
    }
    }};
}

/*
    AutoLog Format Example:
    ###########################################################
    HEADER
    -----------------------------------------------------------
    [TIME Logged] [comp_18] [COMP Disabled] [Log #17]
    -----------------------------------------------------------
    ROBOT SETTINGS
    -----------------------------------------------------------
    [TIME Changed] Auton Program: AWP Tandom
    [TIME Changed] Driver Mode: Tank - "Specific Person"
    -----------------------------------------------------------
    ROBOT DEVICES
    -----------------------------------------------------------
    [TIME Logged] Motor1 {11, 18, true} - [Connected C:4] - [Okay]
    [TIME Logged] Motor2 {12, 18, false} - [Connected C:4] - [Okay]
    [TIME Logged] Motor3 {13, 18, true} - [Connected C:4] - [HOT]
    [TIME Logged] Motor4 {14, 18, false} - [DISCONNECTED C:4.5]
    [TIME Logged] Motor5 {15, 18, true} - [Connected C:4] - [Okay]
    [TIME Logged] Motor6 {16, 18, false} - [DISCONNECTED C:44.5]
    [TIME Logged] Motor7 {17, 18, true} - [Connected C:4] - [Okay]
    [TIME Logged] Motor8 {18, 18, false} - [Connected C:4] - [Okay]
    [TIME Logged] Motor9 {19, 18, true} - [Connected C:4] - [Okay]
    [TIME Logged] IMU {Installed} - [Connected C:4]
    -----------------------------------------------------------
    IMPORTANT VARIABLES
    -----------------------------------------------------------
    [TIME Logged] ODOMETRY: {x: 0, y: 0, theta: 0}
    [TIME Logged] CATAPAULT: {state: 0, speed: 0}
    -----------------------------------------------------------
    IMPORTANT EVENT MESSAGES
    -----------------------------------------------------------
    [00:15.334] [COMP Controller] [Robot is disabled]
    [00:15.754] [GUI] [Screensaver Active]
    ###########################################################

    Log Update Triggers:
        - Every second
        [ignored for now] - When a device state changes
        [ignored for now] - When a robot setting is changed
    The "IMPORTANT VARIBLES", "IMPORTANT EVENT MESSAGES" and "CUSTOM MESSAGES" sections will be 
        queued and logged every update
    
*/

void AutoLogger::pauseAutoLog()
{
    paused = true;
    nextMessage = getTimeStamp_str() + "AUTOLOG PAUSED";
    Logger::logStringMessage(nextMessage);
}

void AutoLogger::resumeAutoLog()
{
    paused = false;
    nextMessage = getTimeStamp_str() + "AUTOLOG RESUMED";
    Logger::logStringMessage(nextMessage);
}

[[deprecated("No need to force stop the autolog, it will stop when the program ends. There for edge cases.")]]
void AutoLogger::stopAutoLog()
{
    terminate = true;
    nextMessage = getTimeStamp_str() + "AUTOLOG STOPPED";
    Logger::logStringMessage(nextMessage);
}

void AutoLogger::motorUpdate()
{
    // This method will be made with the completion of the devices class
    // It will utilize the devices class to get the state of all motors
}

void AutoLogger::deviceUpdate()
{
    // This method will be made with the completion of the devices class
    // It will utilize the devices class to get the state of all devices
}

template<typename T>
void AutoLogger::logVarible(std::string var_name, T var)
{
    importantVaribles.push_back(getTimeStamp_str() + var_name + " = " + std::to_string(var));
}

template<>
void AutoLogger::logVarible<std::string>(std::string var_name, std::string var)
{
    importantVaribles.push_back(getTimeStamp_str() + var_name + " = " + var);
}

template<typename T>
void AutoLogger::logArray(std::string array_name, T* array, int array_length)
{
    nextMessage = "{";
    for (int i = 0; i < array_length; i++) {
        nextMessage += std::to_string(array[i]);
        if (i != array_length - 1)
            nextMessage += ", ";
    }
    nextMessage += "}";
    importantVaribles.push_back(getTimeStamp_str() + array_name + "[" + std::to_string(array_length) + "] = " + nextMessage);
}

template<>
void AutoLogger::logArray<std::string>(std::string array_name, std::string* array, int array_length)
{
    nextMessage = "{";
    for (int i = 0; i < array_length; i++) {
        nextMessage += array[i];
        if (i != array_length - 1)
            nextMessage += ", ";
    }
    nextMessage += "}";
    importantVaribles.push_back(getTimeStamp_str() + array_name + "[" + std::to_string(array_length) + "] = " + nextMessage);
}

void AutoLogger::logStringMessage(std::string message)
{
    // Add mutexes for multithreading
    importantMessages.push_back(getTimeStamp_str() + message);
}

void AutoLogger::logCharMessage(const char* message, ...)
{
    // Add mutexes for multithreading
    nextMessage = getTimeStamp_str();

    va_list args;
    va_start(args, message);
    char buffer[256];
    sprintf(buffer, (nextMessage + message).c_str(), args);
    nextMessage = buffer;
    va_end(args);
    importantMessages.push_back(nextMessage);
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////              Logger Class           ///////////////////////
///////////////////////////////////////////////////////////////////////////////////

Logger::Logger(std::string file_name, bool overwrite, bool append, bool timestamp) {
    file_mode = "a"; // default to append as to not lose logs
    this->timestamp = timestamp;
    std::vector<std::string> fileNames;

    if(file_name.length() > 64)
        throw std::runtime_error("Logger: File name is too long, keep under 64 characters");

    if (!overwrite) {
        char buff[64];
        std::string file_name_copy;
        std::string file_name_number;
        short int file_name_number_int;
        short int highestFileNumber = 0;

        FILE* readFileName = fopen(list_file.c_str(), "r");
        while(fgets(buff, 64, readFileName)) // Read file names into vector
            fileNames.push_back(buff);

        // Get true file name
        if(file_name.find_first_of("_") == std::string::npos)
            file_name_copy = file_name.substr(4, file_name.length()-4 - file_name.substr(file_name.find_first_of(".")).length());
        else
            file_name_copy = file_name.substr(4, file_name.length()-4 - file_name.substr(file_name.find_first_of("_")).length());
            
        for (int i=0;i<fileNames.size();i++) // Remove all file names that don't match the true file name
        {                
            if (fileNames[i].find(file_name_copy) == std::string::npos)
            {
                fileNames.erase(fileNames.begin() + i);
                i--;
            }
        }

        // Get highest file number
        for (int i=0;i<fileNames.size();i++)
        {
            if (fileNames[i].find_first_of("_") != std::string::npos)
            {
                file_name_copy = file_name.substr(0, file_name.find_first_of("_"));
                file_name_number = fileNames[i].substr(fileNames[i].find_first_of("_") + 1,
                    fileNames[i].find_first_of(".") - fileNames[i].find_first_of("_") - 1);

                if (!file_name_number.empty())
                {
                    file_name_number_int = std::stoi(file_name_number);
                    if (file_name_number_int > highestFileNumber)
                        highestFileNumber = file_name_number_int;
                }
            }
            else
            {
                file_name_number_int = 0;
            }
        }

        // Set file number and file name based on append parameter
        if (append) {
            file_mode = "a";
            appending = true;

            if(file_name_copy.find_first_of("_") == std::string::npos)
                file_name_copy = file_name.substr(0, file_name.find_first_of("."));
            
            file_name = file_name_copy + "_" + std::to_string(highestFileNumber) + 
                            file_name.substr(file_name.find_first_of("."));
        }
        else {
            appending = false;
            
            cout << "Highest File Number: " << highestFileNumber << endl;
            highestFileNumber++;
            this->file_num = highestFileNumber;
            cout << "New File Number: " << this->file_num << endl;
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

    // Check if file name is already in list
    bool fileExists = false;
    for (const std::string& fileNameInList : fileNames) {
        if (fileNameInList == (file_name + "\n")) {
            fileExists = true;
            break;
        }
    }

    // If file name is not in list, add it
    if (!fileExists) {
        FILE* logFileName = fopen(list_file.c_str(), "a");
        if (logFileName) {
            fwrite(file_name.c_str(), sizeof(char), file_name.length(), logFileName);
            fwrite("\n", sizeof(char), 1, logFileName); // Truncates the line for readback
            fclose(logFileName);
        }
    }

    // Open log file
    logFile = fopen(file_name.c_str(), file_mode.c_str());

    std::string break_message = "---------------------------\nStart of new log\n---------------------------\n";
    if (logFile)
        fwrite(break_message.c_str(), sizeof(char), break_message.length(), logFile);
}

Logger::~Logger() {
    if(logFile)
        fclose(logFile);
}

std::string Logger::getTimeStamp_str() {
    int milliseconds = pros::millis();
    
    std::stringstream ss;
    ss << "[" << std::setfill('0') << 
        std::setw(2) << (milliseconds / (60 * 1000)) << ":" << 
        std::setw(2) << ((milliseconds / 1000) % 60) << "." << 
        std::setw(3) << (milliseconds % 1000) << "]" << "\t";
    return ss.str();
}

void Logger::logStringMessage(std::string message) {
    if(!logFile) // Failsafe
        logFile = fopen(file_name.c_str(), file_mode.c_str());

    if (logFile) {
        if (timestamp)
            message.insert(0, getTimeStamp_str());
        
        fwrite(message.c_str(), sizeof(char), message.length(), logFile);
        fwrite("\n", sizeof(char), 1, logFile);
        fflush(logFile);
    }
}

void Logger::logCharMessage(const char* message, ...)
{
    if(!logFile) // Failsafe
        logFile = fopen(file_name.c_str(), file_mode.c_str());

    if (logFile) {
        std::string logMessage = "";
        if (timestamp)
            logMessage = getTimeStamp_str();

        va_list args;
        va_start(args, message);
        vfprintf(logFile, (logMessage + message).c_str(), args);
        va_end(args);
        fwrite("\n", sizeof(char), 1, logFile);
        fflush(logFile);
    }
}

template<typename T>
void Logger::logVarible(std::string var_name, T var) {
    if(!logFile) // Failsafe
        logFile = fopen(file_name.c_str(), file_mode.c_str());

    if (logFile) {
        var_name = "VAR: " + var_name + " = ";
        if (timestamp)
            var_name.insert(0, getTimeStamp_str());
        fwrite(var_name.c_str(), sizeof(char), var_name.length(), logFile);
        fwrite(std::to_string(var).c_str(), sizeof(char), std::to_string(var).length(), logFile);
        fwrite("\n", sizeof(char), 1, logFile);
        fflush(logFile);
    }
}

template<>
void Logger::logVarible<std::string>(std::string var_name, std::string var) {
    if(!logFile) // Failsafe
        logFile = fopen(file_name.c_str(), file_mode.c_str());

    if (logFile) {
        var_name = "VAR: " + var_name + " = ";
        if (timestamp)
            var_name.insert(0, getTimeStamp_str());
        fwrite(var_name.c_str(), sizeof(char), var_name.length(), logFile);
        fwrite(var.c_str(), sizeof(char), var.length(), logFile);
        fwrite("\n", sizeof(char), 1, logFile);
        fflush(logFile);
    }
}

template<typename T>
void Logger::logArray(std::string array_name, T* array, int array_length) {
    if(!logFile) // Failsafe
        logFile = fopen(file_name.c_str(), file_mode.c_str());

    if (logFile) {
        array_name = "ARRAY: " + array_name + "[" + std::to_string(array_length) + "] = ";
        if (timestamp)
            array_name.insert(0, getTimeStamp_str());
        fwrite(array_name.c_str(), sizeof(char), array_name.length(), logFile);
        fwrite("{", sizeof(char), 1, logFile);
        for (int i = 0; i < array_length; i++) {
            fwrite(std::to_string(array[i]).c_str(), sizeof(char), std::to_string(array[i]).length(), logFile);
            if (i != array_length - 1)
                fwrite(", ", sizeof(char), 2, logFile);
        }
        fwrite("}\n", sizeof(char), 2, logFile);
        fflush(logFile);
    }
}

template<>
void Logger::logArray<std::string>(std::string array_name, std::string* array, int array_length) {
    if(!logFile) // Failsafe
        logFile = fopen(file_name.c_str(), file_mode.c_str());

    if (logFile) {
        array_name = "ARRAY: " + array_name + "[" + std::to_string(array_length) + "] = ";
        if(timestamp)
            array_name.insert(0, getTimeStamp_str());
        fwrite(array_name.c_str(), sizeof(char), array_name.length(), logFile);
        fwrite("{", sizeof(char), 1, logFile);
        for (int i = 0; i < array_length; i++) {
            fwrite(array[i].c_str(), sizeof(char), array[i].length(), logFile);
            if (i != array_length - 1)
                fwrite(", ", sizeof(char), 2, logFile);
        }
        fwrite("}\n", sizeof(char), 2, logFile);
        fflush(logFile);
    }
}

void Logger::readback()
{
    // Need to determine if terminal is read only. If so, the selection process
    // will need to be reworked    

    printf("#############################################\n");
    printf("AutoLog Readback\n");
    printf("---------------------All Log Files---------------------\n");
    
    std::vector<std::string> fileNames;
    std::vector<std::string> data;
    char buff[256];

    FILE* readFileName = fopen(list_file.c_str(), "r");
    while(fgets(buff, 256, readFileName)) // Read file names into vector
        fileNames.push_back(buff);

    for (const auto& fileName : fileNames) {
        printf("%s", fileName.c_str());
    }

    printf("-------------------Log File Readback-------------------\n");
    printf("NOTE: Last file is empty\n\n");

    printf("Enter file name to readback or \"all\" to read back all files:");
    std::string input;
    std::cin >> input;

    if (input == "all")
    {
        for (std::string fileName : fileNames) {

            fileName = fileName.substr(0, fileName.find_first_of("\n"));
            FILE* logOutCopy = fopen(fileName.c_str(), "r");

            printf("\n\n\n#####     BEGIN LOG FILE READBACK: %s     #####\n\n\n", fileName.c_str());
            while (fgets(buff, 256, logOutCopy))
                data.push_back(buff);

            for (const auto& data_value : data) {
                printf("%s", data_value.c_str());
            }
            printf("\n\n\n#####     END LOG FILE READBACK: %s     #####\n\n\n", fileName.c_str());

            fclose(logOutCopy);
            data.clear();
        }
        std::abort();
    }
    else
    {
        FILE* logOutCopy = fopen(input.c_str(), "r");

        printf("\n\n\n#####     BEGINE LOG FILE READBACK: %s     #####\n\n\n", input.c_str());
        while (fgets(buff, 256, logOutCopy))
            data.push_back(buff);

        for (const auto& data_value : data) {
            printf("%s", data_value.c_str());
        }
        printf("\n\n\n#####     END LOG FILE READBACK: %s     #####\n\n\n", input.c_str());

        std::abort();
    }
}