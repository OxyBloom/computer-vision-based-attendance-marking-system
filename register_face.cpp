#include <opencv2/opencv.hpp>
#include <iostream>

void registerFace() {
    // cv::VideoCapture cap(0);
    cv::VideoCapture cap(0, cv::CAP_V4L2); 
    if (!cap.isOpened()) {
        std::cerr << "Error: Cannot open the camera!" << std::endl;
        return;
    }

    cv::CascadeClassifier faceCascade;
    if (!faceCascade.load("../data/haarcascades_cuda/haarcascade_frontalface_default.xml")) {
        std::cerr << "Error loading face cascade file!" << std::endl;
        return;  // Prevent further code execution
    }

    std::string name;
    long int rollno;
    std::cout << "Enter student's name: ";

    std::getline(std::cin, name);

    std::cout << "Enter student's roll no: ";
    std::cin>>rollno;

    std::string savePath = "../face_data/" + name + std::to_string(rollno) + ".jpg";

    while (true) {
        cv::Mat frame;
        cap >> frame;

        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(frame, faces, 1.1, 4);

        for (auto &face : faces) {
            cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);
        }

        cv::imshow("Register Face", frame);

        if (cv::waitKey(1) == 's') {
            if (!faces.empty()) {
                cv::Mat faceImg = frame(faces[0]);
                cv::resize(faceImg, faceImg, cv::Size(150, 150));
                cv::imwrite(savePath, faceImg);
                std::cout << "Face registered successfully!" << std::endl;
            }
            break;
        }

        if (cv::waitKey(1) == 'q') break;
    }

    cap.release();
    cv::destroyAllWindows();
}
