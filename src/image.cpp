#include "image.h"

//loads image from TGA file
void Image::loadIMG(string file)
{
    //if image already loaded clears the image vector
    if(imageLoaded == true)
    {
        this->image.clear();
    }

    //read in file
    ifstream fileToOpen(file, ios_base::binary);

    if(!fileToOpen.is_open())
    {
        cout << "Sorry, your file could not be opened" << endl;
    }
    else
    {
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.idLength), sizeof(this->imageHeader.idLength));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.colorMapType), sizeof(this->imageHeader.colorMapType));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.dataTypeCode), sizeof(this->imageHeader.dataTypeCode));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.colorMapOrigin), sizeof(this->imageHeader.colorMapOrigin));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.colorMapLength), sizeof(this->imageHeader.colorMapLength));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.colorMapDepth), sizeof(this->imageHeader.colorMapDepth));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.xOrigin), sizeof(this->imageHeader.xOrigin));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.yOrigin), sizeof(this->imageHeader.yOrigin));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.width), sizeof(this->imageHeader.width));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.height), sizeof(this->imageHeader.height));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.bitsPerPixel), sizeof(this->imageHeader.bitsPerPixel));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.imageDescriptor), sizeof(this->imageHeader.imageDescriptor));
    }

    //iterates over the file and reads in pixel values
    for (int i = 0; i < (int)this->imageHeader.height; i++)
    {
        vector<Pixel> nextRow = {};//stores the next row of the image being loaded
        
        //iterates over file to read in the next row
        for(int j = 0; j < (int)this->imageHeader.width; j++)
        {
            unsigned char blue;
            unsigned char green;
            unsigned char red;
            fileToOpen.read(reinterpret_cast<char*>(&blue), sizeof(blue));
            fileToOpen.read(reinterpret_cast<char*>(&green), sizeof(green));
            fileToOpen.read(reinterpret_cast<char*>(&red), sizeof(red));
            nextRow.push_back(Pixel(blue, green, red));
        }

        //pushes back current row and empties vector for next iteration
        this->image.push_back(nextRow);
        nextRow = {};
    }

    //converts image to grayscale upon loading
    this->convertGrayScale();

    //sets loaded image
    imageLoaded = true;

    //close image file
    fileToOpen.close();

}

//prints out header information
void Image::getHeader() const
{
    cout << "ID Length: " << (int)imageHeader.idLength << endl;
    cout << "Color Map Type: " << (int)imageHeader.colorMapType << endl;
    cout << "Data Type Code: " << (int)imageHeader.dataTypeCode << endl;
    cout << "Color Map Origin: " << imageHeader.colorMapOrigin << endl;
    cout << "Color Map Length: " << imageHeader.colorMapLength << endl;
    cout << "Color Map Depth: " << (int)imageHeader.colorMapDepth << endl;
    cout << "X Origin: " << imageHeader.xOrigin << endl;
    cout << "Y Origin: " << imageHeader.yOrigin << endl;
    cout << "Width: " << imageHeader.width << endl;
    cout << "Height: " << imageHeader.height << endl;
    cout << "Bits Per Pixel: " << (int)imageHeader.bitsPerPixel << endl;
    cout << "Image Descriptor: " << (int)imageHeader.imageDescriptor << endl;
}

//exports image as TGA file
void Image::exportIMG(string file) const
{
    //goes through image header and writes out to file
    ofstream fileToWrite(file, ios_base::binary);
    fileToWrite.write((char*) &imageHeader.idLength, sizeof(imageHeader.idLength));
    fileToWrite.write((char*) &imageHeader.colorMapType, sizeof(imageHeader.colorMapType));
    fileToWrite.write((char*) &imageHeader.dataTypeCode, sizeof(imageHeader.dataTypeCode));
    fileToWrite.write((char*) &imageHeader.colorMapOrigin, sizeof(imageHeader.colorMapOrigin));
    fileToWrite.write((char*) &imageHeader.colorMapLength, sizeof(imageHeader.colorMapLength));
    fileToWrite.write((char*) &imageHeader.colorMapDepth, sizeof(imageHeader.colorMapDepth));
    fileToWrite.write((char*) &imageHeader.xOrigin, sizeof(imageHeader.xOrigin));
    fileToWrite.write((char*) &imageHeader.yOrigin, sizeof(imageHeader.yOrigin));
    fileToWrite.write((char*) &imageHeader.width, sizeof(imageHeader.width));
    fileToWrite.write((char*) &imageHeader.height, sizeof(imageHeader.height));
    fileToWrite.write((char*) &imageHeader.bitsPerPixel, sizeof(imageHeader.bitsPerPixel));
    fileToWrite.write((char*) &imageHeader.imageDescriptor, sizeof(imageHeader.imageDescriptor));
    
    //iterates over image and writes out all pixel values
    for(int i = 0; i < (int)this->imageHeader.height; ++i)
    {
        for(int j = 0; j < (int)this->imageHeader.width; ++j)
        {
        fileToWrite.write((char*) &this->image[i][j].blue, sizeof(this->image[i][j].blue));
        fileToWrite.write((char*) &this->image[i][j].green, sizeof(this->image[i][j].green));
        fileToWrite.write((char*) &this->image[i][j].red, sizeof(this->image[i][j].red));
        }
    }
}

//convolution method for applying filters to current image
vector<double> Image::convolve(vector<vector<float>> convolutionKernel)
{

    //stores the value of the new image
    vector<double> newImage = {};

    //iterates over the image and determines the new pixel value based on the sum of 
    //the products of all adjacent pixels based on kernel passed in
    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            //value to store new pixel
            int newPixelValue = 0;

            //tests for segmentation faults by accounting for image borders
            //pixel in first column or left border
            if(j == 0)
            {
                //pixel at top left corner of image
                if(i == 0)
                {
                    newPixelValue += this->image[i][j + 1].red * convolutionKernel[1][2];
                    newPixelValue += this->image[i + 1][j].red * convolutionKernel[2][1];
                    newPixelValue += this->image[i +1][j + 1].red * convolutionKernel[2][2];
                    newPixelValue += this->image[i][j].red * convolutionKernel[1][1];
                    newImage.push_back(newPixelValue);
                }
                //pixel on bottom right corner of the image
                else if(i == imageHeader.height - 1)
                {
                    newPixelValue += this->image[i - 1][j].red * convolutionKernel[0][1];
                    newPixelValue += this->image[i - 1][j + 1].red * convolutionKernel[0][2];
                    newPixelValue += this->image[i][j + 1].red * convolutionKernel[1][2];
                    newPixelValue += this->image[i][j].red * convolutionKernel[1][1];
                    newImage.push_back(newPixelValue);
                }
                //general case pixel is not in corner but against left border
                else
                {
                    newPixelValue += this->image[i - 1][j].red * convolutionKernel[0][1];
                    newPixelValue += this->image[i - 1][j + 1].red * convolutionKernel[0][2];
                    newPixelValue += this->image[i][j + 1].red * convolutionKernel[1][2];
                    newPixelValue += this->image[i +1][j + 1].red * convolutionKernel[2][2];
                    newPixelValue += this->image[i + 1][j].red * convolutionKernel[2][1];
                    newPixelValue += this->image[i][j].red * convolutionKernel[1][1];
                    newImage.push_back(newPixelValue);
                }

            }
            //pixel is along the right border of the image
            else if(j == imageHeader.width - 1)
            {
                //pixel is in the top right corner of the image
                if(i == 0)
                {
                    newPixelValue += this->image[i][j - 1].red * convolutionKernel[1][0];
                    newPixelValue += this->image[i + 1][j - 1].red * convolutionKernel[2][0];
                    newPixelValue += this->image[i + 1][j].red * convolutionKernel[2][1];
                    newPixelValue += this->image[i][j].red * convolutionKernel[1][1];
                    newImage.push_back(newPixelValue);
                }
                //pixel is in the bottom right corner of the image
                else if(i == imageHeader.height - 1)
                {
                    newPixelValue += this->image[i - 1][j].red * convolutionKernel[0][1];
                    newPixelValue += this->image[i][j - 1].red * convolutionKernel[1][0];
                    newPixelValue += this->image[i - 1][j - 1].red * convolutionKernel[0][0];
                    newPixelValue += this->image[i][j].red * convolutionKernel[1][1];
                    newImage.push_back(newPixelValue);
                }
                //general case pixel is not in a corner but is along the right border
                else
                {
                    newPixelValue += this->image[i - 1][j].red * convolutionKernel[0][1];
                    newPixelValue += this->image[i][j - 1].red * convolutionKernel[1][0];
                    newPixelValue += this->image[i - 1][j - 1].red * convolutionKernel[0][0];
                    newPixelValue += this->image[i + 1][j - 1].red * convolutionKernel[2][0];
                    newPixelValue += this->image[i + 1][j].red * convolutionKernel[2][1];
                    newPixelValue += this->image[i][j].red * convolutionKernel[1][1];
                    newImage.push_back(newPixelValue);
                }

            }
            //pixel is not against a horizontal border
            else
            {
                //pixel is along the top of the image
                if(i == 0)
                {
                    newPixelValue += this->image[i][j + 1].red * convolutionKernel[1][2];
                    newPixelValue += this->image[i +1][j + 1].red * convolutionKernel[2][2];
                    newPixelValue += this->image[i][j - 1].red * convolutionKernel[1][0];
                    newPixelValue += this->image[i + 1][j - 1].red * convolutionKernel[2][0];
                    newPixelValue += this->image[i + 1][j].red * convolutionKernel[2][1];
                    newPixelValue += this->image[i][j].red * convolutionKernel[1][1];
                    newImage.push_back(newPixelValue);
                }
                //pixel is along the bottom of the image
                else if(i == imageHeader.height - 1)
                {
                    newPixelValue += this->image[i - 1][j].red * convolutionKernel[0][1];
                    newPixelValue += this->image[i][j - 1].red * convolutionKernel[1][0];
                    newPixelValue += this->image[i - 1][j - 1].red * convolutionKernel[0][0];
                    newPixelValue += this->image[i - 1][j + 1].red * convolutionKernel[0][2];
                    newPixelValue += this->image[i][j + 1].red * convolutionKernel[1][2];
                    newPixelValue += this->image[i][j].red * convolutionKernel[1][1];
                    newImage.push_back(newPixelValue);
                }
                //pixel is in the center of the image all adjacent pixels are available
                else
                {
                    newPixelValue += this->image[i - 1][j].red * convolutionKernel[0][1];
                    newPixelValue += this->image[i][j - 1].red * convolutionKernel[1][0];
                    newPixelValue += this->image[i - 1][j - 1].red * convolutionKernel[0][0];
                    newPixelValue += this->image[i - 1][j + 1].red * convolutionKernel[0][2];
                    newPixelValue += this->image[i][j + 1].red * convolutionKernel[1][2];
                    newPixelValue += this->image[i + 1][j - 1].red * convolutionKernel[2][0];
                    newPixelValue += this->image[i + 1][j].red * convolutionKernel[2][1];
                    newPixelValue += this->image[i + 1][j + 1].red * convolutionKernel[2][2];
                    newPixelValue += this->image[i][j].red * convolutionKernel[1][1];
                    newImage.push_back(newPixelValue);
                }
            }
        }
    }

    //returns the new image
    return newImage;

}

//iterates over image and converts pixels to their grayscale value
void Image::convertGrayScale()
{
    //stores new grayscale value
    float grayScaleValue = 0;

    //iterates over image and sets grayscale intensity using luminosity method
    //which takes into account human color perception
    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            grayScaleValue = (0.21 * this->image[i][j].red) + (0.72 * this->image[i][j].green) + (0.07 * this->image[i][j].blue); 
            this->image[i][j].blue = grayScaleValue;
            this->image[i][j].green = grayScaleValue;
            this->image[i][j].red = grayScaleValue;
        }

    }

}

//gaussian blur is applied to your image to reduce noise
void Image::gaussianBlur()
{
    vector<double> blurredImage; //stores the new values of the blurred image
    //gaussian kernel blends the image on convolution by determining pixel intensity based on the
    //sum of the products of the ratio of surrounding pixels
    vector<vector<float>> gaussianKernel = {{0.075f, 0.124f, 0.075f},{0.124f, .204f, 0.124f},{0.075f, 0.0124f, 0.075f}};
    blurredImage = this->convolve(gaussianKernel);

    //keeps count of position in 1D representation of image
    int count = 0;

    //iterates over image and sets new values to respective color channels
    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            this->image[i][j].red = blurredImage[count];
            this->image[i][j].blue = blurredImage[count];
            this->image[i][j].green = blurredImage[count];
            count += 1;
        }

    }
}

//prewitt kernel for general edge detection
void Image::prewittEdgeDetection()
{
    vector<double> Gx = {}; //stores the magnitude of horizontal edges
    vector<double> Gy = {}; //stores the magnitude of the vertical edges
    vector<double> G = {}; //stores the over magnitude of both axis

    vector<vector<float>> horizontalOperator = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}}; //horizontal prewitt kernel 
    vector<vector<float>> verticalOperator = {{-1, -1, -1}, {0, 0, 0}, {1, 1, 1}}; //vertical prewitt kernel
    Gx = this->convolve(horizontalOperator); //convolves with respective kernel and stores horizontal edges
    Gy = this->convolve(verticalOperator); //convolves with respective kernel and stores vertical edges

    //iterates of vertical and horizontal edges and finds the overall
    //magnitude of pixel intensity
    for(int i = 0; i < Gx.size(); ++i)
    {
        int magnitude = 0;
        magnitude = sqrt(pow(Gx[i], 2) + pow(Gy[i], 2));
        //initial min max threshold for noise reduction
        if(magnitude > 255)
        {
            magnitude = 255;
        }
        if(magnitude < 50)
        {
            magnitude = 0;
        }
        G.push_back(magnitude);
    }

    //Find the angle of the pixel intensity by calculating the hypotenuse of the 
    //individual pixel edge values
    for(int i = 0; i < Gx.size(); ++i)
    {
        double theta = 0.0f;
        theta = atan(Gy[i] * 1.0f/Gx[i]);
        if(theta == -0 || isnan(theta))
        {
            theta = 0;
        }
        theta = theta * 180/M_PI;
        if(theta < 0)
        {
            theta += 180;
        }
        this->edgeAngles.push_back(theta);
    }

    //keeps count of 1D image position
    int count = 0;

    //iterates over the image and sets new pixel values to respective channel
    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            this->image[i][j].red = G[count];
            this->image[i][j].blue = G[count];
            this->image[i][j].green = G[count];
            count += 1;
        }
    }

}

//sobel kernel for general edge detection
void Image::sobelEdgeDetection()
{
    vector<double> Gx = {}; //stores the magnitude of horizontal edges
    vector<double> Gy = {}; //stores the magnitude of the vertical edges
    vector<double> G = {}; //stores the over magnitude of both axis

    vector<vector<float>> horizontalOperator = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}}; //horizontal sobel kernel
    vector<vector<float>> verticalOperator = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}}; //vertical sobel kernel
    Gx = this->convolve(horizontalOperator); //convolves with respective kernel and stores horizontal edges
    Gy = this->convolve(verticalOperator); //convolves with respective kernel and stores vertical edges

    //iterates of vertical and horizontal edges and finds the overall
    //magnitude of pixel intensity
    for(int i = 0; i < Gx.size(); ++i)
    {
        int magnitude = 0;
        magnitude = sqrt(pow(Gx[i], 2) + pow(Gy[i], 2));
        //initial min max threshold for noise reduction
        if(magnitude > 255)
        {
            magnitude = 255;
        }
        G.push_back(magnitude);
    }

   //Find the angle of the pixel intensity by calculating the hypotenuse of the 
    //individual pixel edge values
    for(int i = 0; i < Gx.size(); ++i)
    {
        double theta = 0.0f;
        theta = atan(Gy[i] * 1.0f/Gx[i]);
        if(theta == -0 || isnan(theta))
        {
            theta = 0;
        }
        theta = theta * 180/M_PI;
        if(theta < 0)
        {
            theta += 180;
        }
        this->edgeAngles.push_back(theta);
    }

     //keeps count of 1D image position
    int count = 0;

    //iterates over the image and sets new pixel values to respective channel
    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            this->image[i][j].red = G[count];
            this->image[i][j].blue = G[count];
            this->image[i][j].green = G[count];
            count += 1;
        }
    }

}

//uses the angle of pixel intesities to calculate and thin out pixel fields
void Image::nonMaxSuppression() {

    // This function will use the angle of each edge and evaluate the pixels on both sides in the direction it is facing
    // If a pixel has a higher magnitude than the surrounding pixels then it will be used in the image, if not then 0 will be used.
    int count = 0;

    //stores the intensity values for the new image after suppression
    vector<int> newImage;

    //iterates over image and sets new pixel intensities based on the local maximum
    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            //tests for edge cases based on border of the image
            //pixel is at the left border
            if (j == 0)
            {
                //pixel is in the top left corner of the image
                if (i == 0)
                {
                    // If the edge angle is between +/-22.5 degrees we will round it to 0 degrees. The Pixel at [i][j] is compared to the pixels to the left and right.
                    if ( (0 <= this->edgeAngles[count] && this->edgeAngles[count] < 22.5) || (157.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 180) )
                    {
                        // If the value is greater than both neighboring pixels then it is a local max, and we do not change it
                        if (this->image[i][j].red >= this->image[i][j+1].red)
                        {
                            count += 1;
                            newImage.push_back(this->image[i][j].red);

                        }
                        // If either of the 2 values around it are greater than it then we suppress the pixel to 0
                        else{

                            newImage.push_back(0);
                            count += 1;
                        }
                    }

                    // If the edge angle is between 22.5 and 67.5 degrees we will round it to 45 degrees. The Pixel at [i][j] is compared to the pixels to the top right and bottom left.
                    else if (22.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 67.5)
                    {
                        newImage.push_back(this->image[i][j].red);
                        count += 1;
                    }
                    // If the edge angle is between 67.5 and 112.5 degrees we will round it to 90 degrees. The Pixel at [i][j] is compared to the pixels to the top and bottom.
                    else if (67.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 112.5)
                    {
                        if (this->image[i][j].red >= this->image[i+1][j].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;

                        }
                        else{
                            newImage.push_back(0);
                            count += 1;
                        }
                    }

                        // If the edge angle is between 112.5 and 157.5 degrees we will round it to 135 degrees. The Pixel at [i][j] is compared to the pixels to the top left and bottom right.
                    else if (112.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 157.5)
                    {
                        if (this->image[i][j].red >= this->image[i+1][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }

                }
                //pixel is at the bottom left corner of image
                else if(i == imageHeader.height - 1)
                {
                    // If the edge angle is between +/-22.5 degrees we will round it to 0 degrees. The Pixel at [i][j] is compared to the pixels to the left and right.
                    if ( (0 <= this->edgeAngles[count] && this->edgeAngles[count] < 22.5) || (157.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 180) ){

                        // If the value is greater than both neighboring pixels then it is a local max, and we do not change it
                        if (this->image[i][j].red >= this->image[i][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;

                        }

                            // If either of the 2 values around it are greater than it then we suppress the pixel to 0
                        else{
                            newImage.push_back(0);
                            count += 1;
                        }
                    }

                    // If the edge angle is between 22.5 and 67.5 degrees we will round it to 45 degrees. The Pixel at [i][j] is compared to the pixels to the top right and bottom left.
                    else if (22.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 67.5)
                    {
                        if (this->image[i][j].red >= this->image[i-1][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else{
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 67.5 and 112.5 degrees we will round it to 90 degrees. The Pixel at [i][j] is compared to the pixels to the top and bottom.
                    else if (67.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 112.5)
                    {

                        if (this->image[i][j].red >= this->image[i-1][j].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else{
                            newImage.push_back(0);
                            count += 1;
                        }
                    }

                    // If the edge angle is between 112.5 and 157.5 degrees we will round it to 135 degrees. The Pixel at [i][j] is compared to the pixels to the top left and bottom right.
                    else if (112.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 157.5)
                    {       
                        newImage.push_back(this->image[i][j].red);
                        count += 1;
                    }
                }
                //general case pixel is not in a corner but is against left border
                else
                {
                    // If the edge angle is between +/-22.5 degrees we will round it to 0 degrees. The Pixel at [i][j] is compared to the pixels to the left and right.
                    if ( (0 <= this->edgeAngles[count] && this->edgeAngles[count] < 22.5) || (157.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 180) )
                    {
                        // If the value is greater than both neighboring pixels then it is a local max, and we do not change it
                        if (this->image[i][j].red >= this->image[i][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }

                        // If either of the 2 values around it are greater than it then we suppress the pixel to 0
                        else{
                            newImage.push_back(0);
                            count += 1;
                        }
                    }

                    // If the edge angle is between 22.5 and 67.5 degrees we will round it to 45 degrees. The Pixel at [i][j] is compared to the pixels to the top right and bottom left.
                    else if (22.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 67.5)
                    {
                        if (this->image[i][j].red >= this->image[i-1][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else{
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 67.5 and 112.5 degrees we will round it to 90 degrees. The Pixel at [i][j] is compared to the pixels to the top and bottom.
                    else if (67.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 112.5)
                    {
                        if (this->image[i][j].red >= this->image[i+1][j].red && this->image[i][j].red >= this->image[i-1][j].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else{
                            newImage.push_back(0);
                            count += 1;
                        }
                    }

                    // If the edge angle is between 112.5 and 157.5 degrees we will round it to 135 degrees. The Pixel at [i][j] is compared to the pixels to the top left and bottom right.
                    else if (112.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 157.5)
                    {

                        if (this->image[i][j].red >= this->image[i+1][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                }

            }
            //pixel is along the right border of image
            else if(j == imageHeader.width - 1)
            {
                //pixel is in the rop right corner of image
                if (i == 0)
                {
                    // If the edge angle is between +/-22.5 degrees we will round it to 0 degrees. The Pixel at [i][j] is compared to the pixels to the left and right.
                    if ( (0 <= this->edgeAngles[count] && this->edgeAngles[count] < 22.5) || (157.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 180) ){

                        // If the value is greater than both neighboring pixels then it is a local max, and we do not change it
                        if (this->image[i][j].red >= this->image[i][j-1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }

                        // If either of the 2 values around it are greater than it then we suppress the pixel to 0
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 22.5 and 67.5 degrees we will round it to 45 degrees. The Pixel at [i][j] is compared to the pixels to the top right and bottom left.
                    else if (22.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 67.5)
                    {

                        if (this->image[i][j].red >= this->image[i+1][j-1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;

                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 67.5 and 112.5 degrees we will round it to 90 degrees. The Pixel at [i][j] is compared to the pixels to the top and bottom.
                    else if (67.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 112.5){

                        if (this->image[i][j].red >= this->image[i+1][j].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;

                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 112.5 and 157.5 degrees we will round it to 135 degrees. The Pixel at [i][j] is compared to the pixels to the top left and bottom right.
                    else if (112.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 157.5)
                    {
                        newImage.push_back(this->image[i][j].red);
                        count += 1;
                    }

                }
                //pixel is in the bottom right corner of the image
                else if(i == imageHeader.height - 1)
                {

                    // If the edge angle is between +/-22.5 degrees we will round it to 0 degrees. The Pixel at [i][j] is compared to the pixels to the left and right.
                    if ( (0 <= this->edgeAngles[count] && this->edgeAngles[count] < 22.5) || (157.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 180) )
                    {
                        // If the value is greater than both neighboring pixels then it is a local max, and we do not change it
                        if (this->image[i][j].red >= this->image[i][j-1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        // If either of the 2 values around it are greater than it then we suppress the pixel to 0
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 22.5 and 67.5 degrees we will round it to 45 degrees. The Pixel at [i][j] is compared to the pixels to the top right and bottom left.
                    else if (22.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 67.5)
                    {
                        newImage.push_back(this->image[i][j].red);
                        count += 1;

                    }
                    // If the edge angle is between 67.5 and 112.5 degrees we will round it to 90 degrees. The Pixel at [i][j] is compared to the pixels to the top and bottom.
                    else if (67.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 112.5){

                        if (this->image[i][j].red >= this->image[i-1][j].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 112.5 and 157.5 degrees we will round it to 135 degrees. The Pixel at [i][j] is compared to the pixels to the top left and bottom right.
                    else if (112.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 157.5){

                        if (this->image[i][j].red >= this->image[i-1][j-1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                }
                //general case pixel is not in a corner but is along right border
                else
                {
                    // If the edge angle is between +/-22.5 degrees we will round it to 0 degrees. The Pixel at [i][j] is compared to the pixels to the left and right.
                    if ( (0 <= this->edgeAngles[count] && this->edgeAngles[count] < 22.5) || (157.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 180) )
                    {
                        // If the value is greater than both neighboring pixels then it is a local max, and we do not change it
                        if (this->image[i][j].red >= this->image[i][j-1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        // If either of the 2 values around it are greater than it then we suppress the pixel to 0
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 22.5 and 67.5 degrees we will round it to 45 degrees. The Pixel at [i][j] is compared to the pixels to the top right and bottom left.
                    else if (22.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 67.5)
                    {
                        if (this->image[i][j].red >= this->image[i+1][j-1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 67.5 and 112.5 degrees we will round it to 90 degrees. The Pixel at [i][j] is compared to the pixels to the top and bottom.
                    else if (67.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 112.5)
                    {
                        if (this->image[i][j].red >= this->image[i+1][j].red && this->image[i][j].red >= this->image[i-1][j].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else 
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 112.5 and 157.5 degrees we will round it to 135 degrees. The Pixel at [i][j] is compared to the pixels to the top left and bottom right.
                    else if (112.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 157.5)
                    {
                        if (this->image[i][j].red >= this->image[i-1][j-1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                }

            }
            //pixel is not against any horizontal border
            else 
            {
                //pixel is along the top of the image
                if (i == 0)
                {
                    // If the edge angle is between +/-22.5 degrees we will round it to 0 degrees. The Pixel at [i][j] is compared to the pixels to the left and right.
                    if ( (0 <= this->edgeAngles[count] && this->edgeAngles[count] < 22.5) || (157.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 180) )
                    {
                        // If the value is greater than both neighboring pixels then it is a local max, and we do not change it
                        if (this->image[i][j].red >= this->image[i][j-1].red && this->image[i][j].red >= this->image[i][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        // If either of the 2 values around it are greater than it then we suppress the pixel to 0
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 22.5 and 67.5 degrees we will round it to 45 degrees. The Pixel at [i][j] is compared to the pixels to the top right and bottom left.
                    else if (22.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 67.5)
                    {
                        if (this->image[i][j].red >= this->image[i+1][j-1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 67.5 and 112.5 degrees we will round it to 90 degrees. The Pixel at [i][j] is compared to the pixels to the top and bottom.
                    else if (67.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 112.5)
                    {
                        if (this->image[i][j].red >= this->image[i+1][j].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 112.5 and 157.5 degrees we will round it to 135 degrees. The Pixel at [i][j] is compared to the pixels to the top left and bottom right.
                    else if (112.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 157.5)
                    {
                        if (this->image[i][j].red >= this->image[i+1][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                }
                //pixel is along the bottom of the image
                else if(i == imageHeader.height - 1){
                    // If the edge angle is between +/-22.5 degrees we will round it to 0 degrees. The Pixel at [i][j] is compared to the pixels to the left and right.
                    if ( (0 <= this->edgeAngles[count] && this->edgeAngles[count] < 22.5) || (157.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 180) )
                    {
                        // If the value is greater than both neighboring pixels then it is a local max, and we do not change it
                        if (this->image[i][j].red >= this->image[i][j-1].red && this->image[i][j].red >= this->image[i][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        // If either of the 2 values around it are greater than it then we suppress the pixel to 0
                        else{
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 22.5 and 67.5 degrees we will round it to 45 degrees. The Pixel at [i][j] is compared to the pixels to the top right and bottom left.
                    else if (22.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 67.5)
                    {
                        if (this->image[i][j].red >= this->image[i-1][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                        // If the edge angle is between 67.5 and 112.5 degrees we will round it to 90 degrees. The Pixel at [i][j] is compared to the pixels to the top and bottom.
                    else if (67.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 112.5)
                    {
                        if (this->image[i][j].red >= this->image[i-1][j].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 112.5 and 157.5 degrees we will round it to 135 degrees. The Pixel at [i][j] is compared to the pixels to the top left and bottom right.
                    else if (112.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 157.5)
                    {
                        if (this->image[i][j].red >= this->image[i-1][j-1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                }
                //general case pixel is in the center of image
                //all adjacent pixels are available
                else
                {
                    // If the edge angle is between +/-22.5 degrees we will round it to 0 degrees. The Pixel at [i][j] is compared to the pixels to the left and right.
                    if ( (0 <= this->edgeAngles[count] && this->edgeAngles[count] < 22.5) || (157.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 180) )
                    {

                        // If the value is greater than both neighboring pixels then it is a local max, and we do not change it
                        if (this->image[i][j].red >= this->image[i][j-1].red && this->image[i][j].red >= this->image[i][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;

                        }
                        // If either of the 2 values around it are greater than it then we suppress the pixel to 0
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 22.5 and 67.5 degrees we will round it to 45 degrees. The Pixel at [i][j] is compared to the pixels to the top right and bottom left.
                    else if (22.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 67.5)
                    {
                        if (this->image[i][j].red >= this->image[i+1][j-1].red && this->image[i][j].red >= this->image[i-1][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else{
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 67.5 and 112.5 degrees we will round it to 90 degrees. The Pixel at [i][j] is compared to the pixels to the top and bottom.
                    else if (67.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 112.5){

                        if (this->image[i][j].red >= this->image[i+1][j].red && this->image[i][j].red >= this->image[i-1][j].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 112.5 and 157.5 degrees we will round it to 135 degrees. The Pixel at [i][j] is compared to the pixels to the top left and bottom right.
                    else if (112.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 157.5)
                    {
                        if (this->image[i][j].red >= this->image[i-1][j-1].red && this->image[i][j].red >= this->image[i+1][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                }

            }

        }
    }

    //keeps count of 1D position of image
    count = 0;

    //sets new pixel values to respective channel
    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            this->image[i][j].red = newImage[count];
            this->image[i][j].blue = newImage[count];
            this->image[i][j].green = newImage[count];
            count += 1;
        }
    }
}

//reduces the image into two pixel values strong and weak to further differentiate edges
void Image::doubleThreshold()
{
    float lowThresholdRatio = 0.1; //sets the ratio for low end threshold
    float highThresholdRatio = 0.15; //sets the ratior for high end threshold
    float highThreshold; //high end threshold value
    float lowThreshold; //low end threshold value
    int imageMax = 0; //stores the maximum pixel intesity

    //iterates over image and determines the value
    //of pixel with the highest intensity
    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            if(this->image[i][j].red > imageMax)
            {
                imageMax = this->image[i][j].red;
            }

        }
    }

    highThreshold = imageMax * highThresholdRatio; //determines the highThreshold based on the ratio of the maximum pixel
    lowThreshold = highThreshold * lowThresholdRatio; //determines the low threshold based on the ratio of the highThreshold value

    int weak = 25; //standard value for weak pixel
    int strong = 255; //standard value for strong pixel

    //iterates over the image and test pixel intesnity based on the threshold values
    //pixels above the high threshold are set to 255 and are strong
    //pixels between high and low are weak and set to 25
    //pixels below the thresholds are set to 0
    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            if(this->image[i][j].red >= highThreshold)
            {
                this->image[i][j].red = strong;
                this->image[i][j].blue = strong;
                this->image[i][j].green = strong;

            }
            else if(this->image[i][j].red < lowThreshold)
            {
                this->image[i][j].red = 0;
                this->image[i][j].blue = 0;
                this->image[i][j].green = 0;

            }
            else if(this->image[i][j].red <= highThreshold && this->image[i][j].red >= lowThreshold)
            {
                this->image[i][j].red = weak;
                this->image[i][j].blue = weak;
                this->image[i][j].green = weak;

            }
        }
    }
}

//condenses pixels together based on their surrounding pixel values
void Image::hysteresis()
{

    //iterates over the image and checks all weak pixels for adjacent strong pixels
    //if a weak pixel touches a strong pixel then it too becomes strong
    //if not then the weak pixel is set to 0
    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            //determines if pixel is weak
            if(this->image[i][j].red == 25)
            {
            
            //test for segmentation faults
            //pixel is along the left border of the image
            if(j == 0)
            {
                //pixel is in the top left corner of the image
                if(i == 0)
                {
                    if( this->image[i][j + 1].red == 255 || this->image[i + 1][j].red == 255 || this->image[i +1][j + 1].red == 255)
                    {
                        this->image[i][j].red = 255;
                        this->image[i][j].blue = 255;
                        this->image[i][j].green = 255;
                    }
                    else
                    {
                        this->image[i][j].red = 0;
                        this->image[i][j].blue = 0;
                        this->image[i][j].green = 0;
                    }
                }
                //pixel is in the bottom left corner of the image
                else if(i == imageHeader.height - 1)
                {
                    if(this->image[i - 1][j].red == 255 || this->image[i - 1][j + 1].red == 255 || this->image[i][j + 1].red == 255)
                    {
                        this->image[i][j].red = 255;
                        this->image[i][j].blue = 255;
                        this->image[i][j].green = 255;
                    }
                    else
                    {
                        this->image[i][j].red = 0;
                        this->image[i][j].blue = 0;
                        this->image[i][j].green = 0;
                    }
                   
                }
                //general case pixel is along the left border but not in a corner
                else
                {
                    if(this->image[i - 1][j].red == 255 || 
                    this->image[i - 1][j + 1].red == 255 || 
                    this->image[i][j + 1].red == 255 || 
                    this->image[i +1][j + 1].red == 255 || 
                    this->image[i + 1][j].red == 255)
                    {
                        this->image[i][j].red = 255;
                        this->image[i][j].blue = 255;
                        this->image[i][j].green = 255;
                    }
                    else
                    {
                        this->image[i][j].red = 0;
                        this->image[i][j].blue = 0;
                        this->image[i][j].green = 0;
                    }
                }

            }
            //pixel is along the right border of the image
            else if(j == imageHeader.width - 1)
            {
                //pixel is in the top right corner of the image
                if(i == 0)
                {
                    if( this->image[i][j - 1].red == 255 || this->image[i + 1][j - 1].red == 255 || this->image[i + 1][j].red == 255)
                    {
                        this->image[i][j].red = 255;
                        this->image[i][j].blue = 255;
                        this->image[i][j].green = 255;
                    }
                    else
                    {
                        this->image[i][j].red = 0;
                        this->image[i][j].blue = 0;
                        this->image[i][j].green = 0;
                    }
                }
                //pixel is in the bottom right corner of the image
                else if(i == imageHeader.height - 1)
                {
                    if(this->image[i - 1][j].red == 255 || this->image[i][j - 1].red == 255 || this->image[i - 1][j - 1].red == 255)
                    {
                        this->image[i][j].red = 255;
                        this->image[i][j].blue = 255;
                        this->image[i][j].green = 255;
                    }
                    else
                    {
                        this->image[i][j].red = 0;
                        this->image[i][j].blue = 0;
                        this->image[i][j].green = 0;
                    }
                }
                //general case pixel is not in a corner but is along the right border
                else
                {
                    if( this->image[i - 1][j].red == 255 ||
                        this->image[i][j - 1].red == 255 ||
                        this->image[i - 1][j - 1].red == 255 ||
                        this->image[i + 1][j - 1].red == 255 ||
                        this->image[i + 1][j].red == 255
                        )
                    {
                        this->image[i][j].red = 255;
                        this->image[i][j].blue = 255;
                        this->image[i][j].green = 255;
                    }
                    else
                    {
                        this->image[i][j].red = 0;
                        this->image[i][j].blue = 0;
                        this->image[i][j].green = 0;
                    }
                }

            }
            //general case pixel is not against any horizontal border
            else
            {
                //pixel is against the top of the image
                if(i == 0)
                {
                    if (
                    this->image[i][j + 1].red == 255 ||
                    this->image[i +1][j + 1].red == 255 ||
                    this->image[i][j - 1].red == 255 ||
                    this->image[i + 1][j - 1].red == 255 ||
                    this->image[i + 1][j].red == 255
                    )
                    {
                        this->image[i][j].red = 255;
                        this->image[i][j].blue = 255;
                        this->image[i][j].green = 255;
                    }
                    else
                    {
                        this->image[i][j].red = 0;
                        this->image[i][j].blue = 0;
                        this->image[i][j].green = 0;
                    }
                }
                //pixel is against the bottom of the image
                else if(i == imageHeader.height - 1)
                {
                    if(
                        this->image[i - 1][j].red == 255 ||
                        this->image[i][j - 1].red == 255 ||
                        this->image[i - 1][j - 1].red == 255 ||
                        this->image[i - 1][j + 1].red == 255 ||
                        this->image[i][j + 1].red == 255
                    )
                    {
                        this->image[i][j].red = 255;
                        this->image[i][j].blue = 255;
                        this->image[i][j].green = 255;
                    }
                    else
                    {
                        this->image[i][j].red = 0;
                        this->image[i][j].blue = 0;
                        this->image[i][j].green = 0;
                    }
                }
                //general case pixel is in the middle of the image and all adjacent pixels are available
                else
                {
                    if(
                        this->image[i - 1][j].red == 255 ||
                        this->image[i][j - 1].red == 255 ||
                        this->image[i - 1][j - 1].red == 255 ||
                        this->image[i - 1][j + 1].red == 255 ||
                        this->image[i][j + 1].red == 255 ||
                        this->image[i + 1][j - 1].red == 255 ||
                        this->image[i + 1][j].red == 255 ||
                        this->image[i + 1][j + 1].red == 255
                    )
                    {
                        this->image[i][j].red = 255;
                        this->image[i][j].blue = 255;
                        this->image[i][j].green = 255;
                    }
                    else
                    {
                        this->image[i][j].red = 0;
                        this->image[i][j].blue = 0;
                        this->image[i][j].green = 0;
                    }
                }
            }
        }
        }
    }
}

//canny edge detection calls all preceding steps
void Image::cannyEdgeDetection()
{
    this->gaussianBlur(); //applies gaussian blur
    this->sobelEdgeDetection(); //applies sobel edge detection
    this->nonMaxSuppression(); //applies nonmaxsuppression
    this->doubleThreshold(); //applies thresholding
    this->hysteresis(); //applies hysteresis
}