# Vic To Ascii
takes ur Vic (video, image , camera capture) removes the colors and slaps some ascii chars to replace it.      


# How To Use 
-   default option is to turn img into ascii.    
    -   ```
        $ ./run.sh
        $ ./main -h
        ```
-   if u want to use capture feature
    -   make sure u have opencv installed or install it **apt install libopencv-dev**
    -   make sure u minimize ur terminal before to see full output, it will seem random when its zoomed in
    -   ```
            $ g++ main.cpp -o main `pkg-config --cflags --libs opencv4
            $ ./main -cap-frame ; read          # capture a frame print it and wait for input
            $ ./main -cap                       # capture stream this will fill ur terminal
        ``` 

# Dev Imgs
## Img To Ascii     
![nyan cat](./dev-imgs/nyan_cat.png)     
## Capture-Frame To Ascii    
![me drining coffee](./dev-imgs/me_drinking_coffe.png)     
## Capture Camera To Ascii    
![me testing](./dev-imgs/me_testing.gif)     
     
