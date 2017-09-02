//------------------------------------------------------------
//
// C++ course assignment code 
//
// G. Papaioannou, 2015
//
//

#ifndef _IMAGE
#define _IMAGE

#include "Serializable.h"
#include "Array.h"
#include "Vec3.h"

using namespace math;



// We put every class or function associated with the image storage, compression and manipulation 
// in the "imaging" namespace
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

	class Image : public Serializable, Array<Vec3<float>>
	{
	public:
		enum channel_t { RED = 0, GREEN, BLUE };          // now you can use the names RED, GREEN, BLUE instead of 0,1,2



	public:
		// metric accessors
		const unsigned int getWidth() const { return Array<Vec3<float>>::getWidth(); }      // returns the width of the image
		const unsigned int getHeight() const { return Array<Vec3<float>>::getHeight(); }    // returns the height of the image

		// data accessors
		Vec3<float> * getRawDataPtr();                           // Obtain a pointer to the internal data
		// This is NOT a copy of the internal image data, but rather 
		// a pointer to the internally allocated space, so DO NOT
		// attempt to delete the pointer. 

		Vec3<float>* getPixel(unsigned int x, unsigned int y) const;    // get the color of the image at location (x,y)
		// Do any necessary bound checking. Also take into account
		// the "interleaved" flag to fetch the appropriate data
		// Return a black (0,0,0) color in case of an out-of-bounds
		// x,y pair

		// data mutators
		void setPixel(unsigned int x, unsigned int y, Vec3<float> & value);
		// Set the RGB values for an (x,y) pixel. Do all 
		// necessary bound checks and respect the "interleaved"
		// flag when updating our data.

		void setData(const Vec3<float>* & data_ptr);            // Copy the data from data_ptr to the internal buffer.
		// The function ASSUMES a proper size for the incomming data array.

		void resize(unsigned int new_width, unsigned int new_height);
		// Change the internal data storage size to the new ones.
		// If the one or both of the dimensions are smaller, clip the 
		// by discarding the remaining pixels in the rows / columns outside
		// the margins. If the new dimensions are larger, pad the old pixels
		// with zero values (black color).
		//-------Serializable------
		bool operator << (std::string filename);     // Read the contents of an object from the specified file
		bool Serializable::operator >> (std::string filename);     // Write the contents of an object to the specified file


		// constructors and destructor
		Image();											     // default: zero dimensions, nullptr for the buffer.	
		Image(unsigned int width, unsigned int height);
		Image(unsigned int width, unsigned int height, const Vec3<float> * data_ptr);
		Image(const Image &src);
		~Image();

		Image & operator = (const Image & right);

	};

} //namespace imaging

class Image
{
public:
	Image();
	~Image();
};

#endif

