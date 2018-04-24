#include "monitor.h"
#ifndef SHARED_BUFFER_H
#define SHARED_BUFFER_H
class SharedBuffer : public Monitor
{
	int * buffer;
	Condition full, empty, writing, reading;
	size_t itemCount, maxItemCount;
	size_t nextToWrite, nextToRead;
	bool writingInProgress, readingInProgress;
	
	void printContents();
public:
	SharedBuffer(size_t size);
	~SharedBuffer();
	
	void write(size_t howMany, int what);
	int* read(size_t howMany);
};
#endif 