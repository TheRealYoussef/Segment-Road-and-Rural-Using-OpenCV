#ifndef SEGMENTER_H
#define SEGMENTER_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

class Segmenter
{
public:
    Segmenter(const std::string& imagePath, int imageNumber);
};

#endif // SEGMENTER_H
