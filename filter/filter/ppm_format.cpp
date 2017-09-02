#include "ppm_format.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "Image.h"


namespace imaging{
	using namespace std;

	Image * ReadPPM(const char * filename){

		char *temp_buffer;
		int sizeofbuf;
		//string sLine;
		imaging::Image *image = new imaging::Image();

		ifstream file(filename, ios_base::binary);
		if (!file) {
			cerr << "Cannot open file" << endl;
		}//if


		if (file.good()){ //(!file.fail())&(!file.eof())

			//--header--

			string format, width, height, size;
			file >> format >> width >> height >> size;

			if (format.compare("P6") == 0){ //checking if p6
				if (size.compare("255") == 0) {  //checking size

					//cout << format << " " << width << " " << height << " " << size << endl;

					unsigned int w = atoi(width.c_str()); //string to int
					unsigned int h = atoi(height.c_str()); //string to int
					unsigned int s = atoi(size.c_str()); //string to int


					cout << "Image dimensions: " << width << " x " << height << endl;
					sizeofbuf = 3 * w*h; //there are w*h pixel and each pixel has 3 float elements(r,g,b)

					//--data--
					char *temp = (char*)malloc(1);
					file.read(temp, 1);
					free(temp);

					temp_buffer = (char*)malloc(sizeofbuf);
					file.read((char*)temp_buffer, sizeofbuf - 1);
					float * dataF = new float[3 * w*h];
					// cast to float [0,1] and insert to data buffer
					for (int i = 0; i < 3 * w*h; i++){
						dataF[i] = (static_cast<float>(static_cast<unsigned char>(temp_buffer[i]))) / 255.0f;
					}//for

					//buffer with float to buffer with Vec3/Color/Pixel
					Vec3<float> *data = new Vec3<float>[w*h];

					int i = 0;
					int j = 0;
					while (i < w*h){
						while (j + 2 < 3 * w*h){
							data[i].r = dataF[j];
							data[i].g = dataF[j + 1];
							data[i].b = dataF[j + 2];
							j = j + 3;
							i++;
						}//while
					}//while

					image = new imaging::Image(w, h, data); //insert in object of type Image

					free(temp_buffer);
					free(dataF);
				}
				else{
					cerr << "Image does not have the desired size. The image size must be 255!" << endl;
				}//if size = 255

			}
			else{
				cerr << "Image is not in P6 format.  " << endl;
			}//if p6

		}//if

		file.close();
		return image;



	}//readPPM


	bool WritePPM(Image & image, const char * filename){
		bool written = false;

		ofstream file(filename, ios::binary);
		if (!file) {
			cerr << "Cannot open file" << endl;
			written = false;
		}
		else{
			file << "P6" << "\n" << to_string(image.getWidth()) << "\n" << to_string(image.getHeight()) << "\n" << "255" << endl;
			// cast from float to char and insert to temporary buffer
			char * temp_buffer = (char *)malloc(3 * image.getWidth()*image.getHeight());
			int i = 0;
			int j = 0;
			while (i <image.getWidth()*image.getHeight()){
				while (j + 2 < 3 * image.getWidth()*image.getHeight()){
					temp_buffer[j] = static_cast<char>(static_cast<unsigned char>(image.getRawDataPtr()[i].r * 255.0f));
					temp_buffer[j + 1] = static_cast<char>(static_cast<unsigned char>(image.getRawDataPtr()[i].g * 255.0f));
					temp_buffer[j + 2] = static_cast<char>(static_cast<unsigned char>(image.getRawDataPtr()[i].b * 255.0f));
					j = j + 3;
					i++;
				}//while
			}//while

			file.write((char*)temp_buffer, (image.getWidth())*(image.getHeight()) * 3); //write data to giveb file
			written = true;
			if (file.fail()){
				cerr << "Could not write data" << endl;
				written = false;
			}//if
			file.close();
		}//if
		return written;
	}//writePPM






}//imaging
