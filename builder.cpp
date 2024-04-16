#include <iostream>
#include <string>
#include <vector>

using namespace std;
struct Motor {
    int port;
    bool spin;
};

class MyClass {
private:
    class Builder;
    std::vector<Motor*> drive_motors;
    double param2_;
    std::string param3_;
public:

    static Builder createBuilder(int inaa);

    void someOtherMethod() {
        // This method can be called on the constructed object
        std::cout << "Some other method called." << std::endl;
    }

    void spin() {
        for (auto motor : drive_motors) {
            motor->spin = true;
            std::cout << "Spinning motor " << motor->port << std::endl;
        }
    }

    void stop() {
        for (auto motor : drive_motors) {
            motor->spin = false;
            std::cout << "Stopping motor " << motor->port << std::endl;
        }
    }

    void changeValue(int motor, int value) {
        drive_motors[motor]->port = value;
        cout << "Changed motor " << motor << " to " << value << endl;
    }

};

class MyClass::Builder {
public:
    Builder& method1(Motor *motorINP) {
        obj.drive_motors.push_back(motorINP);
        return *this;
    }

    Builder& method2(double param2) {
        obj.param2_ = param2;
        return *this;
    }

    Builder& method3(const std::string& param3) {
        obj.param3_ = param3;
        return *this;
    }

    MyClass build() {
        cout << "Creating object with parameters: " << obj.param2_ << ", " << obj.param3_ << endl;
        return obj;
    }
    Builder(int aa) {
        cout << "Builder constructor called " << aa << endl;
        // Private constructor
    }

private:
    MyClass obj;
};

MyClass::Builder MyClass::createBuilder(int inaa) {
    return Builder(inaa);
}

int main() {
    // MyClass::Builder builder = MyClass::createBuilder();
    // MyClass obj = builder.method1(10).method2(3.14).method3("Hello").build();
    Motor motor_1 = {5, false};
    Motor motor_2 = {10, false};
    Motor motor_3 = {15, false};

    MyClass obj = MyClass::createBuilder(44)
        .method1(&motor_1) 
        .method1(&motor_2)
        .method1(&motor_3)
        .method2(3.14)
        .method3("Hello")
        .build();

    // obj.method1(20);  // This would be invalid after object construction
    obj.someOtherMethod(); // This is valid
    obj.spin();
    obj.stop();
    obj.changeValue(0, 20);
    obj.spin();
    obj.stop();

    // unsigned int a = 0b00000010;
    // cout << (int)a << endl;
    // a <<= 2;
    // cout << (int)a << endl;

    return 0;
}
