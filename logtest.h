// #pragma once
#include <iostream>
#include <chrono>
#include <string>
#include <string.h>
#include <vector>
#include <fstream>
#include<iomanip>

class Logger {
    private:
        inline static const std::string list_file = "fileLists.txt";
        FILE* logFile;
        bool appending = false;

    protected:
        std::string file_name;
        std::string file_mode;    

        // std::string getTimeStamp_str();
        FILE* closeLogFile();
        FILE* getLogFile();

    public:
        /**
         * \brief Creates a logger object which logs data to a specified file
         * 
         * \param file_name The name of the file to log to 
         *      NOTE: Keep file extension as .txt
         *      NOTE: Keep file name under 64 characters
         * \param overwrite=false Whether or not to overwrite the file if it already exists. (ignores append parameter)
         * \param append=true Whether or not to append to the file if it already exists
        */
        Logger(std::string file_name, bool overwrite=false, bool append=true);
        ~Logger();

        void logStringMessage(std::string message);
        void logCharMessage(const char* message, ...);

        /**
         * @brief Logs the value of a variable to the log file
         * @tparam T a type that can be converted to a string (i.e. int, float, double, etc.)
         *  NOTE: Overloads for a std::string varible is provided
         * @param var_name the name of the varible
         * @param var the variable to log
         */
        template<typename T>
        void logVarible(std::string var_name, T var);

        /**
         * @brief Logs the contents of an array to the log file as a comma seperated list
         * @tparam T a type that can be converted to a string (i.e. int, float, double, etc.)
         *  NOTE: Overloads for a std::string array is provided
         * @param array_name the name of the array
         * @param array the array to log
         * @param array_length the length of the array
         */
        template<typename T>
        void logArray(std::string array_name, T* array, int array_length);
};

class AutoLogger : protected Logger{
    private:
        Logger config_Log;
        AutoLogger() : Logger(auto_log_file_name, false, true), config_Log(auto_log_config_info, false, true) {}
        inline static AutoLogger* instance_ = nullptr;
        inline static std::string auto_log_file_name = "usd/autoLog_data.txt";
        inline static std::string auto_log_config_info = "usd/autoLog_config_info.txt";
        inline static const int auto_log_delay = 50; // milliseconds

        void autoLogRunner();

    public:
        AutoLogger(const AutoLogger& other) = delete;
        ~AutoLogger();

        static AutoLogger* createInstance();
        static AutoLogger* getInstance();

        void startAutoLog();
        void pauseAutoLog();
        void resumeAutoLog();
        void stopAutoLog();
        void logCustomStringMessage(std::string message);
        void logCustomCharMessage(const char* message, ...);
};
extern AutoLogger* autoLogger; // Global Class Access

