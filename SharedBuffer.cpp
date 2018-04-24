#include "monitor.h"
#include "SharedBuffer.h"
#include "print.h"
#include <cstdlib>

SharedBuffer::SharedBuffer(size_t size) 
{
	if(size == 0)
		throw "Attempt to create a null-size buffer\n";
	
	buffer = new int[size]; 
	itemCount = 0; 
	maxItemCount = size;
	nextToWrite = 0; 
	nextToRead = 0; 
	writingInProgress = false;
	readingInProgress = false;
	for(int i = 0; i < maxItemCount; i++)
	{
		buffer[i] = 0;
	}
}

SharedBuffer::~SharedBuffer()
{
	delete [] buffer;
}

void SharedBuffer::write(size_t howMany, int what)
{
	enter();
	debug("Writer entered\n");
	if(writingInProgress)
		wait(writing);
	debug("Writer starts writing\n");
	writingInProgress = true;
	for(size_t i = 0; i < howMany; i++)
	{
		debug("Writer tries to write\n");
		if(itemCount == maxItemCount)
			wait(full);
		debug("Writer writes\n");
		itemCount++;
		buffer[nextToWrite++] = what;
		nextToWrite %= maxItemCount;
		printContents();
		// if(itemCount == 1) // not really needed
		signal(empty);
	}
	debug("Writer ends writing\n");
	writingInProgress = false;
	signal(writing);
	debug("Writer leaves \n");
	leave();
}

int * SharedBuffer::read(size_t howMany)
{
	enter();
	debug("Reader entered\n");
	if(howMany == 0)
	{
		leave();
		return nullptr;
	}
	
	if(readingInProgress)
		wait(reading);
	debug("Reader starts reading\n");
	int* arrayToBeReturned = new int[howMany];
	readingInProgress = true;
	for(size_t i = 0; i < howMany; i++)
	{
		debug("Reader tries to read\n");
		if(itemCount == 0)
			wait(empty);
		debug("Reader reads\n");
		itemCount--;
		arrayToBeReturned[i] = buffer[nextToRead++];
		nextToRead %= maxItemCount;
		printContents();
		//if(itemCount == maxItemCount - 1) // not really needed
		signal(full);
	}
	debug("Reader ends reading\n");
	readingInProgress = false;
	signal(reading);
	leave();
	debug("Reader leaves\n");
	return arrayToBeReturned;
}

void SharedBuffer::printContents()
{
	int i = nextToRead;
	size_t count = 0;
	if(itemCount == 0)
		print("empty");
	while(count < itemCount)
	{
		print("%d ", buffer[i]);
		i++;
		i %= maxItemCount;
		count++;
	}
	print("\n");
	return;
}