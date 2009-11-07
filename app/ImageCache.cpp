/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 */

#include "ImageCache.hpp"
#include <stdio.h>

namespace App {

ImageCache::ImageCache()
{
	// Nothing
}

ImageCache::~ImageCache()
{
	delAll();
}

bool ImageCache::exists(std::string name) 
{
	return (bool)cache.count(name);
}

Cv::Image* ImageCache::get(std::string name)
{
	if(!exists(name)) {
		return 0;
	}
	return cache[name];
}

void ImageCache::set(std::string name, Cv::Image* img)
{
	if(exists(name)) {
		fprintf(stderr, "ImageCache::setMap() Key already exists in map.\n");
		return;
	}
	cache[name] = img;
}

Cv::Image* ImageCache::pop(std::string name)
{
	Cv::Image* temp = 0;

	if(!exists(name)) {
		return 0;
	}

	cache.erase(name);
	return temp;
}

void ImageCache::del(std::string name)
{
	Cv::Image* temp = 0;

	temp = pop(name);

	if(temp != NULL) {
		delete temp;
	}
}

void ImageCache::delAll()
{
	std::map<std::string, Cv::Image*>::iterator it;
	for(it = cache.begin(); it != cache.end(); it++) {
		del((*it).first);
	}
}

} // end namespace App
