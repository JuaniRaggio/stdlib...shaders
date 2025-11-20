cd ./frames/
ffmpeg -i output%d.ppm -r 60 vid.mp4 | echo

cp ./frames/vid.mp4 .

echo "Shaders generated successfully"
