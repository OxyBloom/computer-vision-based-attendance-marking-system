#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include "register_face.h"
#include "attendance_tracker.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Attendance System");

    QVBoxLayout *layout = new QVBoxLayout();

    QPushButton *registerButton = new QPushButton("Register New Face");
    QPushButton *startButton = new QPushButton("Start Attendance");
    QPushButton *stopButton = new QPushButton("Stop Attendance");

    layout->addWidget(registerButton);
    layout->addWidget(startButton);
    layout->addWidget(stopButton);

    QObject::connect(registerButton, &QPushButton::clicked, []() {
        registerFace();
    });

    QObject::connect(startButton, &QPushButton::clicked, []() {
        startAttendance();
    });

    QObject::connect(stopButton, &QPushButton::clicked, []() {
        stopAttendance();
    });

    window.setLayout(layout);
    window.show();

    return app.exec();
}
