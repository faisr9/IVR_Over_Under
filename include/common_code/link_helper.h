#pragma once
#include "main.h"


class LinkHelper {
  private:
    short int linkPort_;
    bool boss_;
    std::string key_;
    pros::Link link_;

  public:
    LinkHelper(short int linkPort, std::string key, bool boss) : linkPort_(linkPort), key_(key), boss_(boss),
        link_(linkPort, key, boss ? pros::link_type_e_t::E_LINK_TRANSMITTER : pros::link_type_e_t::E_LINK_RECIEVER) {};
    ~LinkHelper();

    void sendMsg(std::string msg);
    template <typename T>
        void sendData(T data);
    std::string recvMsg(bool waitForResponse=false);
    
    bool notify();
    bool waitForNotify();
    bool isBoss();
    bool isSlave();
    bool isLinked();
};