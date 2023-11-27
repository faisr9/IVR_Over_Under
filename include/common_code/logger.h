#pragma once
#include "main.h"

class AutoLogger {
    private:
        AutoLogger();
        inline static AutoLogger* instance_ = nullptr;
        inline static std::string auto_log_file_name = "auto_log.txt";
        inline static const int auto_log_delay = 50; // milliseconds

    public:
        AutoLogger(const Logger& other) = delete;
        ~AutoLogger();

        static AutoLogger* createInstance();
        static AutoLogger* getInstance();

        void startAutoLog();
        void pauseAutoLog();
        void resumeAutoLog();
        void stopAutoLog();
        void logCustom(std::string message);
};
extern AutoLogger* autoLogger; // Global Class Access

class Logger {
    private:
        std::string file_mode;
        bool log_time;

    public:
        /**
         * \brief Creates a logger object which logs data to a specified file
         * 
         * \param file_name The name of the file to log to
         * \param overwrite Whether or not to overwrite the file if it already exists. (ignores append parameter)
         * \param append Whether or not to append to the file if it already exists
         * \param log_time Whether or not to log the time of each log message
        */
        Logger(std::string file_name, bool overwrite=false, bool append=false, bool log_time=true);
        ~Logger();

        void logMessage(std::string message);
        void logMessage(const char* message);
        void logVariable(std::string variable_name, int variable_value);
        void logVariable(std::string variable_name, double variable_value);
        void logVariable(std::string variable_name, bool variable_value);
        void logArray(std::string array_name, int* array, int array_length);
};

