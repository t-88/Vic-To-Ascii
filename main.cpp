
#include <stdio.h>
#include <cmath>
#include <assert.h>

#include <iostream>
#include <vector>
#include <map>
#include <string>



#define STB_TRUETYPE_IMPLEMENTATION
#include "third_party/stb_truetype.h"

#define STB_IMAGE_IMPLEMENTATION
#include "third_party/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "third_party/stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "third_party/stb_image_resize.h"

#ifdef VID 
    #include <opencv2/opencv.hpp>
    #include <opencv2/imgproc.hpp>
    #include <opencv2/highgui.hpp>
#endif



int clamp(int val, int min , int max);
void print_help();
void unknow_arg();

void print_vec_to_terminal(std::vector<char> img, int w, int h);
void print_img_to_terminal(const char* path);

#ifdef VID 
    std::vector<char> from_capture_to_ascii(int* w, int* h,cv::VideoCapture* cap);
    std::vector<char> from_capture_once_to_ascii(int* w, int* h);
    void print_capture_to_terminal(bool stream);
#endif


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
    printf("\t -img src-path : loads img and prints it to terminal\n");
    printf("\t -save-img src-path out-path: loads img and saves it to out-path\n");
#ifdef VID 
    printf("\t -vid-frame path frame-idx: prints a frame from video to terminal\n");
    printf("\t -vid path : take video path and saves it to out path\n");
    printf("\t -cap-frame    : capture frame from ur camera and prints it to terminal\n");
    printf("\t -cap          : capture camera stream and prints it to terminal\n");
#endif
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
std::vector<char> from_image_data_to_ascii(uint8_t* img,int w, int h) { 
    int pitch = 3; // RGB

    std::vector<char> out;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int shade = (img[pitch * (x + y * (w))+ 0]  +
                        img[pitch *  (x + y * (w)) + 1] +
                        img[pitch *  (x + y * (w)) + 2]) / 3;

            out.push_back(
                shadding.at(clamp(shade / char_step,0,char_count - 1))
            );
        }
    }
    return out;
}

std::vector<char> from_image_to_ascii(const char* path,int* w, int* h) {
    int pitch = 3; // RGB
    uint8_t* img = stbi_load(path,w,h,0,pitch);

    if(stbi_failure_reason()) {
        std::cout << "[stbi Err] "<< stbi_failure_reason() << "\n";
        exit(1);
    }

    return from_image_data_to_ascii(img,*w,*h);
}  
void print_img_to_terminal(const char* path) {
    int w, h;
    auto img = from_image_to_ascii(path,&w,&h);
    print_vec_to_terminal(img,w,h);
}

#ifdef VID 
std::vector<char> from_capture_to_ascii(int* w, int* h,cv::VideoCapture* cap = 0) {
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

    *w = frame.cols;
    *h = frame.rows;

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
std::vector<char> from_capture_once_to_ascii(int* w, int* h) {
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
        auto img = from_capture_once_to_ascii(&w,&h);
        print_vec_to_terminal(img,w,h);
        return;
    }

    cv::VideoCapture cap(0);
    for (;;) {
        auto img = from_capture_to_ascii(&w,&h,&cap);

        // clear screen
        std::cout << "\033[H\033[2J";
        print_vec_to_terminal(img,w,h);

    }
}

std::vector<char> from_video_frame_to_ascii(const char* path,int frame_idx,int* w, int* h) {
    cv::VideoCapture cap(path);
    if (!cap.isOpened()) {
        printf("[opencv Err] opencv isnt able to open the file\n");
        exit(1);
    }   
    cap.set(cv::CAP_PROP_POS_FRAMES,frame_idx);
    auto img = from_capture_to_ascii(w,h,&cap);   
    cap.release();
    
    return img;
}

std::vector<char> from_video_to_ascii(cv::VideoCapture* cap,int* w, int* h) {
    return from_capture_to_ascii(w,h,cap);   
    
}
void print_video_frame_to_terminal(const char* path, int frame_idx) {
    int w,  h;
    auto img = from_video_frame_to_ascii(path,frame_idx,&w,&h);

    print_vec_to_terminal(img,w,h);
}
void print_video_to_terminal(const char* path) {
    cv::VideoCapture cap(path);

    if (!cap.isOpened()) {
        printf("[opencv Err] opencv isnt able to open the file\n");
        exit(1);
    } 
    cap.set(cv::CAP_PROP_POS_FRAMES,0);

    cv::Mat _;
    int w,  h;
    int cur = 0;
    while (cap.retrieve(_,cur++)) {
        auto img = from_video_to_ascii(&cap,&w,&h);

        // clear screen
        std::cout << "\033[H\033[2J";
        print_vec_to_terminal(img,w,h);
    }
    cap.release();
}
#endif



void load_font_from_file(const char* path,stbtt_fontinfo* font_info) {
    // NOTE: dont forget to free font_info.data
    FILE* f = fopen("font/Roboto/RobotoMono-Regular.ttf","r");
    if(!f) {
        printf("[file Err] could not load font into memory\n");
        exit(1);
    }
    fseek(f,0,SEEK_END);
    long font_file_size = ftell(f);
    fseek(f,0,SEEK_SET);
    
    font_info->data = (uint8_t*) malloc(font_file_size);
    fread(font_info->data,font_file_size,1,f);
    fclose(f);

    stbtt_InitFont(font_info, font_info->data, stbtt_GetFontOffsetForIndex(font_info->data,0));
}

void resize_down_stb_img(uint8_t* img, int* w , int* h, double scale_x,double scale_y) {
    // NOTE: bc i am using the same img as input and output to resize function 
    // i do resize down only

    assert((scale_x <= 1 && scale_y <= 1) && "resize_down_stb_img only scales down");
    
    
    stbir_resize_uint8(img,*w,*h,0, img,*w * scale_x,*h * scale_y,0,3 );

    *w = *w * scale_x;
    *h = *h * scale_y;
}

typedef struct CharFontData {
    uint8_t* bitmap;
    int w;
    int h;
} CharFontData;


void write_ascii_to_png(std::vector<char> ascii_img,int w,int h,const char* out_path = "out.png",int font_size = 10,const char* font_path = "font/Roboto/RobotoMono-Regular.ttf") {
    // Note: its better to use monospace font bc the size is kinda constant
    stbtt_fontinfo font_info;
    load_font_from_file(font_path,&font_info);
    
    // get max w and h that shadding chars takes
    // pre-calc the fonts
    std::map<char,CharFontData> shadding_font;
    int font_w = -1; 
    int font_h = -1; 
    for (int i = 0; i < shadding.size(); i++)  {
        int index = stbtt_FindGlyphIndex(&font_info,shadding.at(i));
    
        int char_w , char_h;
        shadding_font[shadding.at(i)].bitmap =  stbtt_GetGlyphBitmap(&font_info, 
                                                 0, 
                                                 stbtt_ScaleForPixelHeight(&font_info,font_size), 
                                                 index, 
                                                 &char_w, 
                                                 &char_h, 
                                                 0, 
                                                 0 
                                                );    
        shadding_font[shadding.at(i)].w = char_w;
        shadding_font[shadding.at(i)].h = char_h;

        if(char_w > font_w) { font_w = char_w; }     
        if(char_h > font_h) { font_h = char_h; }     
    }

    // alloc img
    int output_img_w = w * font_w;
    int output_img_h = h * font_h;
    uint8_t* output_img = (uint8_t*) calloc(1,output_img_w * output_img_h * 3);

    // write data to img
    int y = 0;
    while(y < h) {
        int x = 0;
        while(x < w) {
            auto char_data = shadding_font[ascii_img[y * w + x]];

            for(int fy = 0; fy < char_data.h; fy++) {
                for(int fx = 0; fx < char_data.w; fx++) {
                    if(((y * font_h + fy) * output_img_w + (x * font_w  + fx)) + 2 < output_img_w * output_img_h) {
                        int color = char_data.bitmap[fy * char_data.w + fx];
                        if (color != 0) {
                            output_img[3 * ((y * font_h + fy) * output_img_w + (x * font_w  + fx)) + 0] = color;
                            output_img[3 * ((y * font_h + fy) * output_img_w + (x * font_w  + fx)) + 1] = color;
                            output_img[3 * ((y * font_h + fy) * output_img_w + (x * font_w  + fx)) + 2] = color;
                        }
                    }
                }
            }              
            x++;
        }
        y++;
    }
  
    stbi_write_png(out_path, output_img_w, output_img_h, 3,  output_img, 0);

    free(font_info.data);
    free(output_img);
}

void write_ascii_img_disk(const char* img_path,const char* out_path) {
    int w, h;
    uint8_t* img = stbi_load(img_path,&w,&h,0,3);
    auto ascii_img = from_image_data_to_ascii(img,w,h);
    write_ascii_to_png(ascii_img,w,h,out_path);
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
#ifdef VID 
            else if (strcmp(argv[1],"-cap-frame")  == 0) {
                print_capture_to_terminal();
                break;
            } else if (strcmp(argv[1],"-cap")  == 0) {
                print_capture_to_terminal(true);
                break;
            }
#endif

            unknow_arg();
            exit(1);
        case 3:
            if(strcmp(argv[1],"-img") == 0) {
                print_img_to_terminal(argv[2]);
            } 
#ifdef VID 
            else if(strcmp(argv[1],"-vid") == 0) {
                print_video_to_terminal(argv[2]);
            } 
#endif
            else {
                printf("more args are required\n");
                print_help();
                exit(1);
            } 
        break;
        case 4:
            if(strcmp(argv[1],"-save-img") == 0) {
                write_ascii_img_disk(argv[2],argv[3]);
                break;
            }
#ifdef VID 

            else if(strcmp(argv[1],"-vid-frame") == 0) { 
                print_video_frame_to_terminal(argv[2],atoi(argv[3]));
                break;
            }
#endif
            unknow_arg();
            exit(1);
        default:
            printf("too many args\n");
            print_help();
            exit(1);
            break;
    }

    return 0;

}