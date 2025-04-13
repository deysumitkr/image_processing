
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;


// --- Global Threshold Variables ---
int red_threshold_high = 150;
int blue_threshold_low = 50;
int green_threshold_low = 50;
const int max_color_value = 255; // Max value for color channels
const char* control_window_name = "Color Controls";


// Dummy callback function for trackbars (can be empty if direct variable update is sufficient)
static void on_trackbar(int, void*) {
    // This function is called when a slider value changes.
    // You could add logic here if needed, but for simple threshold updates,
    // reading the global variables directly in the processing loop is often enough.
}

// Function to find the bounding box of pixels matching the color condition
// AND draws the rectangle on the input image if found.
// Returns the bounding box (cv::Rect). Returns an empty Rect if no pixels match.
// Note: Input image 'im' is modified by this function if a box is found.
cv::Rect colorObjectDetectorAndDraw(cv::Mat &im) { // <-- Changed to non-const reference
    // Ensure the input image is not empty and is 3-channel (BGR)
    if (im.empty() || im.channels() != 3) {
        std::cerr << "Error: Input image is empty or not a 3-channel BGR image." << std::endl;
        return cv::Rect(); // Return an empty rectangle
    }

    int min_row = im.rows;
    int min_col = im.cols;
    int max_row = -1;
    int max_col = -1;
    bool found_pixel = false;

    for (int i = 0; i < im.rows; i++) {
        for (int j = 0; j < im.cols; j++) {
            // Get pixel reference
            // We don't strictly need 'const' here anymore, but it's fine as we only read pixel values
            const cv::Vec3b& pixel = im.at<cv::Vec3b>(i, j);
            uchar blue = pixel[0];
            uchar green = pixel[1];
            uchar red = pixel[2];

            // Check the condition
            if (red > red_threshold_high && blue < blue_threshold_low && green < green) {
                // Update the bounding box coordinates
                min_row = std::min(min_row, i);
                min_col = std::min(min_col, j);
                max_row = std::max(max_row, i);
                max_col = std::max(max_col, j);
                found_pixel = true;
            }
        }
    }

    // If at least one pixel was found, create the bounding box AND draw it
    if (found_pixel) {
        // Create the rectangle object
        cv::Rect bounding_box(min_col, min_row, (max_col - min_col + 1), (max_row - min_row + 1));

        // --- Draw the rectangle on the input image ---
        cv::rectangle(im,                    // Image to draw on (passed by reference)
                      bounding_box,          // The rectangle to draw
                      cv::Scalar(0, 255, 0), // Color (Green in BGR)
                      2);                    // Thickness of the line
        // --- Rectangle drawn ---

        return bounding_box; // Return the calculated bounding box
    } else {
        // No pixels matched the condition, return an empty rectangle
        return cv::Rect();
    }
}

int main(int, char**)
{
    Mat frame;
    //--- INITIALIZE VIDEOCAPTURE
    VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    // open selected camera using selected API
    cap.open(deviceID, apiID);
    // check if we succeeded
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    // --- Create a window for the trackbars ---
    namedWindow(control_window_name, WINDOW_AUTOSIZE);

    // --- Create Trackbars ---
    createTrackbar("Red Thresh High:", control_window_name, &red_threshold_high, max_color_value, on_trackbar);
    createTrackbar("Blue Thresh Low:", control_window_name, &blue_threshold_low, max_color_value, on_trackbar);
    createTrackbar("Green Thresh Low:", control_window_name, &green_threshold_low, max_color_value, on_trackbar);

    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl
        << "Press any key to terminate" << endl;
    for (;;)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        colorObjectDetectorAndDraw(frame);

        // show live and wait for a key with timeout long enough to show images
        imshow("Live", frame);
        if (waitKey(33) >= 0)
            break;
    }
    // Clean up windows
    destroyWindow(control_window_name);
    destroyWindow("Live");

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
