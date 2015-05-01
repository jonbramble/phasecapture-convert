/*
 * ProcessedWrite.h
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

#ifndef PROCESSEDWRITER_H_
#define PROCESSEDWRITER_H_

#include <H5Cpp.h>
#include "Frame.h"

#ifndef H5_NO_NAMESPACE
    using namespace H5;
#endif

class ProcessedWriter {
public:
	ProcessedWriter(const char* );
	virtual ~ProcessedWriter();
	void write_raw(const int offset, const Frame &frame);

private:
	H5File *h5file;
	DataSet *dataset;
};

#endif /* PROCESSEDWRITER_H_ */
