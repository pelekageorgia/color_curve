#include <color_curve.h>

//////////////////////////////////////////////
//Class Constructor
ColorCurve::ColorCurve()
{	
	t_value_ = 0;
	g_value_ = 0;

	std::cout<<"Initialization - OK" << std::endl;
}
//////////////////////////////////////////////

//////////////////////////////////////////////
//Setting the T value
void ColorCurve::setTValue(float &t)
{
	t_value_ = t;
}
//////////////////////////////////////////////

//////////////////////////////////////////////
//Setting the g value
void ColorCurve::setGValue(float &g)
{
	g_value_ = g;
}
//////////////////////////////////////////////

//////////////////////////////////////////////
//Setting the input image
bool ColorCurve::setInputImage(std::string &input_img)
{
    // Load the input image into cv::Mat
    input_image_ = cv::imread(input_img, -1);

    // Check if the image was loaded successfully
    if (input_image_.empty()) {
        std::cout << "Failed to load the image: " << input_img << " Image does not exit" << std::endl;
        return false;
    }

	// Check the image type
    input_image_type_ = input_image_.type();
    if (input_image_type_ != CV_8UC1 && input_image_type_ != CV_8UC3) 
    {
        std::cout << "Unsupported image type. Only 8-bit monochrome and 8-bit RGB images are supported." << std::endl;
        return false;
    }
	output_image_ = input_image_.clone();
    return true;
}
//////////////////////////////////////////////

//////////////////////////////////////////////
//Setting the output image path
void ColorCurve::setOutputImage(std::string &output_img_path)
{
	output_image_path_ = output_img_path;
}
//////////////////////////////////////////////

//////////////////////////////////////////////
//A fucntion to apply a color curve to a monochrome pixel
uchar ColorCurve::modifyPixelValue(uchar &pixelValue)
{
	uchar new_pixel_value;

    if (pixelValue < t_value_) 
    {
        new_pixel_value = static_cast<uchar>(255.0 * pow(pixelValue / 255.0, g_value_) * (pixelValue / static_cast<float>(t_value_)));
    } 
    else 
    {
        new_pixel_value = static_cast<uchar>(255.0 * pow(pixelValue / 255.0, g_value_));
    }

    return new_pixel_value;
}
//////////////////////////////////////////////

//////////////////////////////////////////////
//A fucntion to apply a color curve to a color pixel
cv::Vec3b ColorCurve::modifyPixelValue(cv::Vec3b &pixelValue)
{
 	cv::Vec3b modified_value;

    for (int i = 0; i < 3; ++i) 
    {
        if (pixelValue[i] < t_value_)
        {
            modified_value[i] = static_cast<uchar>(255.0 * pow(pixelValue[i] / 255.0, g_value_) * (pixelValue[i] / static_cast<float>(t_value_)));
        } 
        else
        {
            modified_value[i] = static_cast<uchar>(255.0 * pow(pixelValue[i] / 255.0, g_value_));
        }
    }

    return modified_value;
}
//////////////////////////////////////////////

//////////////////////////////////////////////
//A fucntion to apply a color curve to an input image
bool ColorCurve::applyColorCurveFunction()
{
	//if image is 8UC1
	if(input_image_type_ == 0)
	{
		// // Apply the function to each pixel value
	    // for (int i = 0; i < input_image_.rows; ++i) {
	    //     for (int j = 0; j < input_image_.cols; ++j) {
	    //         uchar pixelValue = input_image_.at<uchar>(i, j);
	    //         uchar modifiedPixelValue = modifyPixelValue(pixelValue);
	    //         output_image_.at<uchar>(i, j) = modifiedPixelValue;
	    //     }
	    // }


		// //first level of optimization
	    // for (int i = 0; i < input_image_.rows; ++i) 
	    // {
		//     uchar* inputPtr = input_image_.ptr<uchar>(i);
		//     uchar* outputPtr = output_image_.ptr<uchar>(i);

		//     for (int j = 0; j < input_image_.cols; ++j) {
		//         uchar pixelValue = inputPtr[j];
		//         uchar modifiedPixelValue = modifyPixelValue(pixelValue);
		//         outputPtr[j] = modifiedPixelValue;
		//     }
		// }

		//second level of optimization
		#pragma omp parallel for
	    for (int i = 0; i < input_image_.rows; ++i) 
	    {
		    uchar* inputPtr = input_image_.ptr<uchar>(i);
		    uchar* outputPtr = output_image_.ptr<uchar>(i);

		    for (int j = 0; j < input_image_.cols; ++j) {
		        uchar pixelValue = inputPtr[j];
		        uchar modifiedPixelValue = modifyPixelValue(pixelValue);
		        outputPtr[j] = modifiedPixelValue;
		    }
		}

	}
	else if (input_image_type_ == 16)
	{
		// // // Apply the function to each pixel value
	    // // for (int i = 0; i < input_image_.rows; ++i) {
	    // //     for (int j = 0; j < input_image_.cols; ++j) {
	    // //         cv::Vec3b pixelValue = input_image_.at<cv::Vec3b>(i, j);

	    // //         cv::Vec3b modifiedPixelValue = modifyPixelValue(pixelValue);

	    // //         output_image_.at<cv::Vec3b>(i, j) = modifiedPixelValue;

	    // //     }
	    // // }

		//first level of optimization
	    // for (int i = 0; i < input_image_.rows; ++i) 
	    // {
		//     cv::Vec3b* inputPtr = input_image_.ptr<cv::Vec3b>(i);
		//     cv::Vec3b* outputPtr = output_image_.ptr<cv::Vec3b>(i);

		//     for (int j = 0; j < input_image_.cols; ++j) {
		//         cv::Vec3b pixelValue = inputPtr[j];
		//         cv::Vec3b modifiedPixelValue = modifyPixelValue(pixelValue);
		//         outputPtr[j] = modifiedPixelValue;
		//     }
		// }

		//second level of optimization
		#pragma omp parallel for
	    for (int i = 0; i < input_image_.rows; ++i) 
	    {
		    cv::Vec3b* inputPtr = input_image_.ptr<cv::Vec3b>(i);
		    cv::Vec3b* outputPtr = output_image_.ptr<cv::Vec3b>(i);

		    for (int j = 0; j < input_image_.cols; ++j) {
		        cv::Vec3b pixelValue = inputPtr[j];
		        cv::Vec3b modifiedPixelValue = modifyPixelValue(pixelValue);
		        outputPtr[j] = modifiedPixelValue;
		    }
		}
	}


}
//////////////////////////////////////////////

//////////////////////////////////////////////
//A fucntion to apply a color curve to an input image
void ColorCurve::saveOutputImage()
{
	cv::imwrite(output_image_path_, output_image_);
}
//////////////////////////////////////////////

//////////////////////////////////////////////
//A fucntion to get the test name from the output image path
std::string ColorCurve::getFileNameWithoutExtension(const std::string& filePath) 
{
    // Find the position of the last occurrence of '/'
    size_t slashPos = filePath.find_last_of('/');
    if (slashPos == std::string::npos) 
    {
        slashPos = 0; // No '/' found, start from the beginning
    } 
    else 
    {
        slashPos++; // Move to the character after '/'
    }

    // Find the position of the last occurrence of '.'
    size_t dotPos = filePath.find_last_of('.');
    if (dotPos == std::string::npos || dotPos <= slashPos) 
    {
        dotPos = filePath.length(); // No '.' found after the last '/'
    }

    // Extract the substring between '/' and '.'
    return filePath.substr(slashPos, dotPos - slashPos);
}
//////////////////////////////////////////////
