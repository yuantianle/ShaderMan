#include "GenTexture.h"

void GenTexture::CreateCrack(float CrackWidth, float CrackDepth)
{
	cv::Mat outmap = cv::Mat::zeros(1000, 1000, CV_8UC3);
	outmap.setTo(255);

    int n = 1000 * 1000;
    for (int i = 0; i < outmap.rows; i++)
    {
        for (int j = 0; j < outmap.cols; j++)
        {
            if (i >= 250 && i <= 750 && j >= 500 - CrackWidth / 2 && j <= 500 + CrackWidth / 2)
            { //apply noise to RGB image//
                outmap.at<cv::Vec3b>(j, i)[0] = abs((float)j - 500.0) / (CrackWidth / 2.0) * 255.0;
                outmap.at<cv::Vec3b>(j, i)[1] = CrackWidth / 1000.0 * 255.0;
                outmap.at<cv::Vec3b>(j, i)[2] = CrackDepth / 1000.0 * 255.0;
            }
        }
    }

    cv::imwrite("./Texture/Pic1.png", outmap);
}