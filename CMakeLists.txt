cmake_minimum_required(VERSION 3.10)
project(AttendanceSystem)

add_subdirectory(../../dlib build) # <--- Change me accordingly!!!
# add_subdirectory(/path/to/dlib/build)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(dlib REQUIRED)

install(
  DIRECTORY
  data
  face_data
  DESTINATION
    share/${PROJECT_NAME}/
)

include_directories(${CMAKE_SOURCE_DIR})  # <--- Add this line

add_executable(attendance_system main.cpp register_face.cpp attendance_tracker.cpp face_recognition.cpp)

target_link_libraries(attendance_system
    Qt5::Widgets
    ${OpenCV_LIBS}
    dlib::dlib
)
