#include "VideoResolution.h"
#include <iostream>


int main() {
    auto res = getResolution("hello_1024x768.mp4");
    std::cout << res.first << ' ' << res.second << std::endl;
//    auto res = getResolution("D:/zhirik.mp4");
//    std::cout << res1.first << ' ' << res1.second;

    return 0;
}
