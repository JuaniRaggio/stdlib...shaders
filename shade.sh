cd ./frames/
ffmpeg -i output%d.ppm -r 60 vid.mp4 | echo

echo "Shaders generated successfully"
