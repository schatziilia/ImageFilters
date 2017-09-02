#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "ppm_format.h"
#include "Filter.h"
#include "Vec3.h"
#include <array>

using namespace std;
using namespace imaging;


void main(int argc, char *argv[]){

	char* filename = new char[50];
	char* fileToWrite = new char[50];
	float red_num = 0.0f, green_num = 0.0f, blue_num = 0.0f;
	bool write = true;
	
	imaging::Image * filteredim = new imaging::Image();
	
	Vec3<float> * col;


	//--------Read the PPM image---------\\

	if (argc>1){ //the filename is provided as argument to the application
		filename = argv[argc - 1]; //filename is the last string in argument list
		cout << "File name of the image to load: " << filename << endl;

		imaging::Image * im = imaging::ReadPPM(filename); //Reading the ppm file
		float pix_num = float(im->getWidth()*im->getHeight()); //number of pixels in image

		//Finding average color of the image
		for (int i = 0; i < im->getWidth()*im->getHeight(); i++){ //adding 
			red_num = red_num + im->getRawDataPtr()[i].r;  // reds,
			green_num = green_num + im->getRawDataPtr()[i].g; //greens
			blue_num = blue_num + im->getRawDataPtr()[i].b; //and blues 
		}//for

		cout << "The average color of the image is (" << red_num / pix_num << " , " << green_num / pix_num << " , " << blue_num / pix_num << " )" << endl;


	//----------Applying Filters---------//
		filteredim = new imaging::Image(im->getWidth(), im->getHeight(), im->getRawDataPtr());
		int i = 1;
		while (i<argc-1){
			if (string(argv[i]) == "-f"){
				if (string(argv[i + 1]) == "gray"){
					filteredim = Filter::gray(filteredim);
					i=i+2;
				}
				else if (string(argv[i + 1]) == "color") {
					if (argc>i+1){
						col = new Vec3<float>(atof(argv[i+2]), atof(argv[i+3]), atof(argv[i+4]));
						filteredim = Filter::color(filteredim, col);
						i = i + 5;
					}//if
				}
				else if (string(argv[i + 1]) == "blur"){
					filteredim = Filter::blur(filteredim);
					i = i + 2;
				}
				else if (string(argv[i + 1]) == "diff"){
					filteredim = Filter::diff(filteredim);
					i = i + 2;
				}
				else if (string(argv[i + 1]) == "median"){
					filteredim = Filter::median(filteredim);
					i = i + 2;
				}//if
			}
			else{
				cout << "No filters were given!" << endl;
				write = false;
				i = argc - 1;
			}//if

		}//while


		//--------Write the image to the given file---------\\
				
		if (write == true){
			string temp = filename;
			temp = temp.substr(0, 7) + ".filtered.ppm";
			fileToWrite = const_cast<char*>(temp.c_str());
				
			bool read = WritePPM(*filteredim, fileToWrite);
			if (read){
				cout << "Image: " << filename << " was written to the file: " << fileToWrite << endl;
			}
			else{
				cerr << "Unable to write image!" << endl;
			}//if
		}//if
		else{
			cout << "image NOT!" << endl;
		}//if



	}//if
	
	system("pause"); //waiting to press a button to close the command line


}//main

