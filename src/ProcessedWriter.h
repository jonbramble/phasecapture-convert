/*
 * ProcessedWrite.h
 *
 *  Created on: 29 Apr 2015
 *      Author: mbajb
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
