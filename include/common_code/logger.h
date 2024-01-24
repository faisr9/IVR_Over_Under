#pragma once
#include "main.h"

class Logger {
    private:
        static const std::string list_file;
        FILE* logFile;
        bool appending = false;

    protected:
        std::string file_name;
        std::string file_mode;   
        int file_num; 
        bool timestamp = false;

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
        Logger(std::string file_name, bool overwrite=false, bool append=true, bool timestamp=true);
        ~Logger();

        /**
         * @brief Returns the time since program start in the format of 
         *      minutes:seconds.milliseconds as a formatted string
         * Example: [1:23.456]
         */
        std::string getTimeStamp_str();
        
        /**
         * @brief Logs a message to the log file
         * @param message the message to log
         */
        void logStringMessage(std::string message);

        /**
         * @brief Logs a message to the log file
         * @param message the message to log
         * @param ... the varibles to be inserted into the message
        */
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

        
        /**
         * @brief Reads out the contents of the auto log file to the brain terminal
         *  accessable through the VScode terminal
         * 
         * IMPORTANT: To run this method correctly, you must run the following command in
         *  the pros (integrated) terminal before starting the program:
         * For CMD: pros terminal 1 > <file to copy to>
         *  Example: pros terminal 1 > logOutput.txt
         * For Powershell: pros terminal | Tee-Object <file to copy to>
         *  Example: pros terminal | Tee-Object logOutput.txt
         *  NOTE: The powershell command will echo the output to the terminal as well
         * 
         * IMPORTANT: This method will self terminate after it is done reading the file
         */
        void readback();
};

class AutoLogger : protected Logger{
    private:
        AutoLogger();

        inline static AutoLogger* instance_ = nullptr;
        static std::string auto_log_file_name;
        static const int auto_log_delay; // milliseconds

        bool terminate = false;
        bool paused = false;
        int logNum;

        std::string nextMessage;
        std::string motorData[9][7];
        std::string deviceData[2][3];
        std::vector<std::string> importantVaribles;
        std::vector<std::string> importantMessages;

        // Add mutexes for multithreading

        // To develop
        void motorUpdate();
        void deviceUpdate();
        
    public:
        void autoLogRunner();
        AutoLogger(const AutoLogger& other) = delete;
        ~AutoLogger();

        static AutoLogger* createInstance();
        static AutoLogger* getInstance();

        /**
         * @brief Pauses the auto log
        */
        void pauseAutoLog();

        /**
         * @brief Resumes the auto log
        */
        void resumeAutoLog();
        
        [[deprecated("No need to force stop the autolog, it will stop when the program ends. There for edge cases.")]]
        /**
         * @brief Stops the auto log
         * NOTE: This method is deprecated, the auto log will stop when the program ends.
         *  This method is only used for edge cases
        */
        void stopAutoLog();

        /**
         * @see Logger::logVarible
        */
        template<typename T>
        void logVarible(std::string var_name, T var);

        /**
         * @see Logger::logArray
        */
        template<typename T>
        void logArray(std::string array_name, T* array, int array_length);

        /**
         * @see Logger::logStringMessage
        */
        void logStringMessage(std::string message);

        /**
         * @see Logger::logCharMessage
        */
        void logCharMessage(const char* message, ...);
};
extern AutoLogger* autoLogger; // Global Class Access