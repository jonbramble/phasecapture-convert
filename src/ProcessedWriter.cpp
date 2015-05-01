/*
 * ProcessedWrite.cpp
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

#include "ProcessedWriter.h"

ProcessedWriter::ProcessedWriter(const char* hdf5filename) {
	// create the main file
	// create the data sets for each part

	hsize_t chunk_dims[3];
	hsize_t maxdims[3];
	hsize_t dimsf[3];

	maxdims[0] = H5S_UNLIMITED;
	maxdims[1] = IMAGE::NX;
	maxdims[2] = IMAGE::NY;

	dimsf[0] = 1;
	dimsf[1] = IMAGE::NX;
	dimsf[2] = IMAGE::NY;

	//chunk dims must be of same rank as dataset
	chunk_dims[0] = 1;
	chunk_dims[1] = IMAGE::NX;
	chunk_dims[2] = IMAGE::NY;

	const H5std_string DATASETNAME("RAW");

	IntType datatype( PredType::NATIVE_UINT16 );
	datatype.setOrder( H5T_ORDER_LE );

	//Set the chunk properties
	DSetCreatPropList prop;
	prop.setChunk(3, chunk_dims);

	DataSpace dataspace( 3, dimsf, maxdims );
	h5file = new H5File( hdf5filename, H5F_ACC_TRUNC );

	//Have a dataset for each image segment I0 -> I3 and conversions
	dataset = new DataSet(h5file->createDataSet( DATASETNAME, datatype, dataspace, prop ));

}

void ProcessedWriter::write_raw(const int offset, const Frame &frame){

	// need to include the offset here

	// reshape the data into 2D format
	int i, j;
	int data[IMAGE::NX][IMAGE::NY];          // buffer for data to write
	for (j = 0; j < IMAGE::NX; j++)
	{
	   for (i = 0; i < IMAGE::NY; i++){
		   data[j][i] = frame.frame_data[i+j*IMAGE::NX];
	   }
	}

	//is it possible to write the linear data directly to dataset without reshaping
	dataset->write(data,PredType::NATIVE_INT);
}

ProcessedWriter::~ProcessedWriter() {
	h5file->close();
	delete dataset;
	delete h5file;
}

