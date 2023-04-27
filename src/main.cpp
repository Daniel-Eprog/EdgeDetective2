/*Edge Detective - general purpose c++ library for edge detecting needs*/

#include "image.h"
#include <string>
#include <ctime>

int main()
{
    //program variables
    bool stillDetecting = true; //determines whether a user is done or not
    bool imageLoaded = false; //determines if an image has been loaded
    int response; //stores user response
    float start = 0.0; //variable to store start time of function
    float end = 0.0; //variable to store end time of function
    float executionTime = 0.0f; //variable to store total execution time
    Image processingImage; //class object to hold image being processed

    //Welcome message
    cout << "***************************************************" << endl;
    cout << "*            Welcome to Edge Detective            *" << endl;
    cout << "*  Your one stop shop c++ edge detection library  *" << endl;
    cout << "*              Load an image to begin             *" << endl;
    cout << "***************************************************" << endl;

    while(stillDetecting)
    {

        //Menu options
        cout << "1.  Load an Image" << endl;
        cout << "2.  Gaussian Blur" << endl;
        cout << "3.  Prewitt Algorithm" << endl;
        cout << "4.  Sobel Algorithm" << endl;
        cout << "5.  Non Maximum Supression" << endl;
        cout << "6.  Double Threshold" << endl;
        cout << "7.  Hysteresis" << endl;
        cout << "8.  Canny Algorithm" << endl;
        cout << "9.  Convolve" << endl;
        cout << "10. Export Image" << endl;
        cout << "11. Exit" << endl;

        //records user response
        cin >> response;

        if(response == 1)
        {
                cout << "Please enter the exact name of your image located in the input folder." << endl;
                string fileName = "";
                string input = "input/";
                cin >> fileName;
                input += fileName;
                start = clock();
                processingImage.loadIMG(input);
                end = clock();
                executionTime = end - start;
                cout << "Image successfully loaded." << endl;
                cout << "Execution time: " << executionTime/CLOCKS_PER_SEC << " seconds." << endl;
                imageLoaded = true;
        }
        else if(response == 2)
        {
            if(!imageLoaded)
            {
                cout << "Sorry, doesn't seem like you've loaded an image yet." << endl;
            }
            else
            {
                start = clock();
                processingImage.gaussianBlur();
                end = clock();
                executionTime = end - start;
                cout << "Gaussian filter successfully applied to image." << endl;
                cout << "Execution time: " << executionTime/CLOCKS_PER_SEC << " seconds." << endl;
            }
        }
        else if(response == 3)
        {
             if(!imageLoaded)
            {
                cout << "Sorry, doesn't seem like you've loaded an image yet." << endl;
            }
            else
            {
                start = clock();
                processingImage.prewittEdgeDetection();
                end = clock();
                executionTime = end - start;
                cout << "Prewitt kernel successfully applied to image." << endl;
                cout << "Execution time: " << executionTime/CLOCKS_PER_SEC << " seconds." << endl;
            }
        }
        else if(response == 4)
        {
             if(!imageLoaded)
            {
                cout << "Sorry, doesn't seem like you've loaded an image yet." << endl;
            }
            else
            {
                start = clock();
                processingImage.sobelEdgeDetection();
                end = clock();
                executionTime = end - start;
                cout << "Sobel kernel successfully applied to image." << endl;
                cout << "Execution time: " << executionTime/CLOCKS_PER_SEC << " seconds." << endl;
            }
        }
        else if(response == 5)
        {
             if(!imageLoaded)
            {
                cout << "Sorry, doesn't seem like you've loaded an image yet." << endl;
            }
            else
            {
                start = clock();
                processingImage.nonMaxSuppression();
                end = clock();
                executionTime = end - start;
                cout << "Supression successfully applied to image." << endl;
                cout << "Execution time: " << executionTime/CLOCKS_PER_SEC << " seconds." << endl;
            }
        }
        else if(response == 6)
        {
             if(!imageLoaded)
            {
                cout << "Sorry, doesn't seem like you've loaded an image yet." << endl;
            }
            else
            {
                start = clock();
                processingImage.doubleThreshold();
                end = clock();
                executionTime = end - start;
                cout << "Threshold set on image" << endl;
                cout << "Execution time: " << executionTime/CLOCKS_PER_SEC << " seconds." << endl;
            }
        }
        else if(response == 7)
        {
             if(!imageLoaded)
            {
                cout << "Sorry, doesn't seem like you've loaded an image yet." << endl;
            }
            else
            {
                start = clock();
                processingImage.hysteresis();
                end = clock();
                executionTime = end - start;
                cout << "Hysteresis successfully applied to image" << endl;
                cout << "Execution time: " << executionTime/CLOCKS_PER_SEC << " seconds." << endl;
            }
        }
        else if(response == 8)
        {
             if(!imageLoaded)
            {
                cout << "Sorry, doesn't seem like you've loaded an image yet." << endl;
            }
            else
            {
                start = clock();
                processingImage.cannyEdgeDetection();
                end = clock();
                executionTime = end - start;
                cout << "Image successfully converted" << endl;
                cout << "Execution time: " << executionTime/CLOCKS_PER_SEC << " seconds." << endl;
            }
        }
        else if(response == 9)
        {
            if(!imageLoaded)
            {
                cout << "Sorry, doesn't seem like you've loaded an image yet." << endl;
            }
            else
            {
                vector<vector<float>> convolutionMatrix;
                cout << "Please enter the values for a 3x3 convolution matrix" << endl;
                cout << "Enter 3 digits at a time separated by a space" << endl;
                for(int i = 0; i < 3; ++i)
                {
                    int a, b, c;
                    cin >> a;
                    cin >> b;
                    cin >> c;
                    convolutionMatrix.push_back({a * 1.0f,b * 1.0f,c * 1.0f});
                }
                start = clock();
                processingImage.convolve(convolutionMatrix);
                end = clock();
                executionTime = end - start;
                cout << "Image successfully convolved with kernel" << endl;
                cout << "Execution time: " << executionTime/CLOCKS_PER_SEC << " seconds." << endl;
            }
        }
        else if(response == 10)
        {
            if(!imageLoaded)
            {
                cout << "Sorry, doesn't seem like you've loaded an image yet." << endl;
            }
            else
            {
                cout << "Please enter the name you wish for your exported image." << endl;
                string fileName = "";
                string output = "output/";
                cin >> fileName;
                output += fileName;
                start = clock();
                processingImage.exportIMG(output);
                end = clock();
                executionTime = end - start;
                cout << "Image successfully exported to the output folder." << endl;
                cout << "Execution time: " << executionTime/CLOCKS_PER_SEC << " seconds." << endl;
            } 
        }
        else if(response == 11)
        {
            stillDetecting = false;
        }
        else
        {
            cout << "Sorry, thats an invalid entry. Please try again." << endl;
        }
        
    }

    //exit message
    cout << "***************************************************" << endl;
    cout << "*#################################################*" << endl;
    cout << "*####### Thanks for using Edge Detective! ########*" << endl;
    cout << "*#################################################*" << endl;
    cout << "***************************************************" << endl;

    
     return 0;
}
