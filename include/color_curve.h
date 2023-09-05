//standart libs
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>

//Opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>


struct InputData {
    std::string inputImagePath;
    float tValue;
    float gValue;
    std::string outputImagePath;
};


class ColorCurve
{
    public:
        //////////////////////////////////////////////
        //Class Constructor / Destructor
        ColorCurve();
        ~ColorCurve(){};

        // Public methods
        void setTValue(float &t);
        void setGValue(float &g);
        bool setInputImage(std::string &input_img);
        void setOutputImage(std::string &output_img_path);
        bool applyColorCurveFunction();
		void saveOutputImage();
        std::string getFileNameWithoutExtension(const std::string& filePath);


    private:
        //////////////////////////////////////////////
        //Private Class members and declarations
        //////////////////////////////////////////////
		uchar modifyPixelValue(uchar &pixelValue);
		cv::Vec3b modifyPixelValue(cv::Vec3b &pixelValue);
        void createLookupTable(int t_value, double g_value);

        float t_value_, g_value_;
        std::string output_image_path_;
        std::vector<uchar> lut_;

        //input and output cv imgs
        cv::Mat input_image_, output_image_;
};

