#include <color_curve.h>

//////////////////////////////////////////////
//Class Constructor
ColorCurve::ColorCurve()
{	
	t_value_ = 0;
	g_value_ = 0;

	std::cout<<"Initialized - OK" << std::endl;
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
    input_image_ = cv::imread(input_img);

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
	output_image_ = cv::Mat::zeros(input_image_.rows, input_image_.cols, input_image_.type());
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
    if (pixelValue < t_value_) 
    {
        return static_cast<uchar>(255.0 * pow(pixelValue / 255.0, g_value_) * (pixelValue / static_cast<float>(t_value_)));
    } 
    else 
    {
        return static_cast<uchar>(255.0 * pow(pixelValue / 255.0, g_value_));
    }
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

		// //second level of optimization
		// #pragma omp parallel for
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


		//third level of optimization
		const int block_size = 8; // Process 8 pixels at a time (adjust based on your CPU's vector width)
		#pragma omp parallel for
		for (int i = 0; i < input_image_.rows; i += block_size) {
		    for (int j = 0; j < input_image_.cols; j += block_size) {
		        for (int ii = 0; ii < block_size; ++ii) {
		            uchar* inputPtr = input_image_.ptr<uchar>(i + ii);
		            uchar* outputPtr = output_image_.ptr<uchar>(i + ii);

		            for (int jj = 0; jj < block_size; ++jj) {
		                uchar pixelValue = inputPtr[j + jj];
		                uchar modifiedPixelValue = modifyPixelValue(pixelValue);
		                outputPtr[j + jj] = modifiedPixelValue;
		            }
		        }
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

		// //second level of optimization
		// #pragma omp parallel for
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

		//third level of optimization
		const int block_size = 8; // Process 8 pixels at a time (adjust based on your CPU's vector width)
		#pragma omp parallel for

		for (int i = 0; i < input_image_.rows; i += block_size) {
		    for (int j = 0; j < input_image_.cols; j += block_size) {
		        for (int ii = 0; ii < block_size; ++ii) {
		            cv::Vec3b* inputPtr = input_image_.ptr<cv::Vec3b>(i + ii);
		            cv::Vec3b* outputPtr = output_image_.ptr<cv::Vec3b>(i + ii);

		            for (int jj = 0; jj < block_size; ++jj) {
		                cv::Vec3b pixelValue = inputPtr[j + jj];
		                cv::Vec3b modifiedPixelValue = modifyPixelValue(pixelValue);
		                outputPtr[j + jj] = modifiedPixelValue;
		            }
		        }
		    }
		}




	   //   // Apply the function to each pixel value of a color image
       //  // Split the channels of the color image
       //  std::vector<cv::Mat> channels(3);
       //  cv::split(input_image_, channels);

       //  // Apply the function to each channel
       //  for (int c = 0; c < 3; ++c) {
       //      cv::Mat& channel = channels[c];
       //      cv::Mat modifiedChannel(channel.size(), channel.type());
      // 		cv::Mat lut(1, 256, CV_8UC1);
	   //      uchar* lutData = lut.ptr<uchar>();
	   //      for (int i = 0; i < 256; ++i) {
	   //          lutData[i] = modifyPixelValue(pixelValue);
	   //      cv::LUT(channel, lut, modifiedChannel);
       // // .......
       //      modifiedChannel.copyTo(channel);
       //  }

       //  // Merge the modified channels back into the modified image
       //  cv::merge(channels, output_image_);
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
