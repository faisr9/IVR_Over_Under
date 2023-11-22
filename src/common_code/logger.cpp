#include "main.h"

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