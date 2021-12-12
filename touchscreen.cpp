#include <iostream>
#include <linux/input.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

#include "touchscreen.h"

TouchScreen::TouchScreen(Xepoll *epoll, Interface *interface)
: epoll_(epoll), m_interface_(interface)
{
    char          buf[256] = { 0, };  /* RATS: Use ok */
    int           fd = -1;
    std::string dev = "";
    std::vector<std::string> events;
    getFiles("/dev/input/", events);
    for (auto iter : events) {
        std::cout << "Device name "<< iter << std::endl;
        if ((fd = open(iter.c_str(), O_RDONLY, 0)) >= 0) {
            ioctl(fd, EVIOCGNAME(sizeof(buf)), buf);
            dev = buf;
            std::cout << "Device info "<< dev << std::endl;
            if(dev == "ADS7846 Touchscreen") {
                key_input_fd_ = fd;
                break;
            }
            close(fd);
        }
    }

    init();
}

TouchScreen::~TouchScreen(void)
{
    if (key_input_fd_ > 0) {
        close(key_input_fd_);
    }
}

int TouchScreen::ReadTouchData(void)
{
    struct timeval		time;
    struct input_event	event;
    int ret = read(key_input_fd_, &event, sizeof(struct input_event));
    if(ret > 0) {
        time	= event.time;
        switch(event.type) {
            case EV_SYN:    // 分隔
            break;

            case EV_ABS: {  // 可以读取坐标点了
                switch(event.code) {
                    case ABS_X: // 读取x轴坐标
                        std::cout << RED << "abs_x: " << event.value << "\n";
                    break;

                    case ABS_Y: // 读取y轴坐标
                    {
                        std::cout << GREEN << "abs_y: " << event.value << "\n";
                    }
                    break;

                    case ABS_PRESSURE: // 按下/松开
                    {
                        if(event.value == 1) {
                            // 按下
                        } else {
                            // 松开
                        }
                    } break;

                    default:
                    break;
                }
            }
            break;

            default:
            break;
        }
		// printf( "timeS=%d,timeUS=%d,type=%d,code=%d,value=%d\n", time.tv_sec, time.tv_usec, event.type, event.code, event.value );

        if(0) {
            if(nullptr != m_interface_){
                m_interface_->Transfer(true);
            }
        }
    }
    return ret;
}

bool TouchScreen::init() {
  // 绑定回调函数
  if (key_input_fd_ > 0) {
        std::cout << "Bind epoll" << std::endl;
        epoll_->add(key_input_fd_, std::bind(&TouchScreen::ReadTouchData, this));
  }
  return true;
}

void TouchScreen::getFiles(std::string path, std::vector<std::string>& files)
{
	// check the parameter !
	if( path.empty() ) {
		return;
	}
	// check if dir_name is a valid dir
	struct stat s;
	lstat( path.c_str(), &s );
	if( ! S_ISDIR( s.st_mode ) ) {
		return;
	}

	struct dirent * filename;    // return value for readdir()
	DIR * dir;                   // return value for opendir()
	dir = opendir( path.c_str() );
	if( NULL == dir ) {
		return;
	}

	/* read all the files in the dir ~ */
	while( ( filename = readdir(dir) ) != NULL ) {
		// get rid of "." and ".."
		if( strcmp( filename->d_name , "." ) == 0 ||
			strcmp( filename->d_name , "..") == 0 )
			continue;
        std::string full_path = path + filename->d_name;
        struct stat s;
        lstat( full_path.c_str(), &s );
        if( S_ISDIR( s.st_mode ) ) {
            continue;
        }
        files.push_back(full_path);
	}
}
