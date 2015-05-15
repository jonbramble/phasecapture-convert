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
	//void static_cast_valarray(std::valarray<float>& outArray, const std::valarray<uint16_t>& inArray);

	static float static_cast_op(const uint16_t);

public:
	Frame();
	virtual ~Frame();

	std::valarray<uint16_t> frame_data;

	std::valarray<uint16_t> I1,I2,I3,I4;
	std::valarray<double> DC,Amp,Real,Phase; //need to change type here

    void process();

	/*void get_ave_I1(uint16_t&);
	void get_ave_I2(uint16_t&);
	void get_ave_I3(uint16_t&);
	void get_ave_I4(uint16_t&);

	void get_ave_dc(float&);
	void get_ave_amp(float&);
	void get_ave_phase(float&);
	void get_ave_real(float&);*/

};

#endif /* FRAME_H_ */
