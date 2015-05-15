/*
 * Frame.cpp
 *
 *  Created on: 29 Apr 2015
 *      Author: mbajb
 */
/*
PhaseCapture Convert - HDF5 file format for PhaseCapture Camera Data
    Copyright (C) 2015  Jonathan Bramble

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Frame.h"

Frame::Frame() {
	frame_data.resize(IMAGE::FRAME_SIZE);
	I1.resize(IMAGE::IMAGE_SIZE);
	I2.resize(IMAGE::IMAGE_SIZE);
	I3.resize(IMAGE::IMAGE_SIZE);
	I4.resize(IMAGE::IMAGE_SIZE);
	Amp.resize(IMAGE::IMAGE_SIZE);
	DC.resize(IMAGE::IMAGE_SIZE);

}

void Frame::process(){
	std::cout << "process " << std::endl;

	I1 = frame_data[std::slice(0,IMAGE::IMAGE_SIZE,1)];
	I2 = frame_data[std::slice(1*IMAGE::IMAGE_SIZE,IMAGE::IMAGE_SIZE,1)];
	I3 = frame_data[std::slice(2*IMAGE::IMAGE_SIZE,IMAGE::IMAGE_SIZE,1)];
	I4 = frame_data[std::slice(3*IMAGE::IMAGE_SIZE,IMAGE::IMAGE_SIZE,1)];

	//process others
	//choose method
	process_raw_trig();
}

Frame::~Frame() {
}

void Frame::process_raw_dft(){
	// parallel DFT version
	// Do matrix DFT conversion here on spliced vectors

}

void Frame::process_raw_trig(){
	// element wise operations on arrays

	// this is still far too slow!	// allocates memory on each run, could preallocate this  in constructor?
	std::valarray<float> xf(IMAGE::IMAGE_SIZE), yf(IMAGE::IMAGE_SIZE), xf2(IMAGE::IMAGE_SIZE),
		yf2(IMAGE::IMAGE_SIZE), zf(IMAGE::IMAGE_SIZE), phradf(IMAGE::IMAGE_SIZE), i1f(IMAGE::IMAGE_SIZE), i2f(IMAGE::IMAGE_SIZE), i3f(IMAGE::IMAGE_SIZE), i4f(IMAGE::IMAGE_SIZE);

	static_cast_valarray(i1f,I1);
	static_cast_valarray(i2f,I2);
	static_cast_valarray(i3f,I3);
	static_cast_valarray(i4f,I4);

	// but these could be -ve
	std::valarray<float> y = i1f-i3f;
	std::valarray<float> x = i2f-i4f;
	std::valarray<float> s(2.0, IMAGE::IMAGE_SIZE);

	// element wise operations on arrays
	phradf = atan2(xf,yf);
	Phase = phradf;
	Phase *= 180/M_PI;

	xf2 = std::pow(xf,s);
	yf2 = std::pow(yf,s);
	zf = xf2+yf2;

	//confusion over amp here
	Amp = std::sqrt(zf);
	Amp /= 2;

	Real = std::cos(phradf)*Amp;

	Amp /= 4;

	DC = i1f+i2f+i3f+i4f;
	DC /= 4;
}

void Frame::static_cast_valarray(std::valarray<float>& outArray, const std::valarray<uint16_t>& inArray){
	for(int k=0;k<inArray.size();k++) outArray[k]= static_cast<float>(inArray[k]);
}

