#include "Filter.h"
#include <vector>
#include <algorithm>

namespace imaging{


	//------------Local Filters------------//

	Image* Filter::gray(Image* im){
		Image * imageF ;
		const int size = im->getWidth()*im->getHeight();
		Vec3<float> * newbuf = new Vec3<float>[size];
		for (int i = 0; i < im->getHeight()*im->getWidth(); i++){
			newbuf[i].r = (im->getRawDataPtr()[i].r + im->getRawDataPtr()[i].g + im->getRawDataPtr()[i].b)/3;
			newbuf[i].g = (im->getRawDataPtr()[i].r + im->getRawDataPtr()[i].g + im->getRawDataPtr()[i].b) / 3;
			newbuf[i].b = (im->getRawDataPtr()[i].r + im->getRawDataPtr()[i].g + im->getRawDataPtr()[i].b) / 3;
		}//for
		imageF = new Image(im->getWidth(),im->getHeight(), newbuf);
		return imageF;
	};


	Image* Filter::color(Image* im, Vec3<float>* col){
		Image * imageF;
		const int size = im->getWidth()*im->getHeight();
		Vec3<float> * newbuf = new Vec3<float>[size];
		for (int i = 0; i < im->getHeight()*im->getWidth(); i++){
			newbuf[i].r = im->getRawDataPtr()[i].r* col->r;
			newbuf[i].g = im->getRawDataPtr()[i].g* col->g;
			newbuf[i].b = im->getRawDataPtr()[i].b* col->b;
		}//for
		imageF = new Image(im->getWidth(), im->getHeight(), newbuf);
		return imageF;
	};


	//-------------Neighbourhood Filters-------------//

	std::vector<Vec3<float>*> findNeighbors(Image* im, int  i, int j){
		std::vector<Vec3<float>*>  ne;
		ne.push_back(im->getPixel(i, j));
		if (i - 1<im->getHeight() && i - 1 >= 0 && j - 1<im->getWidth() && j - 1 >= 0){
			ne.push_back(im->getPixel(i - 1, j - 1));
		}//if
		if (i - 1<im->getHeight() && i - 1 >= 0)
		{
			ne.push_back(im->getPixel(i - 1, j));
			if (j + 1 < im->getWidth() && j + 1 >= 0){
				ne.push_back(im->getPixel(i - 1, j + 1));
			}//if
		}//if
		if (j - 1<im->getWidth() && j - 1 >= 0){
			ne.push_back(im->getPixel(i, j - 1));
		}//if
		if (i + 1<im->getHeight() && j + 1<im->getWidth() && i + 1 >= 0 && j + 1 >= 0){
			ne.push_back(im->getPixel(i + 1, j + 1));
		}//if
		if (i + 1<im->getHeight() && i + 1 >= 0){
			ne.push_back(im->getPixel(i + 1, j));
			if (j - 1 < im->getWidth() && j - 1 >= 0){
				ne.push_back(im->getPixel(i + 1, j - 1));
			}//if
		}//if
		if (j + 1<im->getWidth() && j + 1 >= 0){
			ne.push_back(im->getPixel(i, j + 1));
		} //if

		return ne;

	}




	Image* Filter::blur(Image* im){
		Image * imageF = new Image(im->getWidth(), im->getHeight(), im->getRawDataPtr());
		std::vector<Vec3<float>*>  ne;
		Vec3<float> pix;
		float reds , greens, blues;
		for (int i = 0; i <im->getHeight(); i++){
			for (int j = 0; j < im->getWidth(); j++){
				ne = findNeighbors(im, i, j);
				reds = 0.0f, greens = 0.0f, blues = 0.0f;
				for (int m = 0; m < ne.size(); m++){
					reds = reds + ne[m]->r;
					greens = greens + ne[m]->g;
					blues = blues + ne[m]->b;
				}//for

				pix.r = reds / (float)ne.size();
				pix.g = greens / (float)ne.size();
				pix.b = blues / (float)ne.size();

				imageF->setPixel(i, j, pix);
				ne.clear();
				pix.r = 0.0f;
				pix.g = 0.0f;
				pix.b = 0.0f;
			}//for
		}//for
		 
		
		return imageF;
	};


	Image* Filter::diff(Image* im){
		Image * imageF = new Image(im->getWidth(), im->getHeight(), im->getRawDataPtr());
		std::vector<Vec3<float>*>  ne;
		Vec3<float> pix;
		std::vector<float> reds, greens, blues;
		int n;

		for (int i = 0; i < im->getHeight(); i++){
			for (int j = 0; j < im->getWidth(); j++){
				ne = findNeighbors(im, i, j);
				for (int m = 0; m < ne.size(); m++){
					reds.push_back( ne[m]->r);
					greens.push_back(ne[m]->g);
					blues.push_back(ne[m]->b);
				}//for

				std::sort(reds.begin(), reds.end());
				std::sort(greens.begin(), greens.end());
				std::sort(blues.begin(), blues.end());
				
				pix.r = reds[ne.size()-1] - reds[0];
				pix.g = greens[ne.size() - 1] - greens[0];
				pix.b = blues[ne.size() - 1] - blues[0];

				imageF->setPixel(i, j, pix);

				ne.clear();
				reds.clear();
				greens.clear();
				blues.clear();
				pix.r = 0.0f;
				pix.g = 0.0f;
				pix.b = 0.0f;

			}//for
		}//for
		
		return imageF;
	};

	Image* Filter::median(Image* im){
		Image * imageF = new Image(im->getWidth(), im->getHeight(), im->getRawDataPtr());
		std::vector<Vec3<float>*>  ne;
		Vec3<float> pix;
		std::vector<float> reds, greens, blues;
		int n;

		for (int i = 0; i < im->getHeight(); i++){
			for (int j = 0; j < im->getWidth(); j++){
				ne = findNeighbors(im, i, j);
				for (int m = 0; m < ne.size(); m++){
					reds.push_back(ne[m]->r);
					greens.push_back(ne[m]->g);
					blues.push_back(ne[m]->b);
				}//for

				n = ne.size() / 2;
				std::nth_element(reds.begin(), reds.begin() + n, reds.end());
				std::nth_element(greens.begin(), greens.begin() + n, greens.end());
				std::nth_element(blues.begin(), blues.begin() + n, blues.end());

				pix.r = reds[n];
				pix.g = greens[n];
				pix.b = blues[n];
				imageF->setPixel(i, j, pix);

				ne.clear();
				reds.clear();
				greens.clear();
				blues.clear();
				pix.r = 0.0f;
				pix.g = 0.0f;
				pix.b = 0.0f;

			}//for
		}//for

		return imageF;
	};


	

	//--------CONSTRUCTOR-------//
	Filter::Filter() {};

	//--------DESTRUCTOR--------//
	Filter::~Filter() {};

}//namespace