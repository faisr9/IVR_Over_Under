// The auto logger will log all the vital data like device connections and important initiation events.

// There needs to be a master file that stores the name of every log file created when reading to the computer 

#include "main.h"

 /**
  * @brief Creates the single instance of the class. This is done here to ensure
  * the object is created before main() is called allowing the autoLogger to function
  * properly.
  */
AutoLogger* autoLogger = AutoLogger::createInstance();

AutoLogger::AutoLogger() {
    FILE* autoLogWrite = fopen(auto_log_file_name.c_str(), "a");
    const char* break_message = "---------------------------\nStart of new log\n---------------------------\n";
    if (autoLogWrite)
        fwrite(break_message, sizeof(char), strlen(break_message), autoLogWrite);

}
AutoLogger::~AutoLogger() {
    if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }
}

AutoLogger* AutoLogger::createInstance() {
    if (!instance_)
        instance_ = new AutoLogger();

    return instance_;
}

AutoLogger* AutoLogger::getInstance() {
    if (instance_ == nullptr)
        throw std::runtime_error("AutoLogger: Attempting to getInstance while instance_ is nullptr");

    return instance_;
}

Logger::Logger(std::string file_name, bool overwrite, bool append, bool log_time) {
    this->log_time = log_time;
    file_mode = "w"; // default to overwrite

    if (!overwrite) {
        if (append) {
            file_mode = "a";
            
            // Create a break between the old and new log
            const char* break_message = "---------------------------\nStart of new log\n---------------------------\n";
            FILE* file = fopen(file_name.c_str(), file_mode.c_str());
            if (file)
                fwrite(break_message, sizeof(char), strlen(break_message), file);
                
            fclose(file);
        }
        else {
            if (file_name.find_first_of("_") == std::string::npos) {
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

    FILE* file = fopen(file_name.c_str(), file_mode.c_str());
}