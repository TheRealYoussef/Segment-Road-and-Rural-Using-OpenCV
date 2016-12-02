#include "segmenter.h"
#include <algorithm>

Scalar lower[] = {Scalar(25, 15, 15), Scalar(25, 15, 5), Scalar(22, 15, 5), Scalar(20, 0, 0)};
Scalar upper[] = {Scalar(103, 200, 255), Scalar(94, 255, 200), Scalar(94, 255, 200), Scalar(94, 255, 200)};

Segmenter::Segmenter(const std::string& imagePath, int imageNumber)
{
    Mat image = imread(imagePath, CV_LOAD_IMAGE_COLOR);

    Mat imageHSV;
    cvtColor(image, imageHSV, COLOR_BGR2HSV);

    Mat grass(image.rows, image.cols, CV_8UC1, Scalar(0));
    Mat road(image.rows, image.cols, CV_8UC1, Scalar(0));

    //Thresholding
    inRange(imageHSV, lower[imageNumber - 1], upper[imageNumber - 1], grass);

    imshow("Rural", grass);

    //CCL to get road
    vector<vector<int> >ccl(image.rows, vector<int>(image.cols, 0));
    vector<int>sizes(1, 0);
    vector<int>equalities(1, 0);
    int label = 1;

    for (int i = 0; i < grass.rows; ++i) {
        for (int j = 0; j < grass.cols; ++j) {
            if (grass.at<uchar>(i, j) == 0) {
                int lowestLabel = label + 1;
                if (i - 1 >= 0) {
                    lowestLabel = min(lowestLabel, equalities[ccl[i - 1][j]]);
                    if (j - 1 >= 0)
                        lowestLabel = min(lowestLabel, equalities[ccl[i - 1][j - 1]]);
                    if (j + 1 < image.cols)
                        lowestLabel = min(lowestLabel, equalities[ccl[i - 1][j + 1]]);
                }
                if (j - 1 >= 0)
                    lowestLabel = min(lowestLabel, equalities[ccl[i][j - 1]]);
                if (lowestLabel == 0) {
                    ccl[i][j] = label++;
                    sizes.push_back(1);
                    equalities.push_back(ccl[i][j]);
                }
                else {
                    ccl[i][j] = lowestLabel;
                    ++sizes[lowestLabel];
                    if (i - 1 >= 0) {
                        if (equalities[ccl[i - 1][j]] != lowestLabel)
                            equalities[ccl[i - 1][j]] = lowestLabel;
                        if (j - 1 >= 0)
                            if (equalities[ccl[i - 1][j - 1]] != lowestLabel)
                                equalities[ccl[i - 1][j - 1]] = lowestLabel;
                        if (j + 1 < image.cols)
                            if (equalities[ccl[i - 1][j + 1]] != lowestLabel)
                                equalities[ccl[i - 1][j + 1]] = lowestLabel;
                    }
                    if (j - 1 >= 0)
                        if (equalities[ccl[i][j - 1]] != lowestLabel)
                            equalities[ccl[i][j - 1]] = lowestLabel;
                }
            }
        }
    }

    //Choose road label
    int maxSize = 0;
    int roadLabel;

    for (int i = 1; i < label; ++i) {
        if (sizes[i] > maxSize) {
            maxSize = sizes[i];
            roadLabel = i;
        }
    }

    if (imageNumber == 1 || imageNumber == 3) {
        for (int i = 0; i < ccl.size(); ++i) {
            for (int j = 0; j < ccl[i].size(); ++j) {
                if (equalities[ccl[i][j]] == roadLabel)
                    road.at<uchar>(i, j) = 255;
            }
        }
    }
    else {
        int secondMaxSize = 0;
        for (int i = 1; i < label; ++i) {
            if (sizes[i] > secondMaxSize && sizes[i] != maxSize) {
                secondMaxSize = sizes[i];
                roadLabel = i;
            }
        }
    }

    for (int i = 0; i < ccl.size(); ++i) {
        for (int j = 0; j < ccl[i].size(); ++j) {
            if (equalities[ccl[i][j]] == roadLabel)
                road.at<uchar>(i, j) = 255;
        }
    }

    imshow("Road", road);
}
