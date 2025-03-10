#include <opencv2/opencv.hpp>
#include <iostream>
#include <dlib/opencv.h>
#include <dlib/dnn.h>
#include <dlib/image_processing.h>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <map>
#include <set>
#include "face_recognition.h"


// Dlib's face recognition model
auto close_flag = false;
using namespace dlib;

std::map<std::pair<std::string, std::string>, std::set<std::string>> attendanceData;


std::string currentTimestamp;
std::string sessionDateTime;

std::set<std::pair<std::string, std::string>> markedStudents;
std::map<std::string, std::string> nameToRollMap;


void saveAttendanceToFile() {
    std::ofstream file("../attendance.csv", std::ios::out | std::ios::app);
    if (!file) {
        std::cerr << "Error opening attendance file!" << std::endl;
        return;
    }

    // Write the header if file is empty
    if (attendanceData.empty()) return;

    file << "Roll No";
    file << ", "<< sessionDateTime << "\n";

    // Write each student's attendance
    for (const auto& [key, timestamps] : attendanceData) {
        const std::string& name = key.first;
        const std::string& rollno = key.second;

        file<< rollno;
        file << "," << (timestamps.count(sessionDateTime) ? "1" : "0");

        file << "\n";
    }

    file.close();
}

void startAttendance() {
    close_flag = false;
    markedStudents.clear(); // Reset marked students for each session
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");
    sessionDateTime = ss.str();

    // cv::VideoCapture cap(0);
    cv::VideoCapture cap(0, cv::CAP_V4L2);  // For Linux systems (common for webcams)
    if (!cap.isOpened()) {
        std::cerr << "Error: Cannot open the camera!" << std::endl;
        return;
    }

    cv::CascadeClassifier faceCascade;
    if (!faceCascade.load("../data/haarcascades_cuda/haarcascade_frontalface_default.xml")) {
        std::cerr << "Error loading face cascade file!" << std::endl;
        return;  // Prevent further code execution
    }

    // Load known faces
    std::vector<matrix<rgb_pixel>> known_faces;
    std::vector<std::string> known_names;
    std::vector<std::string> known_rollnos;
    int i=0;
    for (const auto &entry : std::filesystem::directory_iterator("../face_data")) {
        
        cv::Mat img = cv::imread(entry.path().string());

        cv::resize(img, img, cv::Size(150, 150));  // Resize to 150x150
        matrix<rgb_pixel> dlib_img;
        assign_image(dlib_img, cv_image<rgb_pixel>(img));

        known_faces.push_back(dlib_img);
        // known_names.push_back(entry.path().stem().string());
        std::string filename = entry.path().stem().string();
        // Extract name and roll number from filename
        size_t pos = filename.find_last_not_of("0123456789");
        
        if (pos != std::string::npos) {
            std::string name = filename.substr(0, pos + 1);   // Extract name
            std::string rollno = filename.substr(pos + 1);     // Extract roll number

            std::cout << "Name: " << name << ", Roll No: " << rollno << std::endl;

            known_names.push_back(name);   // Store only the name if that's the intended purpose
            // Optionally, if you want to track roll numbers too, store them in a separate vector
            known_rollnos.push_back(rollno);
            
            nameToRollMap[known_names[i]] = known_rollnos[i];
        }
        ++i;
    }

    i=0;

    

    while (true) {
        cv::Mat frame;
        cap >> frame;

        std::vector<cv::Rect> faces;

        faceCascade.detectMultiScale(frame, faces, 1.1, 4);

        for (auto &face : faces) {
            cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);
            cv::Mat faceImg = frame(face);
            cv::resize(faceImg, faceImg, cv::Size(150, 150));

            matrix<rgb_pixel> dlib_face;
            assign_image(dlib_face, cv_image<rgb_pixel>(faceImg));

            std::string detected_name = recognizeFace(dlib_face, known_faces, known_names);
            std::string detected_rollno = nameToRollMap[detected_name];

            if (detected_name != "Unknown") {
                cv::putText(frame, detected_name, cv::Point(face.x, face.y - 10),
                            cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(0, 255, 0), 2);
                if (markedStudents.find({detected_name, detected_rollno}) == markedStudents.end()) {
                    attendanceData[{detected_name, detected_rollno}].insert(sessionDateTime);
                    markedStudents.insert({detected_name, detected_rollno});
                }

            } else {
                cv::putText(frame, "Unknown", cv::Point(face.x, face.y - 10),
                            cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(0, 0, 255), 2);
            }

            
        }

        cv::imshow("Attendance System", frame);
        if (cv::waitKey(1) == 'q') break;
        if(close_flag) break;
    }

    cap.release();
    cv::destroyAllWindows();
}

void stopAttendance() {
    std::cout << "Attendance marking stopped!" << std::endl;
    saveAttendanceToFile();
    cv::destroyAllWindows();
    close_flag = true;

}
