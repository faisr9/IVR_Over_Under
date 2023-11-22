#pragma once
#include "main.h"

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
};

