# Vic To Ascii
takes ur Vic (video, image , camera capture) removes the colors and slaps some ascii chars to replace it.      


# How To Use 
-   if the terminal output doesnt work that means u are using the wrong terminal, this app was tested on alacritty.
-   by default Victa only handles imgs ant its uses stbi to load them.    
    -   ```
        $ ./run.sh
        $ ./Victa -h
        ```

-   if u want to use video features u need to use opencv 
    -   make sure u have opencv installed or install it **apt install libopencv-dev**
    -   make sure u minimize ur terminal before to see full output, it will seem random when its zoomed in
    -   ```
            $ ./run.sh -opencv
            $ ./Victa -h
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
## Video To Ascii    
![bad apple](./dev-imgs/bad_apple.gif)     
     
