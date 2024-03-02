#pragma once
#include "main.h"

/**
 * IMPORTANT: When connecting the radios for both robots, it is important to
 * have the radio that will be used as the link radio on a port that is higher
 * than that of the radio that will be used for the controller. The brain
 * prioritizes the radio on a higher port. Becuase of this, if the link radio
 * is on a lower port than the controller radio, the link radio will
 * be disabled. When the program starts, this second radio doesn't exist according
 * to the brain and wont initialize as a link radio. 
 * 
 * Correct Example:
 * Link Radio - Port 15
 * Controller Radio - Port 5
 * 
 * The link radio will initially serve as the controller radio. When the program starts
 * it will be overrided by the brain, causing the second radio to be enabled.
*/
class LinkHelper {
  private:
    static LinkHelper* instance_;
    static const std::string key_;
    static const size_t max_msg_size_;
    static int msgRecvTimeout;

    int linkPort_;
    pros::link_type_e_t boss_;
    pros::Link* link_ = nullptr;
    bool hasInit;

  public:
    LinkHelper(int linkPort, pros::link_type_e_t boss);
    static LinkHelper* createInstance(int linkPort, pros::link_type_e_t boss);
    static LinkHelper* getInstance();
    ~LinkHelper();

    /**
     * @brief Initialize the link object
     * 
     * NOTE: This is required because the link object will not initialize
     *  if the constructor is called in this class' constructor
     */
    void init();

    /**
     * @brief Set the timeout for receiving messages
     * @param timeout the timeout in milliseconds
     * 
     * NOTE: By default, this is set to 3500 milliseconds
     */
    void setMsgRecvTimeout(int timeout);

    /**
     * @brief Send a custom message to the other robot
     * @param msg the message to send
     *  NOTE: msg must be less than 128 characters
     * 
     * @return true if the message was sent successfully
     */
    bool sendMsg(std::string msg);
    
    /**
     * @brief Receive a custom message from the other robot
     * @param waitForResponse whether or not to pause and wait for a response
     *  NOTE: By default, this is true as there will be expected delay til the
     *  msg is recieved. Do not set this to false unless you know what you are doing.
     *
     * @return the message received, or "~timeout~" if no message was received within
     *  the timeout period
     */
    std::string recvMsg(/*bool waitForResponse=true*/);
    
    /**
     * @brief Send a notification to the other robot
     * @returns true if the notification was sent successfully
     */
    bool notify();

    /**
     * @brief Wait for a notification from the other robot
     * @param maxWaitTime the maximum time to wait for the notification (in milliseconds)
     * 
     * @returns true if the notification was received successfully, false if the timeout
     *  condition was reached
     */
    bool waitForNotify(int maxWaitTime=3500);

    /**
     * @return True if this robot is the boss (aka transmitter)
     */
    bool isBoss();

    /**
     * @return True if this robot is linked to the other robot
     */
    bool isLinked(); 
};  