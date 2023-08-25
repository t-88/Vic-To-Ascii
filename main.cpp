#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
// #include <opencv2/opencv.hpp>
// #include <opencv2/imgproc.hpp>
// #include <opencv2/highgui.hpp>



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
    printf("\t -h         : shows help\n");
    printf("\t -img src-path: take img path and prints it to ascii\n");
    printf("\t -vid path out: take video path and saves it to out path\n");
    printf("\t -cap path out: uses opencv to capture ur screen and prints back the output to terminal\n");
}
void unknow_arg() {
    printf("unkown arg given\n");
    print_help();
}

std::vector<char> from_image_to_ascii(const char* path,int& w, int& h) {
    int pitch = 3; // RGB
    uint8_t* img = stbi_load(path,&w,&h,0,pitch);

    if(stbi_failure_reason()) {
        std::cout << "[STBI Err] "<< stbi_failure_reason() << "\n";
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

void print_image_to_terminal(const char* path) {
    int w, h;
    auto img = from_image_to_ascii(path,w,h);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            std::cout << img[x + y * w];
        }
        std::cout << "\n";
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
            } 
            unknow_arg();
            exit(1);
        case 3:
            if(strcmp(argv[1],"-img") == 0) {
                print_image_to_terminal(argv[2]);
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

