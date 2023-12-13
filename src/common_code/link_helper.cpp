#include "main.h"

LinkHelper::LinkHelper(short int linkPort, std::string key, bool boss) : linkPort_(linkPort), key_(key), boss_(boss),
    link_(linkPort, key, boss ? pros::link_type_e_t::E_LINK_TRANSMITTER : pros::link_type_e_t::E_LINK_RECIEVER) 
{};

LinkHelper::~LinkHelper() {};

void LinkHelper::sendMsg(std::string msg)
{
    return;
}

template <typename T>
void LinkHelper::sendData(T data)
{
    return;
}

std::string LinkHelper::recvMsg(bool waitForResponse)
{
    return "";
}

bool LinkHelper::notify()
{
    return true;
}

bool LinkHelper::waitForNotify()
{
    return true;
}

bool LinkHelper::isBoss()
{
    return true;
}

bool LinkHelper::isSlave()
{
    return true;
}

bool LinkHelper::isLinked()
{
    return true;
}

