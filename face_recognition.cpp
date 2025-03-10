#include "face_recognition.h"


anet_type net;  // Definition of `net` — only defined here.

void loadKnownFaces(std::vector<matrix<rgb_pixel>>& known_faces, 
                    std::vector<std::string>& known_names) 
{
    for (const auto &entry : std::filesystem::directory_iterator("../face_data")) {
        cv::Mat img = cv::imread(entry.path().string());
        if (img.empty()) {
            std::cerr << "Error loading image: " << entry.path().string() << std::endl;
            continue;
        }
        cv::resize(img, img, cv::Size(150, 150));
        if (img.rows != 150 || img.cols != 150) {
            std::cerr << "Error: Image size mismatch. Resizing failed." << std::endl;
            continue;
        }

        matrix<rgb_pixel> dlib_img;
        assign_image(dlib_img, dlib::cv_image<rgb_pixel>(img));  // Corrected cv_image syntax

        known_faces.push_back(dlib_img);
        known_names.push_back(entry.path().stem().string());
    }
}

std::string recognizeFace(const matrix<rgb_pixel>& face, 
                          const std::vector<matrix<rgb_pixel>>& known_faces,
                          const std::vector<std::string>& known_names) 
{
    if (known_faces.empty()) return "Unknown";

    // Get face descriptor (128D vector)
    std::vector<matrix<rgb_pixel>> faces = { face };
    std::vector<matrix<float, 0, 1>> face_descriptors = net(faces);

    for (size_t i = 0; i < known_faces.size(); ++i) {
        std::vector<matrix<float, 0, 1>> known_face_descriptor = net(known_faces);

        // Check similarity
        if (length(face_descriptors[0] - known_face_descriptor[0]) < 0.6) {
            return known_names[i];
        }
    }

    return "Unknown";
}
