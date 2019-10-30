#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <experimental/filesystem>
#include "of/Image.h"
#include "of/OpticalFlow.h"
#include "of/Cache.h"
#include "of/Opencv.h"

namespace fs = std::experimental::filesystem;


int main(int argc, char *argv[]){
    std::vector<cv::String> fn;
    cv::glob(argv[1], fn, false);

    auto imageOld = cv::Mat();
    auto imageRef = cv::Mat();
    auto mappingU = cv::Mat(), mappingV = cv::Mat();
    for (size_t i=0; i<fn.size(); i++){
        auto imageNew = cv::imread( fn[i], cv::IMREAD_COLOR );

        if(i == 0){
            mappingU = cv::Mat(imageNew.rows, imageNew.cols, CV_32FC1);
            mappingV = cv::Mat(imageNew.rows, imageNew.cols, CV_32FC1);
            for(int y = 0;y < imageNew.rows; y++){
                for(int x = 0;x < imageNew.cols; x++){
                    mappingU.at<float>(y ,x) = x;
                    mappingV.at<float>(y ,x) = y;
                }
            }
            imageRef = imageNew;
        } else {
            auto flowU = cv::Mat(), flowV = cv::Mat();

            auto imageFlowNew = cv::Mat();
            imageNew.convertTo( imageFlowNew, CV_64FC3 );
            imageFlowNew /= 255.0;

            auto imageFlowOld = cv::Mat();
            imageOld.convertTo( imageFlowOld, CV_64FC3 );
            imageFlowOld /= 255.0;

            ofCache(imageFlowOld, imageFlowNew, flowU, flowV, std::string("cache"));

//            cv::Mat humanMat;
//            writeUv(flowU, flowV, humanMat);
//            cv::namedWindow( "miaou", cv::WINDOW_KEEPRATIO );
//            cv::imshow( "miaou", humanMat);
//            cv::waitKey(100);

            auto imageRemap = cv::Mat();
            cv::remap(flowU, flowU, mappingU, mappingV, cv::INTER_CUBIC, cv::BORDER_CONSTANT, cv::Scalar(0,255,0) );
            cv::remap(flowV, flowV, mappingU, mappingV, cv::INTER_CUBIC, cv::BORDER_CONSTANT, cv::Scalar(0,255,0) );
            mappingU += flowU;
            mappingV += flowV;

            cv::remap(imageNew, imageRemap, mappingU, mappingV, cv::INTER_CUBIC, cv::BORDER_CONSTANT, cv::Scalar(0,255,0) );
            cv::namedWindow( "miaou", cv::WINDOW_KEEPRATIO );
            cv::imshow( "miaou", imageRemap);
            cv::waitKey(0);

            cv::imwrite(std::string("tmp/") + std::to_string(i) + ".png", imageRemap);
        }
        imageOld = imageNew;

    }
}
