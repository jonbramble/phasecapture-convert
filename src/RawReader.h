/*
 * RawRead.h
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

#ifndef RAWREADER_H_
#define RAWREADER_H_

#include <H5Cpp.h>
#include <Frame.h>
#include <vector>
#include "phasecapture_constants.h"
#include <iostream>

#ifndef H5_NO_NAMESPACE
    using namespace H5;
#endif

class RawReader {
public:
	RawReader(const char*);
	virtual ~RawReader();
	int getFrameCount();
	void getFrame(const int index, Frame &_frame);
	void getFrameArray(const int index, Frame &_frame);
	void getFrames(const int frames,const int index, std::vector<Frame> &_frames);

private:
	H5File *h5file;
	int dims_frame;
	DataSet dataset;
	DataSpace dataspace;

};

#endif /* RAWREADER_H_ */
