/*
 * Frame.h
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

#include <valarray>
#include <iostream>
#include <algorithm>
#include <iterator>
#include "phasecapture_constants.h"

#ifndef FRAME_H_
#define FRAME_H_

class Frame {

private:
	long int sum(const std::valarray<uint16_t> &sub_frame);
	float sum(const std::valarray<float> &sub_frame);

	void process_raw_trig();
	void process_raw_dft();

	static double static_cast_op(const uint16_t);

public:
	Frame();
	virtual ~Frame();

	static void average_frames(const std::vector<Frame> frames, Frame &sub_frame);

	std::valarray<uint16_t> frame_data_i;	// raw data from file
	std::valarray<double> frame_data_d;		// double version

	std::valarray<double> I1,I2,I3,I4,DC,Amp,Real,Phase;; // spliced data
    void process();	// run the conversion

};

#endif /* FRAME_H_ */
