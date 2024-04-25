#include "common_code/link_helper.h"

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
    if (link_ != nullptr) {
        delete link_;
        link_ = nullptr;
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

        // Pad message with '~' to max_msg_size_ since link only receives messages of max_msg_size_
        for (int i = 0; i < max_msg_size_ - msg.length(); i++)
            msg += "~";

        char msg_c[max_msg_size_];
        strcpy(msg_c, msg.c_str());

        size_t sizeSend = sizeof(msg_c);
        uint32_t sizeReturned = link_->transmit(&msg_c, sizeSend);
        return ((sizeReturned-4) == sizeSend) ? true : false; 
    }

    return false;
}

                                // Will implement later (for all recieve functions)
std::string LinkHelper::recvMsg( /*bool waitForResponse*/)
{
    if (!hasInit)
        throw std::runtime_error("LinkHelper: Attempting to recvMsg before init()");

    if (this->isLinked() == false)
        return "~no-link~";
    else {
        // Prefill message with '\0' to check if message was received
        char msg_c[max_msg_size_] = {'\0'};
        std::string msg = "";
        size_t sizeRecv = sizeof(msg_c);

        // Wait for message to be received or for timeout
        int time_s = pros::millis();
        do {
            link_->receive(&msg_c, sizeRecv);
            pros::delay(15);
        } while (msg_c[0] == '\0' && pros::millis()-time_s < msgRecvTimeout);
        int time_e = pros::millis();

        // If timeout, return timeout message
        if (time_e-time_s > msgRecvTimeout)
            return "~timeout~";

        msg = msg_c;

        // Remove padding from message
        if (msg.find_first_of("~") != std::string::npos)
            msg = msg.substr(0, msg.find_first_of("~"));

        // Clear link buffer for next message
        link_->clear_receive_buf();
        return msg;
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