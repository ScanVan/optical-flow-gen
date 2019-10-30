#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <experimental/filesystem>
#include "of/Image.h"
#include "of/OpticalFlow.h"
#include "of/Opencv.h"

namespace fs = std::experimental::filesystem;


int main(int argc, char *argv[]){
    std::vector<cv::String> fn;
    cv::glob(argv[1], fn, false);

    auto imageOld = cv::Mat();
    for (size_t i=0; i<fn.size(); i++){
        auto imageNew = sv_dense_io_image(fn[i].c_str(), 1.0);
        if(imageOld.cols != 0){
            DImage u;
            DImage v;
            sv_dense_flow(imageOld, imageNew, imageNew.cols, imageNew.rows, imageNew.channels(),u, v);
            cv::Mat mat;

            writeUv(u,v,mat);

            fs::path pt = fs::u8path(fn[i].c_str());
            auto fileName = pt.filename().string();
            cv::imwrite(std::string(argv[2]) + "/" + fileName, mat);

            cv::namedWindow( "miaou", cv::WINDOW_KEEPRATIO );
            cv::imshow( "miaou", mat);
            cv::waitKey(1000);
        }
        imageOld = imageNew;

    }
}
