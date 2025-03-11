# computer-vision-based-attendance-marking-system
A C++ based attendance making system that uses basic GUI and computer vision

# System Setup
OS: Ubuntu 22.04

(Feel free to make a windows compactible version if I haven't by the time you find this repository)

# Dependencies
DLIB - For Face detection

OPENCV

QT

## Note:
Preferably clone this to a seperate repository and follow the build instructions

You will have to modify the CmakeLists.txt file of the project to include the directory of the dlib library
```
git clone https://github.com/davisking/dlib.git
```
Install other dependencies with sudo apt
```
sudo apt-get install libopencv-dev
sudo apt-get install libdlib-dev
sudo apt-get install qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools
```
# Usage
### To use
Inside the project directory do
```
cd build
./attendance_system
```
### You might need to do this if it doesn't work
Inside the project directory do
```
rm -rf build
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

You are always Welcome to contribute or reach out incase you face issues