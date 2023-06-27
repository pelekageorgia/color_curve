#include <color_curve.h>

int main(int argc, char* argv[]) 
{
	//Ensure that all input variables are given by the user
    if (argc != 5) {
        std::cout << "Invalid number of arguments." << std::endl;
        std::cout << "Usage: ./color_curve <input_image> <t_value> <g_value> <output_image>" << std::endl;
        return -1;
    }

    //Parse the input arguments
    std::string inputImage = argv[1];
    float t_value = std::atof(argv[2]);
    float g_value = std::atof(argv[3]);
    std::string outputImage = argv[4];


    //Create a class object
	ColorCurve cv;

	//Pass the input variables to the class object
	cv.setTValue(t_value);
	cv.setGValue(g_value);
	cv.setOutputImage(outputImage);

	if(!cv.setInputImage(inputImage))
		return -1;

	// Start the timer
    auto start = std::chrono::steady_clock::now();
	cv.applyColorCurveFunction();

	// Stop the timer
    auto end = std::chrono::steady_clock::now();

    // Calculate the duration in milliseconds
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Print the duration with three decimal places
    std::cout.precision(3);
    std::cout << "Processing time: " << std::fixed << duration.count()<< " ms" << std::endl;

    cv.saveOutputImage();

	return 0;
}