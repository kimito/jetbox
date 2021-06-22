#pragma once

#include <string>
#include <memory>

#include "opencv2/opencv.hpp"
#include "NvInfer.h"


namespace lunchjet {

struct DriveParameter {
    float steering;
    float throtle;
};

class DriveDetector {
    public:

    DriveDetector(const std::string &model_file_path);
    ~DriveDetector() = default;

    DriveParameter detect(const cv::Mat &image);

    private:

    struct InferDeleter
    {
        template <typename T>
        void operator()(T* obj) const
        {
            if (obj) obj->destroy();
        }
    };
    template <typename T> using unique_ptr_for_nv = std::unique_ptr<T, InferDeleter>;

    std::string engine_file_path;
    unique_ptr_for_nv<nvinfer1::ICudaEngine> engine;
};

} //namespace lunchjet