#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <pthread.h>
#include <ctime>
#include <cstdlib>
#include "SharedBuffer.h"
#include "print.h"

using threadID = pthread_t;
using threadArguments = void*;

const int BUFFER_SIZE = 3;
const int ITERATIONS = 1;

const int WRITERS = 3;
const int WRITE_SIZE = 4;

const int READERS = 2;
const int READ_SIZE = 5;

SharedBuffer sharedBuffer(BUFFER_SIZE);

void* writerMain(void* what)
{
	int value = *((int *) what);
	for(int i = 0; i < ITERATIONS; i++)
	{
		sharedBuffer.write(WRITE_SIZE, value);
	}
	debug("Writer thread ends\n");
}

void* readerMain(void* what)
{
	int value = *((int *) what);
	std::string filename = std::to_string(value) + ".txt";
	std::ofstream file;
	file.open(filename);
	for(int i = 0; i < ITERATIONS; i++)
	{
		int* dataFromBuffer = sharedBuffer.read(READ_SIZE);
		for(int j = 0; j < READ_SIZE; j++)
		{
			file << dataFromBuffer[j] << "\n";
		}
		delete [] dataFromBuffer;
	}
	file.close();
	debug("Reader thread ends\n");
}

int main(int argc, char** argv)
{
	if(READERS * READ_SIZE > WRITERS * WRITE_SIZE)
	{
		print("Error:\nWriters won't produce enough data.\nReaders will be waiting for data endlessly.\n");
		exit(1);
	}
	else if(WRITERS * WRITE_SIZE * ITERATIONS - ITERATIONS * READERS * READ_SIZE > BUFFER_SIZE)
	{
		print("Error:\nWriters will produce too much data.\nThey will be waiting for buffer to be freed endlessly.\n");
		exit(1);
	}
	
	std::vector<threadID> threads;
	
	int* whatToWriteVector = new int[WRITERS];
	int* readerIDs = new int[READERS];
	
	for(int i = 0; i < WRITERS; i++)
	{
		threadID id;
		whatToWriteVector[i] = i;
		auto status = pthread_create(&id, nullptr, writerMain, (void*)(whatToWriteVector + i));
		if(status != 0) // error
			exit(1);
		
		debug("Consumer thread created\n");
		threads.push_back(id);
	}
	
	for(int i = 0; i < READERS; i++)
	{
		threadID id;
		readerIDs[i] = i;
		auto status = pthread_create(&id, nullptr, readerMain, (void*)(readerIDs + i));
		if(status != 0) // error
			exit(1);
			
		debug("Producer thread created\n");
		threads.push_back(id);
	}
	
	
	
	for(auto& thread: threads)
	{
		pthread_join(thread, nullptr);
	}
	
	delete [] whatToWriteVector;
	delete [] readerIDs;
	
	return 0;
}