//------------------------------------------------------------
//
// C++ course assignment code 
//
// G. Papaioannou, 2014
//
//
//-------------------------------------------------------------

#ifndef _FILTER
#define _FILTER

#include "Image.h"

namespace imaging
{
	class Filter
	{
		// Προσθέστε αν θέλετε πεδία και μεθόδους
	public:

		Filter();
		virtual ~Filter() ;

		///////////////////////////////////////////
		static Image*  gray(Image* im);
		static Image* color(Image* im, Vec3<float>* color);
		static Image* blur(Image* im);
		static Image* diff(Image* im);
		static Image* median(Image* im);
	
	
	};

}
#endif