#include <iostream>
#include <string>

using namespace std;

class MyClass {
private:
    class Builder;
    // MyClass() {
    //     // Private constructor
    // }

    int param1_;
    double param2_;
    std::string param3_;
public:

    static Builder createBuilder(int inaa);

    void someOtherMethod() {
        // This method can be called on the constructed object
        std::cout << "Some other method called." << std::endl;
    }

};

class MyClass::Builder {
public:
    Builder& method1(int param1) {
        obj.param1_ = param1;
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
        cout << "Creating object with parameters: " << obj.param1_ << ", " << obj.param2_ << ", " << obj.param3_ << endl;
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

    MyClass obj = MyClass::createBuilder(44)
        .method1(10)
        .method2(3.14)
        .method3("Hello")
        .build();

    // obj.method1(20);  // This would be invalid after object construction
    obj.someOtherMethod(); // This is valid

    return 0;
}
