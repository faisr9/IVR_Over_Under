#pragma once
#include "main.h"

class AutoLogger {
    private:
        AutoLogger();
        inline static AutoLogger* instance_ = nullptr;
        inline static std::string auto_log_file_name = "auto_log.txt";
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
        void logCustom(std::string message);
};
extern AutoLogger* autoLogger; // Global Class Access

class Logger {
    private:
        inline static const std::string list_file = "fileLists.txt";
        FILE* logFile;
        std::string file_name;
        std::string file_mode;
        bool isFileOpen = false;    
        void changeFileMode(std::string new_mode);
        bool appending;
    
    public:
        /**
         * \brief Creates a logger object which logs data to a specified file
         * 
         * \param file_name The name of the file to log to
         * \param overwrite Whether or not to overwrite the file if it already exists. (ignores append parameter)
         * \param append Whether or not to append to the file if it already exists
        */
        Logger(std::string file_name, bool overwrite=false, bool append=false);
        ~Logger();

        void logMessage(std::string message);
        void logMessage(const char* message, ...);
        void logVariable(std::string variable_name, int variable_value);
        void logVariable(std::string variable_name, double variable_value);
        void logVariable(std::string variable_name, bool variable_value);
        void logArray(std::string array_name, int* array, int array_length);
};

