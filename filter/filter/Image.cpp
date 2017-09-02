#include "Image.h"
#include "ppm_format.h"

#include <iostream>


namespace imaging
{


	//------------------------------------ class Image ------------------------------------------------
	// 
	// It is the class that represents a generic data container for an image. It holds the actual buffer
	// of the pixel values and provides methods for accessing them either as individual pixels or as
	// a memory block. The Image class alone does not provide any functionality for loading and storing an image, as
	// it is the result or input to such a procedure. 
	//
	// The internal buffer of an image object stores the actual bytes (data) of the color image as
	// a contiguous sequence of RGB triplets. Hence, the size of the buffer variable holding these data is 
	// 3 X width X height bytes.

	// data accessors

	// Obtain a pointer to the internal data
	// This is NOT a copy of the internal image data, but rather 
	// a pointer to the internally allocated space, so DO NOT
	// attempt to delete the pointer. 
	Vec3<float> * Image::getRawDataPtr(){

		return static_cast<Vec3<float> *>(this->Array<Vec3<float>>::getRawDataPtr());

	}//getrawdataptr                           




	// get the color of the image at location (x,y)
	// Do any necessary bound checking. Also take into account
	// the "interleaved" flag to fetch the appropriate data
	// Return a black (0,0,0) color in case of an out-of-bounds
	// x,y pair
	Vec3<float>* Image::getPixel(unsigned int x, unsigned int y) const{
		Vec3<float>* color;
		if ((x < (this->height)) && (y < (this->width))){ //checking the boundaries
			color = &buffer[x*(this->width) + y]; //where x*(this->width)+y] is the position of the red of the pixel in (x,y)
		}
		else{
			color = new Vec3<float>(); //else return black color
		}//if
		return color;

	}//getpixel


	// data mutators

	// Set the RGB values for an (x,y) pixel. Do all 
	// necessary bound checks 
	void Image::setPixel(unsigned int x, unsigned int y, Vec3<float> & value){
		if ((x<(this->height)) && (y<(this->width))){ //checking the boundaries
			buffer[x*(this->width) + y] = value; //where x*(this->width)+y] is the position of the pixel in (x,y)
		}//if
	}//setpixel



	// Copy the data from data_ptr to the internal buffer.
	// The function ASSUMES a proper size for the incomming data array.
	void Image::setData(const Vec3<float>* & data_ptr){
		this->buffer = new Vec3<float>[(this->width)*(this->height)];
		for (int i = 0; i < (this->width)*(this->height); i++){
			this->buffer[i] = data_ptr[i];
		}//for

	}//setdata 




	// Change the internal data storage size to the new ones.
	// If the one or both of the dimensions are smaller, clip the 
	// by discarding the remaining pixels in the rows / columns outside
	// the margins. If the new dimensions are larger, pad the old pixels
	// with zero values (black color).
	void Image::resize(unsigned int new_width, unsigned int new_height)  {
		this->Array<Vec3<float>>::resize(new_width, new_height);

		Vec3<float> * newdata = new Vec3<float>[new_width*new_height];
		//if (new_width != this->width || new_height != this->height){
		if (new_width < this->width && new_height < this->height){
			for (int i = 0; i < new_width; i++){
				for (int j = 0; j < new_height; j++){
					newdata[j*(new_width)+i] = static_cast<Vec3<float>>(this->buffer[j*new_width + i]);
				}//for
			}//for

			const Vec3<float> *dataptr = newdata;
			this->setData(dataptr);

		}
		else if ((new_width > this->width) || (new_height > this->height)){
			for (int i = 0; i < new_width; i++){
				for (int j = 0; j < new_height; j++){
					if (i > this->width || j > this->height){
						newdata[j*(new_width)+i] = Vec3<float>();
					}
					else {
						newdata[j*(new_width)+i] = static_cast<Vec3<float>>(this->buffer[j*new_width + i]);
					}//if
				}//for
			}//for
			const Vec3<float> *dataptr = newdata;
			this->setData(dataptr);

		}//if


	}//resize



	// constructors 

	Image::Image() : Array<Vec3<float>>(0, 0){      // default: zero dimensions, nullptr for the buffer.
		//height = 0;
		//width = 0; 
		buffer = nullptr;

	}


	Image::Image(unsigned int width, unsigned int height) : Array<Vec3<float>>(width, height){
		const Vec3<float> * dataptr;
		buffer = new Vec3<float>[width*height];
		for (int i = 0; i < width*height; i++)
		{
			buffer[i] = static_cast<Vec3<float>>(0.0f);
		}//for

		this->setData(dataptr);
	}

	Image::Image(unsigned int width, unsigned int height, const Vec3<float> * data_ptr) : Array<Vec3<float>>(width, height){
		this->setData(data_ptr);
	}

	Image::Image(const Image &src) : Array<Vec3<float>>(src){
		Image source = src;
		const Vec3<float> * dataptr = source.getRawDataPtr();
		this->setData(dataptr);
	}


	//destructor

	Image::~Image(){
		std::cout << "deleting" << std::endl;
		delete[]buffer;
	}//~


	imaging::Image & Image::operator = (const Image & right){
		return static_cast<Image &>(this->Array<Vec3<float>>::operator=(right));
	}//=



	//-------Serializable--------

	// Read the contents of an object from the specified file
	bool Image::operator<< (std::string filename){
		const char* charFilename = filename.c_str();
		Image * temp = ReadPPM(charFilename); //Reading the ppm file
		this->buffer = temp->buffer;
		this->height = temp->height;
		this->width = temp->width;
		if (this->buffer != nullptr && this->width != 0 && this->height != 0){
			return true;
		}
		else{
			return false;
		}//if
	}//<<

	// Write the contents of an object to the specified file
	bool Image::operator>> (std::string filename){
		const char* charFilename = filename.c_str();
		return WritePPM(*this, charFilename);
	}//>>



} //namespace imaging

