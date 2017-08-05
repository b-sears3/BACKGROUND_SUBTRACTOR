//Background Subtraction Program
//Created by Bertram Sears III on 4/26/2017

#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
using namespace std;

const int THRESHOLD = 20;

/***********************************
Pixel Structure: contains RGB values
************************************/
struct RGB {
	int r, g, b;
	bool is_object = true;
};


/*******************
	Image Class:
*******************/
class Image
{
private:
	int width = 0;
	int height = 0;                         //dimensions of image
	int totalPixels = 0;                    //total number of pixels in image
	RGB *pixelMap;                          //pixel mapping of image
	string ppmType;                         // (P3 or P6)
	int maxRGB;
	string imagePath;

public:
	Image();
	Image(const string fileName);
	~Image();

	void read_image(const string fileName);
	void write_image(const string fileName);
	//void subtract_background(Image picture, Image objectShadow);
	Image operator-(const Image &right);
	Image operator+(const Image &right);

	//ACCESSOR FUNCTIONS
	const string get_path()
		{return imagePath;}

	//HELPER FUNCTIONS
	int size_of_file(const string fileName);
	int max(int a, int b);
};

/*BEGINNING OF IMAGE CLASS
*********************************************************************************/
Image::Image() 
{
	width = 720;
	height = 480;
	totalPixels = width*height;
	pixelMap = new RGB[totalPixels];
	int maxRGB = 255;
}

Image::Image(const string fileName)
{
	read_image(fileName);
}
Image::~Image()
{
	//Do something here
}

int Image::size_of_file(const string fileName)
{
	int numLines = 0;
	string line;
	ifstream myFile(fileName.c_str());

	if (myFile.is_open())
		while (getline(myFile, line))
			++numLines;

	return numLines;
}

int Image::max(int a, int b)
{
		int max;
		(a >= b) ? max = a : max = b;
		return max;
}

void Image::read_image(const string fileName)
{
	string *trashBin = new string[50]; //holds lines in file that don't need to be read

	//Open the .ppm file
	ifstream imageStream;
	imageStream.open(fileName.c_str());

	//Read header data from file
	if (imageStream.is_open()) {
		imageStream >> ppmType;

		//throw file comments into trash bin
		imageStream >> trashBin[0] >> trashBin[1] >> trashBin[2] >> trashBin[3];

		//Obtain width, height, and maxRGB
		imageStream >> width >> height >> maxRGB;
		totalPixels = width*height; //set totalPixels

		
		cout << "\nReading from " << "'" << fileName << "'" << "....\n";
		cout << "Type: " << ppmType << endl;
		cout << "Width: " << width << "  " << "Height: " << height << endl;
		cout << "Max RGB: " << maxRGB << endl;
		cout << "Num Pixels: " << totalPixels << endl;
		cout << endl;


		//Obtain pixel values from file: read them into array of structs
		pixelMap = new RGB[totalPixels];
		while (!imageStream.eof()) {
			for (int count = 0; count < totalPixels; count++) {
				imageStream >> pixelMap[count].r;
				imageStream >> pixelMap[count].g;
				imageStream >> pixelMap[count].b;
			}
		}
	}
	else if (!imageStream.is_open())
		cout << "ERROR! File not open\n";
	else
		cout << "Woops! Something happened...not sure what\n";

	imageStream.close();
}

void Image::write_image(const string fileName)
{
	ofstream outStream;

	//Construct .ppm header
	outStream.open(fileName.c_str());
	outStream << "P3\n";
	outStream << 720 << " " << 480 << endl;
	outStream << 255;
	outStream << endl;

	//write in pixel data
	int count = 0;
	for (int i = 0; i < totalPixels; i++) {
		if (pixelMap[i].is_object == true) 
			outStream << pixelMap[i].r << " " << pixelMap[i].g << " " << pixelMap[i].b << " ";

		else if (pixelMap[i].is_object == false)
			outStream << 255 << " " << 255 << " " << 255 << " ";

		count += 3;
		if (count >= 24) {
			outStream << endl;
			count = 0;
		}
	}
	outStream.close();
}

Image Image::operator-(const Image &right) {
	
	Image result;
	RGB resultingPixel;
	ofstream outFile;

	result.width = max(width, right.width);
	result.height = max(height, right.height);

	if (maxRGB == right.maxRGB) 
		result.maxRGB = maxRGB;
	else result.maxRGB = max(maxRGB, right.maxRGB);
	

	//START HERE WITH IF STATEMENT
	for (int i = 0; i < totalPixels; i++) {
		if (abs(pixelMap[i].b - right.pixelMap[i].b) >= THRESHOLD && abs(pixelMap[i].g - right.pixelMap[i].g) >= 
			THRESHOLD && abs(pixelMap[i].r- right.pixelMap[i].r) >= THRESHOLD){
		resultingPixel.r = abs(pixelMap[i].r - right.pixelMap[i].r);
		resultingPixel.g = abs(pixelMap[i].g - right.pixelMap[i].g);
		resultingPixel.b = abs(pixelMap[i].b - right.pixelMap[i].b);
		resultingPixel.is_object = true;
	}
		else {
			resultingPixel.r = pixelMap[i].r;
			resultingPixel.g = pixelMap[i].g;
			resultingPixel.b = pixelMap[i].b;
			resultingPixel.is_object = false;
		}
		result.pixelMap[i].r = resultingPixel.r;
		result.pixelMap[i].g = resultingPixel.g;
		result.pixelMap[i].b = resultingPixel.b;
		result.pixelMap[i].is_object = resultingPixel.is_object;

	}

	return result;

}

Image Image::operator+(const Image &right)
{
	Image result;
	RGB resultingPixel;
	ofstream outFile;

	result.width = max(width, right.width);
	result.height = max(height, right.height);

	if (maxRGB == right.maxRGB)
		result.maxRGB = maxRGB;
	else result.maxRGB = max(maxRGB, right.maxRGB);

	for (int i = 0; i < totalPixels; i++) {
		if (abs(pixelMap[i].b - right.pixelMap[i].b) <= 5 && abs(pixelMap[i].g - right.pixelMap[i].g) <=
			5 && abs(pixelMap[i].r - right.pixelMap[i].r) <= 5) {
			resultingPixel.r = pixelMap[i].r;
			resultingPixel.g = pixelMap[i].g;
			resultingPixel.b = pixelMap[i].b;
			resultingPixel.is_object = false;

			//resultingPixel.r = abs((pixelMap[i].r + right.pixelMap[i].r) / 2  );
			//resultingPixel.g = abs((pixelMap[i].g + right.pixelMap[i].g) / 2);
			//resultingPixel.b = abs((pixelMap[i].b + right.pixelMap[i].b) / 2 );
			//resultingPixel.is_object = true;
		}
		//else {
			//resultingPixel.r = pixelMap[i].r;
			//resultingPixel.g = pixelMap[i].g;
			//resultingPixel.b = pixelMap[i].b;
			//resultingPixel.is_object = false;
		//}
		result.pixelMap[i].r = resultingPixel.r;
		result.pixelMap[i].g = resultingPixel.g;
		result.pixelMap[i].b = resultingPixel.b;
		result.pixelMap[i].is_object = resultingPixel.is_object;

	}
	
	return result;
}

/*END OF IMAGE CLASS
*****************************************************************************************************************/


class Blob
{
	
	float minx;
	float miny;
	float maxx;
	float maxy;

	Blob(float x, float y) {
		minx = x;
		miny = y;
		maxx = x;
		maxy = y;
	}


	float size() {
		return (maxx - minx)*(maxy - miny);
	}

	bool isNear(float x, float y) {
		float cx = (minx + maxx) / 2;
		float cy = (miny + maxy) / 2;

		float d = 0; // distanceFormula(cx, cy, x, y);
		/*
		if (d < distThreshold*distThreshold) {
			return true;
		}
		else {
			return false;
		}
		*/
	}
};

int main()
{

	
	Image picsFolder[21] = { Image("image002.ppm"), Image("image003.ppm"), Image("image004.ppm"), Image("image005.ppm"),
							Image("image006.ppm"), Image("image007.ppm"), Image("image008.ppm"),Image("image009.ppm"), 
		                    Image("image010.ppm"), Image("image011.ppm"), Image("image012.ppm"), Image("image013.ppm"),
							Image("image014.ppm"), Image("image015.ppm"), Image("image016.ppm"),
							Image("image017.ppm"), Image("image018.ppm"), Image("image019.ppm"), Image("image020.ppm"),
							Image("image021.ppm"), Image("image022.ppm")
	};
	
	
	
	
	//Image objectShadow = picsFolder[0] - picsFolder[20]; //produces image of where object was, and where it is now

	
	//Define D(N) |I(T) - I(T + N)|   =  Frame 1 - Frame 2
	//adjusting the rate at which we perform two-frame differencing
	cout << "\nPerforming frame differencing...\n";
	Image objectPrediction = (picsFolder[0] - picsFolder[1]) - (picsFolder[1] - picsFolder[2]) - (picsFolder[2] - picsFolder[3]) - (picsFolder[3] - picsFolder[4]) 
		- (picsFolder[4] - picsFolder[5]) - (picsFolder[5] - picsFolder[6]) - (picsFolder[6] - picsFolder[7]) - (picsFolder[7] - picsFolder[8]) 
		- (picsFolder[8] - picsFolder[9]) - (picsFolder[9] - picsFolder[10]) - (picsFolder[10] - picsFolder[11]) - (picsFolder[11] - picsFolder[12]) 
		- (picsFolder[12] - picsFolder[13]) - (picsFolder[13] - picsFolder[14]) - (picsFolder[14] - picsFolder[15]) - (picsFolder[15] - picsFolder[16]) 
		- (picsFolder[16] - picsFolder[17]) - (picsFolder[17] - picsFolder[18]) - (picsFolder[18] - picsFolder[19]) - (picsFolder[19] - picsFolder[20]);
	cout << "\nWriting our subtracted image to file 'NEW_IMAGE.PPM' \n";
	objectPrediction.write_image("NEW_IMAGE.ppm");


	//Attempt to create a model of the original background using (+) operator
	cout << "\nCreating background model\n";
	Image backgroundPrediction = (picsFolder[0] + picsFolder[5]);
	cout << "\nWriting background model to 'BACKGROUND_MODEL.PPM' \n";
	backgroundPrediction.write_image("BACKGROUND_MODEL.ppm");
	


	system("PAUSE");
	return 0;
}