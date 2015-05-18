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

Frame::Frame() : frame_data_d(0.0,IMAGE::FRAME_SIZE)
	{
	frame_data_i.resize(IMAGE::FRAME_SIZE);

	I1.resize(IMAGE::IMAGE_SIZE);
	I2.resize(IMAGE::IMAGE_SIZE);
	I3.resize(IMAGE::IMAGE_SIZE);
	I4.resize(IMAGE::IMAGE_SIZE);

	Amp.resize(IMAGE::IMAGE_SIZE);
	DC.resize(IMAGE::IMAGE_SIZE);
	Phase.resize(IMAGE::IMAGE_SIZE);
	Real.resize(IMAGE::IMAGE_SIZE);
}

void Frame::process(){
	// all the data needs to be cast as double for trig functions
	std::transform(std::begin(frame_data_i), std::end(frame_data_i), std::begin(frame_data_d), static_cast_op);

	//slice up the data
	I1 = frame_data_d[std::slice(0,IMAGE::IMAGE_SIZE,1)];
	I2 = frame_data_d[std::slice(1*IMAGE::IMAGE_SIZE,IMAGE::IMAGE_SIZE,1)];
	I3 = frame_data_d[std::slice(2*IMAGE::IMAGE_SIZE,IMAGE::IMAGE_SIZE,1)];
	I4 = frame_data_d[std::slice(3*IMAGE::IMAGE_SIZE,IMAGE::IMAGE_SIZE,1)];

	//choose method
	//process_raw_trig();
	process_raw_dft();

}

Frame::~Frame() {
}

void Frame::process_raw_dft(){
	// parallel DFT version
	// Do matrix DFT conversion here on spliced vectors

	// serial version first

	// F0
	DC = I1+I2+I3+I4;
	DC /= 4;

	//F3
	std::valarray<double> F3real = I1-I3;
	std::valarray<double> F3img = I2-I4;
	std::valarray<double> phradf = atan2(F3img,F3real);

	Phase = phradf;
	Phase *= 180/M_PI;
	Amp = F3real/(2*std::cos(phradf));
}

void Frame::process_raw_trig(){
	// element wise operations on arrays

	// but these could be -ve
	std::valarray<double> y = I1-I3;
	std::valarray<double> x = I2-I4;
	std::valarray<double> s(2.0, IMAGE::IMAGE_SIZE);

	// element wise operations on arrays
	std::valarray<double> phradf = atan2(x,y);

	Phase = phradf;
	Phase *= 180/M_PI;

	std::valarray<double> xf2 = std::pow(x,s);
	std::valarray<double> yf2 = std::pow(y,s);
	std::valarray<double> zf = xf2+yf2;

	//confusion over amp here
	Amp = std::sqrt(zf);
	Amp /= 2;

	Real = std::cos(phradf)*Amp;

	DC = I1+I2+I3+I4;
	DC /= 4;
}

void Frame::average_frames(const std::vector<Frame> frames, Frame &sub_frame){
	// iterate over a number of frames
	// do the averaging and rounding
	// return single frame

	// single thread at first

	// tmp write through
	int size = frames.size();
	std::valarray<double> tmp(IMAGE::FRAME_SIZE);
	std::valarray<uint16_t> frame_data(IMAGE::FRAME_SIZE);

	// alternative impl using valarray
	for(int f=0;f<size;f++)
		sub_frame.frame_data_d += frames[f].frame_data_d;


}

double Frame::static_cast_op(const uint16_t in) { return static_cast<double>(in); }





// simple method at first

/*for(int k=0;k<IMAGE::FRAME_SIZE;k++){
	// we do rounding here for now - but will change with additional of double frame_data array
	long tmp = 0;
	for(int f=0;f<size;f++)
	{
		tmp += frames[f].frame_data[k];
	}

	sub_frame.frame_data[k] = std::round(tmp/size);
}*/
