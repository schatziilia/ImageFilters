//------------------------------------------------------------
//
// C++ course assignment code 
//
// G. Papaioannou, 2015
//
//

#ifndef _ARRAY_
#define _ARRAY_

namespace math
{

	//---------------------------------------------------------------------------------------------
	// Do NOT modify this section. For the implementation, see comment below the class declaration

	template <typename T>
	class Array
	{
	protected:
		T * buffer;
		unsigned int width, height;

	public:

		unsigned int getWidth() const { return width; }      // returns the width of the image
		unsigned int getHeight() const { return height; }    // returns the height of the image

		void * getRawDataPtr();                              // Obtain a pointer to the internal data
		// This is NOT a copy of the internal image data, but rather 
		// a pointer to the internally allocated space, so DO NOT
		// attempt to delete the pointer. 

		T & operator () (int x, int y);                      // return a reference to the element at position (column x, row y) 
		// of the array (zeor-based)

		const T & operator () (int x, int y) const;          // return a reference to the element at position (column x, row y) 
		// of the array (zeor-based)

		Array(unsigned int w, unsigned int h);               // Constructors. No default constructor allowed, as it makes no sense.
		Array(const Array<T> & source);                      //

		Array & operator = (const Array<T> & source);        // Copy assignment operator.

		bool operator == (const Array<T> & source) const;    // returns true of the current array and the source have the same dimensions AND 
		// one by one their elements of type T are the same. 

		void resize(unsigned int new_width, unsigned int new_height);	// Change the internal data storage size to the new ones.
		// If the one or both of the dimensions are smaller, clip the 
		// by discarding the remaining elements in the rows / columns outside
		// the margins. If the new dimensions are larger, pad the old elements
		// with elements initialized ot their default value (i.e. with their default initializer/constructor).

		virtual ~Array();									 // The distructor

	};

	// This is a templated class so include its implementation in the header file below this line:
	//--------------------------------------------------------------------------------------------

	template <typename T>
	void * Array<T>::getRawDataPtr(){
		return Array<T>::buffer;
	}//getRawDataPtr


	// return a reference to the element at position (column x, row y) 
	// of the array (zeor-based)
	template <typename T>
	T & Array<T>::operator () (int x, int y){
		T element;
		if ((x < (this->height)) && (y < (this->width))){ //checking the boundaries
			element = buffer[x*(this->width) + y]; //where x*(this->width)+y] is the position of the pixel in (x,y)
		}
		else{
			element = 0; //else return black color
		}//if
		return element;
	}//()

	// return a reference to the element at position (column x, row y) 
	// of the array (zeor-based)
	template <typename T>
	const T & Array<T>::operator () (int x, int y) const{
		return static_cast<T>(Array<T>::operator()(x, y));
	}//()


	// Constructors. No default constructor allowed, as it makes no sense.

	template <typename T>
	Array<T>::Array(unsigned int w, unsigned int h){
		this->width = w;
		this->height = h;


	}

	template <typename T>
	Array<T>::Array(const Array<T> & source){
		this->width = source.getWidth();
		this->height = source.getHeight();
	}


	// Copy assignment operator.
	template <typename T>
	Array<T> & Array<T>::operator = (const Array<T> & source){
		return Array<T>(source.width, source.height);
	}




	// returns true of the current array and the source have the same dimensions AND 
	// one by one their elements of type T are the same. 
	template <typename T>
	bool Array<T>::operator == (const Array<T> & source) const{
		bool flag = true;
		if ((this->width == source.getWidth()) && (this->height == source.getHeight())){
			int i = 0;
			while ((i < (this->height)*(this->width)) && (flag != false)){ //if buffer with color/vec3
				if (!(this->buffer[i] == source.buffer[i])){
					flag = false;
				}//if
				i++;
			}//while
		}
		else{
			flag = false;
		}//if
		return flag;
	}//==



	// Change the internal data storage size to the new ones.
	// If the one or both of the dimensions are smaller, clip the 
	// by discarding the remaining elements in the rows / columns outside
	// the margins. If the new dimensions are larger, pad the old elements
	// with elements initialized ot their default value (i.e. with their default initializer/constructor).
	template <typename T>
	void Array<T>::resize(unsigned int new_width, unsigned int new_height){
		if (new_width != this->width || new_height != this->height){
			
			this->width = new_width;
			this->height = new_height;

		}
		else{
			std::cout << "The given dimensions are the original dimensions!" << std::endl;

		}//if


	}//resize





	// The distructor
	template <typename T>
	Array<T>::~Array(){
		std::cout << "deleting" << std::endl;
		delete[]buffer;
	}






} // namespace math


#endif