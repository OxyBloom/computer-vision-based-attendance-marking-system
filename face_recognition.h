#ifndef FACE_RECOGNITION_H
#define FACE_RECOGNITION_H

#include <dlib/dnn.h>
#include <dlib/image_processing.h>
#include <dlib/image_io.h>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <dlib/opencv/cv_image.h>  // Add this for `cv_image`

// Dlib's face recognition model
using namespace dlib;
using namespace std;

// Network definition
template <int N, template <typename> class BN, int stride, typename SUBNET>
using residual = add_prev1<relu<BN<con<N, 3, 3, stride, stride,
                        relu<BN<con<N, 3, 3, 1, 1, SUBNET>>>>>>>;

template <int N, template <typename> class BN, typename SUBNET>
using resblock = relu<BN<con<N, 3, 3, 1, 1, SUBNET>>>;

template <typename SUBNET> 
using res = resblock<32, bn_con, SUBNET>;

template <typename SUBNET> 
using level4 = repeat<2, res, SUBNET>;

template <typename SUBNET> 
using level3 = repeat<2, res, SUBNET>;

template <typename SUBNET> 
using level2 = repeat<2, res, SUBNET>;

template <typename SUBNET> 
using level1 = repeat<2, res, SUBNET>;

using anet_type = loss_metric<fc_no_bias<128, avg_pool_everything<
    level4<level3<level2<level1<relu<bn_con<con<32, 7, 7, 2, 2,
    input_rgb_image_sized<150>>>>>>>>>>>;

// Extern declaration of the network to avoid multiple definitions
extern anet_type net;  

// Function declarations
void loadKnownFaces(std::vector<matrix<rgb_pixel>>& known_faces, 
                    std::vector<std::string>& known_names);

std::string recognizeFace(const matrix<rgb_pixel>& face, 
                          const std::vector<matrix<rgb_pixel>>& known_faces,
                          const std::vector<std::string>& known_names);

#endif  // FACE_RECOGNITION_H
