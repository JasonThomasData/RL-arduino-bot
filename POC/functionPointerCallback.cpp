// g++ -std=c++14 functionPointerCallback.cpp -o func && ./func

#include <iostream>

int func(double d, double e, int i){
    return static_cast<int>(d + e) + i;
}

int doFun(int(*f)(double, double, int)){
    return f(2.0, 3.0, 1);
}

int main(){
    std::cout<< doFun(&func)<< std::endl;
}
