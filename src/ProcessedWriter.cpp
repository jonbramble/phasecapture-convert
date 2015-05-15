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

	// create the main file
	// create the data sets for each part
ProcessedWriter::ProcessedWriter(const char* hdf5filename){

	h5file = new H5File( hdf5filename, H5F_ACC_TRUNC );

	maxdims[0] = H5S_UNLIMITED;		// this dim is known for fixed input file
	maxdims[1] = IMAGE::NX;
	maxdims[2] = IMAGE::NY;

	dimsf[0] = 100;					// should set this or we will have zeros in the file - will need to calculate the averaging
	dimsf[1] = IMAGE::NX;
	dimsf[2] = IMAGE::NY;

	//chunk dims must be of same rank as dataset
	//a frame is a chunk
	chunk_dims[0] = 1;
	chunk_dims[1] = IMAGE::NX;
	chunk_dims[2] = IMAGE::NY;

	const H5std_string dataset_RW_name = "RW";
	const H5std_string dataset_I0_name = "I0";
	const H5std_string dataset_I1_name = "I1";
	const H5std_string dataset_I2_name = "I2";
	const H5std_string dataset_I3_name = "I3";
	const H5std_string dataset_DC_name = "DC";
	const H5std_string dataset_PH_name = "PH";
	const H5std_string dataset_AM_name = "AM";
	const H5std_string dataset_RE_name = "RE";

	//Set the chunk properties
	DSetCreatPropList prop;
	prop.setChunk(3, chunk_dims);

	IntType raw_datatype( PredType::NATIVE_SHORT );		//needs to change to float for real data
	FloatType datatype ( PredType::NATIVE_FLOAT);
	datatype.setOrder( H5T_ORDER_LE );
	memspace = new DataSpace(3, chunk_dims, NULL);  // create memoryspace geometries
	dataspace = new DataSpace( 3, dimsf, maxdims ); // create dataspace geometries

	//create the datasets
	dataset_raw = new DataSet(h5file->createDataSet( dataset_RW_name, raw_datatype, *dataspace, prop ));
	dataset_I0 = new DataSet(h5file->createDataSet( dataset_I0_name, datatype, *dataspace, prop ));
	dataset_I1 = new DataSet(h5file->createDataSet( dataset_I1_name, datatype, *dataspace, prop ));
	dataset_I2 = new DataSet(h5file->createDataSet( dataset_I2_name, datatype, *dataspace, prop ));
	dataset_I3 = new DataSet(h5file->createDataSet( dataset_I3_name, datatype, *dataspace, prop ));
	dataset_dc = new DataSet(h5file->createDataSet( dataset_DC_name, datatype, *dataspace, prop ));
	dataset_phase = new DataSet(h5file->createDataSet( dataset_PH_name, datatype, *dataspace, prop ));
	dataset_amp = new DataSet(h5file->createDataSet( dataset_AM_name, datatype, *dataspace, prop ));
	dataset_re = new DataSet(h5file->createDataSet( dataset_RE_name, datatype, *dataspace, prop ));

	prop.close();
	datatype.close();
	raw_datatype.close();

}

// this writes raw frame data to raw dataset
void ProcessedWriter::write_raw(const int offset, const Frame &frame){
	// need to include the offset here

	hsize_t shift[3];
	hsize_t dim_out[3];

	shift[0] = offset;
	shift[1] = 0;
	shift[2] = 0;

	// reshape the data into 2D format
	int i, j;
	short data[IMAGE::NX][IMAGE::NY];          // buffer for data to write
	for (j = 0; j < IMAGE::NX; j++)
	{
	   for (i = 0; i < IMAGE::NY; i++){
		   data[j][i] = frame.frame_data[i+j*IMAGE::NX];
	   }
	}

	dataspace->getSimpleExtentDims( dim_out, NULL);

	if(offset < dim_out[0]){ // can remove this later
		dataspace->selectHyperslab(H5S_SELECT_SET, chunk_dims, shift);  // problem here with offset
		//is it possible to write the linear data directly to dataset without reshaping
		dataset_raw->write(data,PredType::NATIVE_SHORT, *memspace, *dataspace);
	}
}

void ProcessedWriter::write_frame(const int offset, const Frame &frame){

	//std::cout << "write frame " << offset << std::endl;

	// here we must write to the different datasets each processed component of the frame
	// need to include the offset here

	hsize_t shift[3];
	hsize_t dim_out[3];

	shift[0] = offset;
	shift[1] = 0;
	shift[2] = 0;

	// reshape the data into 2D format
	int i, j;
	float I0_tmp[IMAGE::NX][IMAGE::NY];          // buffer for data to write
	float I1_tmp[IMAGE::NX][IMAGE::NY];
	float I2_tmp[IMAGE::NX][IMAGE::NY];
	float I3_tmp[IMAGE::NX][IMAGE::NY];
	float DC_tmp[IMAGE::NX][IMAGE::NY];
	float Amp_tmp[IMAGE::NX][IMAGE::NY];
	float Phase_tmp[IMAGE::NX][IMAGE::NY];

	for (j = 0; j < IMAGE::NX; j++)
	{
		for (i = 0; i < IMAGE::NY; i++){//check orientations
			I0_tmp[j][i] = frame.I1[i+j*IMAGE::NX];
			I1_tmp[j][i] = frame.I2[i+j*IMAGE::NX];
			I2_tmp[j][i] = frame.I3[i+j*IMAGE::NX];
			I3_tmp[j][i] = frame.I4[i+j*IMAGE::NX];
			DC_tmp[j][i] = frame.DC[i+j*IMAGE::NX];
			Amp_tmp[j][i] = frame.Amp[i+j*IMAGE::NX];
			Phase_tmp[j][i] = frame.Phase[i+j*IMAGE::NX];
		 }
	}

	dataspace->getSimpleExtentDims( dim_out, NULL);

	if(offset < dim_out[0]){ // can remove this later
		dataspace->selectHyperslab(H5S_SELECT_SET, chunk_dims, shift);
		dataset_I0->write(I0_tmp,PredType::NATIVE_FLOAT, *memspace, *dataspace);
		dataset_I1->write(I1_tmp,PredType::NATIVE_FLOAT, *memspace, *dataspace);
		dataset_I2->write(I2_tmp,PredType::NATIVE_FLOAT, *memspace, *dataspace);
		dataset_I3->write(I3_tmp,PredType::NATIVE_FLOAT, *memspace, *dataspace);
		dataset_dc->write(DC_tmp,PredType::NATIVE_FLOAT, *memspace, *dataspace);
		dataset_amp->write(Amp_tmp,PredType::NATIVE_FLOAT, *memspace, *dataspace);
		dataset_phase->write(Phase_tmp,PredType::NATIVE_FLOAT, *memspace, *dataspace);

	}
}

ProcessedWriter::~ProcessedWriter() {
	h5file->close();
	delete memspace;
	delete dataspace;
	delete dataset_raw, dataset_I0, dataset_I1, dataset_I2, dataset_I3, dataset_dc, dataset_phase, dataset_amp, dataset_re;
	delete h5file;
}
