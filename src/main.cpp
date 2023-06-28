#include <color_curve.h>


int main(int argc, char* argv[]) 
{
    bool testing_mode = false;

    if (argc == 2 && std::string(argv[1]) == "--testing")
    {
        testing_mode = true;
    } 
    //Ensure that all input variables are given by the user
    else if (argc != 5)
    {
        std::cout << "Invalid number of arguments." << std::endl;
        std::cout << "Usage: ./color_curve <input_image> <t_value> <g_value> <output_image> or run in testing mode: ./color_curve --testing" << std::endl;
        return -1;
    }

    if (testing_mode) 
    {
        std::cout << "Running in testing mode" << std::endl;

        //Load the testing data
        std::string filename = "../src/testing.txt"; // Update with your text file name

        //open the testing .txt file
        std::ifstream inputFile(filename);
        if (!inputFile.is_open()) 
        {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return 1;
        }

        //variables to store the lines of the file and their translation to InputData struct
        std::string line;
        std::vector<InputData> inputs;

        //read every line in file and translate it to InputData 
        while (std::getline(inputFile, line)) 
        {
            std::istringstream iss(line);

            InputData inputData;
            if (iss >> inputData.inputImagePath >> inputData.tValue >> inputData.gValue >> inputData.outputImagePath) 
            {
                inputs.push_back(inputData);
            } 
            else 
            {
                std::cerr << "Failed to parse line: " << line << std::endl;
            }
        }

        inputFile.close();


        //Create a class object
        ColorCurve cv;

        // Process the loaded inputs
        for (InputData& inputData : inputs) 
        {
            //Pass the input variables to the class object
            cv.setTValue(inputData.tValue );
            cv.setGValue(inputData.gValue);
            cv.setOutputImage(inputData.outputImagePath);

            if(!cv.setInputImage(inputData.inputImagePath))
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
            std::cout << "Test: " << cv.getFileNameWithoutExtension(inputData.outputImagePath) << "->  Processing time: " << std::fixed << duration.count()<< " ms" << std::endl;
            
            cv.saveOutputImage();
        }
    } 
    else 
    {
        std::cout << "Running in normal mode" << std::endl;

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
    }

	return 0;
}