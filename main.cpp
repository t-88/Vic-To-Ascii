#include <stdio.h>
#include <cmath>

#include <iostream>
#include <vector>
#include <string>


#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>



#define STB_IMAGE_IMPLEMENTATION
#include "third_party/stb_image.h"


static std::string shadding = " .:-=+*#%@";
static int char_count; 
static int char_step; 


// i am not about to include algorithm for clamp function
int clamp(int val, int min , int max) {
    if(val < min)
        val = min;
    if(val > max)
        val = max;

    return val;
}



void print_help() {
    printf("Vic To Ascii help:\n");
    printf("\t -h            : shows help\n");
    printf("\t -img src-path : take img path and prints it to terminal\n");
    printf("\t -cap-frame    : capture frame from ur camera and prints it to terminal\n");
    printf("\t -cap          : capture camera stream and prints it to terminal\n");
    printf("\t -vid path out : take video path and saves it to out path\n");
}
void unknow_arg() {
    printf("unkown arg given\n");
    print_help();
}

void print_vec_to_terminal(std::vector<char> img, int w, int h) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            std::cout << img[x + y * w];
        }
        std::cout << "\n";
    }
}

std::vector<char> from_image_to_ascii(const char* path,int& w, int& h) {
    int pitch = 3; // RGB
    uint8_t* img = stbi_load(path,&w,&h,0,pitch);

    if(stbi_failure_reason()) {
        std::cout << "[stbi Err] "<< stbi_failure_reason() << "\n";
        exit(1);
    }

    std::vector<char> out;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int shade = (img[pitch * (x + y * w)+ 0]    +
                        img[pitch *  (x + y * w) + 1] +
                        img[pitch *  (x + y * w) + 2]) / 3;

            out.push_back(
                shadding.at(clamp(shade / char_step,0,char_count - 1))
            );
        }
    }
    return out;
}  
void print_img_to_terminal(const char* path) {
    int w, h;
    auto img = from_image_to_ascii(path,w,h);
    print_vec_to_terminal(img,w,h);
}


std::vector<char> from_capture_to_ascii(int& w, int& h,cv::VideoCapture* cap = 0) {
    // if no cap is provided then its a one frame capture
    // else its a stream capture
    // its build like that so i dont have to create a cap obj every-frame
    
    bool one_frame = cap == 0;

    if(one_frame) {
        cap = new cv::VideoCapture(0);
        if(!(*cap).isOpened()) {
            printf("[opencv Err] capture device could not be opend\n");
            delete cap;
            exit(1);
        }
    }

    cv::Mat frame;
    *cap >> frame;

    w = frame.cols;
    h = frame.rows;

    // flip y-axis my cam capture returns reversed for some reason
    // obs also gives same problem
    // obs hire me i can fixed u 
    cv::flip(frame,frame,1);

    std::vector<char> out;
    for (int y = 0; y < frame.rows; y++) {
        for (int x = 0; x < frame.cols; x++) {
            cv::Vec3b color = frame.at<cv::Vec3b>(y,x);
            int shade = (color[0] + color[1] + color[2]) / 3;

            color[0] = color[1] = color[2] = shade; 
            frame.at<cv::Vec3b>(y,x) = color;


            out.push_back(
                shadding.at(clamp(shade / char_step,0,char_count - 1))
            );
        }
    }

    if(one_frame) {
        delete cap;
    }
    return out;
}
std::vector<char> from_capture_once_to_ascii(int& w, int& h) {
    // capture_once captures one frame and returns it as ascii
    return from_capture_to_ascii(w,h,0);
}
void print_capture_to_terminal(bool stream = false) {
    //NOTE: if stream == true terminal screen will be cleaned at least for mint os linux users  
    //NOTE: if stream == true there is a while loop to exist ur can break by pressing ctr-c 


    // wraper around from_capture_to_ascii
    // print one frame by default
    // if stream is true it runs a while loop and prints capture frame by frame to terminal

    int w, h;
    if(!stream) {
        auto img = from_capture_once_to_ascii(w,h);
        print_vec_to_terminal(img,w,h);
        return;
    }

    cv::VideoCapture cap(0);
    for (;;) {
        auto img = from_capture_to_ascii(w,h,&cap);

        // clear screen
        std::cout << "\033[H\033[2J";
        print_vec_to_terminal(img,w,h);

        // exit loop
        if(cv::waitKey(33) == 27) break;
    }
}

int main(int argc, char** argv) {
    char_count = shadding.size();
    char_step = 255 / char_count;


    switch (argc)
    {
        case 1:
            printf("no enough args\n");
            print_help();
            exit(1);
        break;
        case 2:
            if(strcmp(argv[1],"-h")  == 0) {
                print_help();
                break;
            } else if (strcmp(argv[1],"-cap-frame")  == 0) {
                print_capture_to_terminal();
                break;
            } else if (strcmp(argv[1],"-cap")  == 0) {
                print_capture_to_terminal(true);
                break;
            }
            
            
            unknow_arg();
            exit(1);
        case 3:
            if(strcmp(argv[1],"-img") == 0) {
                print_img_to_terminal(argv[2]);
                break;
            } else {
                printf("more args are required\n");
                print_help();
                exit(1);
            } 
        break;
        default:
            printf("too many args\n");
            print_help();
            exit(1);
            break;
    }

    return 0;

}