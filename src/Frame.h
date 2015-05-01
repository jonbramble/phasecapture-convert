/*
 * Frame.h
 *
 *  Created on: 29 Apr 2015
 *      Author: mbajb
 */

#include <valarray>
#include <iostream>
#include "phasecapture_constants.h"

#ifndef FRAME_H_
#define FRAME_H_

class Frame {
public:
	Frame();
	virtual ~Frame();
	//int for testing
	std::valarray<int> frame_data;
};

#endif /* FRAME_H_ */
