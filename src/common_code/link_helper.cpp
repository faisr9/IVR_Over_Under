#include "main.h"

LinkHelper* LinkHelper::instance_ = nullptr;

const std::string LinkHelper::key_ = "ILLINI_IVR_2024";
const size_t LinkHelper::max_msg_size_ = 128;
int LinkHelper::msgRecvTimeout = 3500;

LinkHelper* LinkHelper::createInstance(int linkPort, pros::link_type_e_t boss)
{
    if (!instance_ || instance_ == nullptr)
        instance_ = new LinkHelper(linkPort, boss);
        
    return instance_;    
}

LinkHelper* LinkHelper::getInstance()
{
    if (instance_ == nullptr)
        throw std::runtime_error("LinkHelper: Attempting to getInstance while instance_ is nullptr");

    return instance_;
}

LinkHelper::LinkHelper(int linkPort, pros::link_type_e_t boss) : 
    linkPort_(linkPort), boss_(boss)
{
    hasInit = false;
};

LinkHelper::~LinkHelper() {
    if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }
}

void LinkHelper::init()
{
    if (!hasInit)
        link_ = new pros::Link(this->linkPort_, this->key_, this->boss_, true);

    if (link_ == nullptr)
        throw std::runtime_error("LinkHelper: Unable to initialize link_"); // To replace with non-fatal error
        
    hasInit = true;
}

void LinkHelper::setMsgRecvTimeout(int timeout) { msgRecvTimeout = timeout; }

bool LinkHelper::sendMsg(std::string msg)
{
    if (!hasInit)
        throw std::runtime_error("LinkHelper: Attempting to sendMsg before init()");

    if (this->isLinked() == false)
        return false;
    else {
        if (msg.length() > max_msg_size_)
            throw std::runtime_error("LinkHelper: Message length exceeds max_msg_size_");
        else if (msg.find_first_of("~") != std::string::npos)
            throw std::runtime_error("LinkHelper: Message contains invalid character '~'");

        for (int i = 0; i < max_msg_size_ - msg.length(); i++)
            msg += "~";

        char msg_c[max_msg_size_];
        strcpy(msg_c, msg.c_str());

        size_t sizeSend = sizeof(msg_c);
        uint32_t retrun = link_->transmit(&msg_c, sizeSend);
        lcd::print(5, "sizeSend: %d | retrun: %d", sizeSend, retrun-4);
        return retrun-4 == sizeSend ? true : false; 
    }

    return false;
}

template<typename T>
bool LinkHelper::template_sendData(T var)
{
    static_assert(std::is_arithmetic<T>::value, "LinkHelper: sendData only accepts primitive types (int, float, double, bool)");

    if (!hasInit)
        throw std::runtime_error("LinkHelper: Attempting to sendData before init()");

    if (this->isLinked() == false)
        return false;
    else {
        if (sizeof(var) > max_msg_size_)
            throw std::runtime_error("LinkHelper: Data length exceeds max_msg_size_");

        char data_c[max_msg_size_];
        std::string data_s = std::to_string(var);

        for (int i = 0; i < max_msg_size_ - data_s.length(); i++)
            data_s += "~";

        strcpy(data_c, data_s.c_str());

        size_t sizeSend = sizeof(data_c);
        uint32_t retrun = link_->transmit(&data_c, sizeSend);
        return retrun-4 == sizeSend ? true : false;
    }

    return false;
}

bool LinkHelper::sendData(int var)
{
    if (!hasInit)
        throw std::runtime_error("LinkHelper: Attempting to sendData before init()");

    return this->template_sendData(var);
}

bool LinkHelper::sendData(float var)
{
    if (!hasInit)
        throw std::runtime_error("LinkHelper: Attempting to sendData before init()");

    return this->template_sendData(var);
}

bool LinkHelper::sendData(double var)
{
    if (!hasInit)
        throw std::runtime_error("LinkHelper: Attempting to sendData before init()");

    return this->template_sendData(var);
}

bool LinkHelper::sendData(bool var)
{
    if (!hasInit)
        throw std::runtime_error("LinkHelper: Attempting to sendData before init()");

    return this->template_sendData(var);
}

std::string LinkHelper::recvMsg(bool waitForResponse)
{
    if (!hasInit)
        throw std::runtime_error("LinkHelper: Attempting to recvMsg before init()");

    if (this->isLinked() == false)
        return "~no-link~";
    else {
        char msg_c[max_msg_size_] = {'\0'};
        std::string msg = "";
        size_t sizeRecv = sizeof(msg_c);
        int time_s = pros::millis();
        do {
            link_->receive(&msg_c, sizeRecv);
            delay(15);
        } while (msg_c[0] == '\0' && pros::millis()-time_s < msgRecvTimeout);
        int time_e = pros::millis();
        if (time_e-time_s > msgRecvTimeout)
            return "~timeout~";
        msg = msg_c;

        if (msg.find_first_of("~") != std::string::npos)
            msg = msg.substr(0, msg.find_first_of("~"));

        link_->clear_receive_buf();
        return msg;
    }
}

void LinkHelper::recvData(int &data_out, bool waitForResponse)
{
    if (!hasInit)
        throw std::runtime_error("LinkHelper: Attempting to recvData before init()");

    if (this->isLinked() == false)
        data_out = 0;
    else {
        char data_c[max_msg_size_] = {'\0'};
        std::string data = "";
        size_t sizeRecv = sizeof(data_c);
        int time_s = pros::millis();
        do {
            link_->receive(&data_c, sizeRecv);
            delay(15);
        } while (data_c[0] == '\0' && pros::millis()-time_s < msgRecvTimeout);
        int time_e = pros::millis();
        if (time_e-time_s > msgRecvTimeout)
            { data_out = 0; return; } 

        data = data_c;

        if (data.find_first_of("~") != std::string::npos)
            data = data.substr(0, data.find_first_of("~"));

        std::stringstream ss(data);
        int convertedData;
        ss >> convertedData;
        data_out = (int)convertedData;
        link_->clear_receive_buf();
    }
}

void LinkHelper::recvData(float &data_out, bool waitForResponse)
{
    if (!hasInit)
        throw std::runtime_error("LinkHelper: Attempting to recvData before init()");

    if (this->isLinked() == false)
        data_out = 0;
    else {
        char data_c[max_msg_size_] = {'\0'};
        std::string data = "";
        size_t sizeRecv = sizeof(data_c);
        int time_s = pros::millis();
        do {
            link_->receive(&data_c, sizeRecv);
            delay(15);
        } while (data_c[0] == '\0' && pros::millis()-time_s < msgRecvTimeout);
        int time_e = pros::millis();
        if (time_e-time_s > msgRecvTimeout)
            { data_out = 0; return; }

        data = data_c;

        if (data.find_first_of("~") != std::string::npos)
            data = data.substr(0, data.find_first_of("~"));

        std::stringstream ss(data);
        float convertedData;
        ss >> convertedData;
        data_out = (float)convertedData;
        link_->clear_receive_buf();
    }
}

void LinkHelper::recvData(double &data_out, bool waitForResponse)
{
    if (!hasInit)
        throw std::runtime_error("LinkHelper: Attempting to recvData before init()");

    if (this->isLinked() == false)
        data_out = 0;
    else {
        char data_c[max_msg_size_] = {'\0'};
        std::string data = "";
        size_t sizeRecv = sizeof(data_c);
        int time_s = pros::millis();
        do {
            link_->receive(&data_c, sizeRecv);
            delay(15);
        } while (data_c[0] == '\0' && pros::millis()-time_s < msgRecvTimeout);
        int time_e = pros::millis();
        if (time_e-time_s > msgRecvTimeout)
            { data_out = 0; return; }

        data = data_c;

        if (data.find_first_of("~") != std::string::npos)
            data = data.substr(0, data.find_first_of("~"));

        std::stringstream ss(data);
        double convertedData;
        ss >> convertedData;
        data_out = (double)convertedData;
        link_->clear_receive_buf();
    }
}

void LinkHelper::recvData(bool &data_out, bool waitForResponse)
{
    if (!hasInit)
        throw std::runtime_error("LinkHelper: Attempting to recvData before init()");

    if (this->isLinked() == false)
        data_out = false;
    else {
        char data_c[max_msg_size_] = {'\0'};
        std::string data = "";
        size_t sizeRecv = sizeof(data_c);
        int time_s = pros::millis();
        do {
            link_->receive(&data_c, sizeRecv);
            delay(15);
        } while (data_c[0] == '\0' && pros::millis()-time_s < msgRecvTimeout);
        int time_e = pros::millis();
        if (time_e-time_s > msgRecvTimeout)
            { data_out = false; return; }

        data = data_c;

        if (data.find_first_of("~") != std::string::npos)
            data = data.substr(0, data.find_first_of("~"));

        std::stringstream ss(data);
        bool convertedData;
        ss >> convertedData;
        data_out = (bool)convertedData;
        link_->clear_receive_buf();
    }
}

bool LinkHelper::notify()
{
    if (!hasInit)
        throw std::runtime_error("LinkHelper: Attempting to notify before init()");

    return this->sendMsg("notify-ping");
}

bool LinkHelper::waitForNotify(int maxWaitTime)
{
    if (!hasInit)
        throw std::runtime_error("LinkHelper: Attempting to waitForNotify before init()");


    int time_s = pros::millis();
    waitUntil(this->recvMsg() == "notify-ping" || pros::millis()-time_s > maxWaitTime);
    int time_e = pros::millis();

    return time_e-time_s > maxWaitTime ? false : true;
}

bool LinkHelper::isBoss() { return boss_ == pros::E_LINK_TRANSMITTER ? true : false; }

bool LinkHelper::isLinked()
{
    if (!hasInit)
        throw std::runtime_error("LinkHelper: Attempting to check link stat before init()");

    return link_->connected();
}