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

using namespace std;

int main(void) {

	// all this will be in a interface function later with args for files, averaging and processing options

	// create a read only reader object
	RawReader *raw_reader = new RawReader("PhaseCaptureHDF5.h5");

	// create the output file
	ProcessedWriter *processed_writer = new ProcessedWriter("PhaseCaptureOUT.h5");

	// create a new frame
	Frame *aframe = new Frame();

	int count = raw_reader->getFrameCount();
	std::cout << "count " << count << std::endl;

	//process through each frame and write to raw dataset - a simple copy
	for(int k=0;k<count;k++)
	{
		raw_reader->getFrame(k,aframe);			// this reads a single frame
		processed_writer->write_raw(k,*aframe);
	}

	int n = 3;
	std::vector<Frame> sframes(n);			//create a vector for averaging over multiple frames
	raw_reader->getFrames(n,0,sframes);		// this reads sets of n frames

	delete aframe;
    delete raw_reader;				// closes file
    delete processed_writer;		// closes file

	return 0;
}
