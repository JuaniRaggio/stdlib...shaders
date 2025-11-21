# stdlib...shaders

This is a single-file implementation of shaders using only the stdlib. Shaders where initially writen in GLSL [Check it out](https://x.com/XorDev/status/1894123951401378051), but as you can see, you don't need GLSL

![Output](output.gif)

## Dependencies

- ffmpeg
- any cpp compiler for C++20 or above

## How to generate the `.mp4`

1. Compile the source file with any C++ compiler
    ```sh
    c++ -O3 -std=c++20 main.cpp
    # or
    ./build.sh
    ```

2. Run output file
    ```sh
    ./a.out
    ```

3. Generate the video
_There are multiple ways to generate the video, explore ffmpeg, its a great tool_
    ```sh
    cd frames
    ffmpeg -framerate 30 -i out%03d.ppm -c:v libx264 -pix_fmt yuv420p temp.mp4
    ffmpeg -i temp.mp4 -vf reverse reversed.mp4
    ffmpeg -i temp.mp4 -i reversed.mp4 -filter_complex "[0:v][1:v]concat=n=2:v=1[out]" -map "[out]" seamless_loop.mp4
    ```

## Credits

This project is inspired by a tsoding video, _Graphics API is irrelevant_

