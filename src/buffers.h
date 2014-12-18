/*
 * buffers.h - buffer pool management.
 *
 *  Created on: 18 Dec 2014
 *      Author: matt
 */

#ifndef BUFFERS_H_
#define BUFFERS_H_

class BufferManager {
public:

	BufferManager(const int bufferSizeInBytes, const int maxBuffers);
	~BufferManager();
};



#endif /* BUFFERS_H_ */
