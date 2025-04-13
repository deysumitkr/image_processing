#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

int main() {
    printf("Hello, World!\n");

    std::string image_path = samples::findFile("../data/Sunflower.jpg");
    Mat img = imread(image_path, IMREAD_COLOR);

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
            if ( blue > 100 &&  red < 200 )
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

    return 0;
}
