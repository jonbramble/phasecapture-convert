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

#include <ProcessedWriter.h>
#include <RawReader.h>
#include <iostream>
#include <vector>

typedef unsigned short uint16_t;

using namespace std;

int main(void) {

	// all this will be in a interface function later with args for files, averaging and processing options

	// create a read only reader object
	RawReader *raw_reader = new RawReader("PhaseCaptureNA.h5");

	// create the output file
	ProcessedWriter *processed_writer = new ProcessedWriter("PhaseCaptureOUT.h5");

	// create a new frame
	Frame ave_frame;
	Frame read_frame;

	// this tells us the length of the file so we can calculate size of the output file
	int count = raw_reader->getFrameCount();
	std::cout << "count " << count << std::endl;

	int n = 5;											// set averaging with n for now

	std::vector<Frame> frames(n);						// create a vector to hold the frames

	int parts = count / n;
	int extra = count % n;								// these are discarded

	std::cout << parts << std::endl;

	for(int p=0;p<parts;p++){

		    // replace this with a reader that gets a 2d hyperslab
			for(int k=0;k<n;k++){
				raw_reader->getFrameArray(n*p+k,frames[k]);		// is there a neater way with iterators
			}

			Frame::average_frames(frames,ave_frame);
			ave_frame.process();								// process the frame once averaged -- could we avoid a rounding error with a float storage
			processed_writer->write_frame(p,ave_frame);			// write this frame to the output file
	}

	//TODO: Calculations of size of output

    delete raw_reader;				// closes file
    delete processed_writer;		// closes file

	return 0;
}
