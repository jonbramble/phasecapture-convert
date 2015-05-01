/*
 * RawRead.cpp
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

#include <RawReader.h>

RawReader::RawReader(const char* hdf5filename) {

	// check file type - is it hdf5, is it a phasecapture hdf5 file

	// should be in try block?

	const char* DATASET_NAME = "I1";							// change this for suitable raw dataset name

	h5file = new H5File(hdf5filename,H5F_ACC_RDONLY);			// open file// do I need the new delete here
	dataset = h5file->openDataSet( DATASET_NAME );
	dataspace = dataset.getSpace();
}

int RawReader::getFrameCount(){
	// how many frames are there to read?
	hsize_t dims_out[2];
	int ndims = dataspace.getSimpleExtentDims( dims_out, NULL);
    return dims_out[0];
}

void RawReader::getFrame(const int index, Frame *_frame){
	// create a hyperslab in the file
	hsize_t      offset[3];   // hyperslab offset in the file
	hsize_t      count[3];    // size of the hyperslab in the file

	hsize_t dims_out[2];
	int ndims = dataspace.getSimpleExtentDims( dims_out, NULL);

	//protect for out of range - might be a better way with exceptions
	if(index < dims_out[0])
	{

	/*
	* Define hyperslab in the dataset; implicitly giving strike and
	* block NULL.
	*/

	offset[0] = index;
	offset[1] = 0;
	offset[2] = 0;

	count[0]  = 1;
	count[1]  = IMAGE::NX;
	count[2]  = IMAGE::NY;

	dataspace.selectHyperslab( H5S_SELECT_SET, count, offset );

	/*
	* Define the memory dataspace in 2D
	*/
	hsize_t     dimsm[2];              /* memory space dimensions */
	dimsm[0] = IMAGE::NX;
	dimsm[1] = IMAGE::NY;
	DataSpace memspace( 2, dimsm );

	/*
	* Define memory hyperslab in 2D
	*/
	hsize_t      offset_out[2];   // hyperslab offset in memory
	hsize_t      count_out[2];    // size of the hyperslab in memory
	offset_out[0] = 0;
	offset_out[1] = 0;
	count_out[0]  = IMAGE::NX;
	count_out[1]  = IMAGE::NY;
	memspace.selectHyperslab( H5S_SELECT_SET, count_out, offset_out );

	// TEMP FOR DEBUG AND TEST
	/*
	* Read data from hyperslab in the file into the hyperslab in
	* memory and display the data.
	*/

	// 2D array for temp storage
	int data_out[IMAGE::NX][IMAGE::NY];

	dataset.read( data_out, PredType::NATIVE_INT, memspace, dataspace );

	for (int j = 0; j < IMAGE::NY; j++){
		   for (int i = 0; i < IMAGE::NX; i++)
		       _frame->frame_data[i+j*IMAGE::NX] = data_out[j][i];		//frame storage is linear
	}
	}// end if
	// TEMP FOR DEBUG AND TEST


}

void RawReader::getFrames(const int nframes, const int index, std::vector<Frame> &_frames){

	// have to get n frames starting at offset but cope with end of file

	//&& nframes == _frames.size()

	// the hyperslab on out could be 3D, but perhaps sequential 2D is better.

	// create a hyperslab in the file
	hsize_t      offset[3];   // hyperslab offset in the file
	hsize_t      count[3];    // size of the hyperslab in the file

	hsize_t dims_out[2];
	int ndims = dataspace.getSimpleExtentDims( dims_out, NULL);

	//protect for out of range - might be a better way with exceptions
	if(index < dims_out[0] )
		{
		std::cout << "run frames " << std::endl;

		/*
		* Define hyperslab in the dataset; implicitly giving strike and
		* block NULL.
		*/
		offset[0] = index;		// this needs to move along in steps of nframes
		offset[1] = 0;
		offset[2] = 0;

		count[0]  = nframes;
		count[1]  = IMAGE::NX;
		count[2]  = IMAGE::NY;

		dataspace.selectHyperslab( H5S_SELECT_SET, count, offset );

		/*
		* Define the memory dataspace in 3D
		*/
		hsize_t dimsm[3];              /* memory space dimensions */
		dimsm[0] = nframes;
		dimsm[1] = IMAGE::NX;
		dimsm[2] = IMAGE::NY;
		DataSpace memspace( 3, dimsm );

		/*
		* Define memory hyperslab in 3D
		*/
		hsize_t      offset_out[3];   // hyperslab offset in memory
		hsize_t      count_out[3];    // size of the hyperslab in memory
		offset_out[0] = 0;
		offset_out[1] = 0;
		offset_out[2] = 0;
		count_out[0]  = nframes;
		count_out[1]  = IMAGE::NX;
		count_out[2]  = IMAGE::NY;
		memspace.selectHyperslab( H5S_SELECT_SET, count_out, offset_out );

		// TEMP FOR DEBUG AND TEST
		/*
		* Read data from hyperslab in the file into the hyperslab in
		* memory and display the data.
		*/

		// 3D array for temp storage
		int data_out[nframes][IMAGE::NX][IMAGE::NY];

		dataset.read( data_out, PredType::NATIVE_INT, memspace, dataspace );

		// copy the data from the memory space into the Frame vector with linear array storage
		for (int k =0; k < nframes; k++){
			for (int j = 0; j < IMAGE::NY; j++){
			   for (int i = 0; i < IMAGE::NX; i++){
				   _frames[k].frame_data[i+j*IMAGE::NX] = data_out[k][j][i];
			   }
			}
		}

		}// end if
}

RawReader::~RawReader() {
	// close the file
	h5file->close();
	delete h5file;
}
