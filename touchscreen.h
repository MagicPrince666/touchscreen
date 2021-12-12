#pragma once

#include <vector>
#include "xepoll.h"
#include "interface.h"

class TouchScreen
{
public:
    TouchScreen(Xepoll *epoll, Interface *interface);
    ~TouchScreen();

    bool init();
    int ReadTouchData();

private:
    Xepoll *epoll_;
    int key_input_fd_{-1};
    Interface *m_interface_;
    void getFiles(std::string path, std::vector<std::string>& files);
};
