#ifndef ATTENDANCE_TRACKER_H
#define ATTENDANCE_TRACKER_H

#include <opencv2/opencv.hpp>
#include <string>
#pragma once

void stopAttendance();  // Declare here, no definition
void markAttendance(const std::string& name);
void startAttendance();


#endif // REGISTER_FACE_H
