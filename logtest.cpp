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

Logger::Logger(std::string file_name, bool overwrite, bool append) {
    file_mode = "a"; // default to append as to not lose logs
    if (!overwrite) {
        if (append) {
            file_mode = "a";
            appending = true;
        }
        else {
            appending = false;
            
            char buff[64];
            vector<std::string> fileNames;
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
                    file_name_number = fileNames[i].substr(fileNames[i].find_first_of("_") + 1,
                        fileNames[i].find_first_of(".") - fileNames[i].find_first_of("_") - 1);

                    file_name_number_int = std::stoi(file_name_number);
                    cout << file_name_number << endl;
                }
                else  
                    file_name_number_int = 0;
                
                if(file_name_number_int > highestFileNumber)
                    highestFileNumber = file_name_number_int;
            }
            cout << highestFileNumber << endl;
        }
    }
    else {
        file_mode = "w";
        appending = false;
    }

    cout << endl << "Stopped" << endl;
    std::abort(); 

    this->file_name = file_name;
    FILE* logFileName = fopen(list_file.c_str(), "a");
    if (logFileName) {
        fwrite(file_name.c_str(), sizeof(char), file_name.length(), logFileName);
        fwrite("\n", sizeof(char), 1, logFileName);
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

void Logger::logStringMessage(std::string message) {
    if(!logFile) // Failsafe
        logFile = fopen(file_name.c_str(), file_mode.c_str());

    if (logFile) {
        message.insert(0, getTimeStamp_str((rand() % 100000)+100));
        
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
        logMessage = getTimeStamp_str((rand() % 100000)+100);

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
        var_name.insert(0, getTimeStamp_str((rand() % 100000)+100));
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
        var_name.insert(0, getTimeStamp_str((rand() % 100000)+100));
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
        array_name.insert(0, getTimeStamp_str((rand() % 100000)+100));
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
        array_name.insert(0, getTimeStamp_str((rand() % 100000)+100));
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

int main()
{
    srand(time(NULL));
    Logger logBuild("usd/logOut_5.txt", false, false);
    logBuild.logStringMessage("Hello World");
    logBuild.logStringMessage("This is cool");
    logBuild.logStringMessage("Rishi is black");
    int x = 5;
    int y = 10;
    double z = 3.14159265358;
    logBuild.logCharMessage("x: %d, y: %d, z: %f", x, y, z);

    int array[5] = {11235, 2563123, 35645, -4564, -455};
    logBuild.logArray("Array", array, 5);

    int varible1 = 5;
    logBuild.logVarible("varible1", varible1);
    double varible2 = 2.17856456156485;
    logBuild.logVarible("varible2", varible2);
    bool varible3 = true;
    logBuild.logVarible("varible3", varible3);
    std::string varible4 = "World Hello!";
    logBuild.logVarible("varible4", varible4);

    double array2[5] = {1.1235, 2.563123, 3.5645, -4.564, -4.55};
    logBuild.logArray("Array2", array2, 5);
    bool array3[5] = {true, false, true, false, true};
    logBuild.logArray("Array3", array3, 5);
    std::string array4[5] = {"Hello", "World", "This", "Is", "Cool"};
    logBuild.logArray("Array4", array4, 5);
    
    
    // logBuild.~Logger();
    return 0;
}