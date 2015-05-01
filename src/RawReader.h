/*
 * RawRead.h
 *
 *  Created on: 29 Apr 2015
 *      Author: mbajb
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
	void getFrame(const int index, Frame *_frame);
	void getFrames(const int frames,const int index, std::vector<Frame> &_frames);

private:
	H5File *h5file;
	int dims_frame;
	DataSet dataset;
	DataSpace dataspace;

};

#endif /* RAWREADER_H_ */
