#include "include/common_code/Geometry/vector2d.h"
#include "include/common_code/Geometry/angle2d.h"
#include <iostream>

int main(){
    Vector2d a = Vector2d(1,2);
    Vector2d b = Vector2d(3,4);

    std::cout << a.plus(b).toString() << std::endl;
}


