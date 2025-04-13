#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

Mat src, src_gray;
Mat dst, detected_edges;

int lowThreshold = 0;
const int max_lowThreshold = 100;
const int ratio = 3;
const int kernel_size = 3;
const char* window_name = "Edge Map";

static void CannyThreshold(int, void*)
{
    blur( src_gray, detected_edges, Size(11,11) );
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    dst = Scalar::all(0);
    src.copyTo( dst, detected_edges);
    imshow( window_name, dst );
}

int main() {
    printf("Hello, World!\n");

    std::string image_path = samples::findFile("../data/Sunflower.jpg");
    Mat img = imread(image_path, IMREAD_COLOR);
    src = img.clone();


    if (img.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }

    imshow("Display window", img);
    int k = waitKey(0);  // Wait for a keystroke in the window

    std::cout << img.at<Vec3b>(0, 0) << std::endl;

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            Vec3b& pixel = img.at<Vec3b>(i, j);
            uchar blue = pixel[0];
            uchar green = pixel[1];
            uchar red = pixel[2];
            if ( blue > 10 &&  red < 200 )
            {
                blue = 0;
                green = 0;
                red = 0;
            }
            pixel[0] = blue;
            pixel[1] = green;
            pixel[2] = red;
        }
    }

    imshow("Display window", img);
    waitKey(0);  // Wait for a keystroke in the window


    dst.create( src.size(), src.type() );
    cvtColor( src, src_gray, COLOR_BGR2GRAY );

    namedWindow( window_name, WINDOW_AUTOSIZE );

    createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );

    CannyThreshold(0, 0);

    waitKey(0);

    return 0;
}
