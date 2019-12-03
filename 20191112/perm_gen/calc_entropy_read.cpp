/* calc_entropy_read.cpp
Defines the file reading interface of the calc-entropy application.
Uses multiple buffers and a helper thread to read data from a file.

Copyright(c) 2019 David Krikheli

Modification history:
	3/Dec/2019 - David Krikheli created the module.
*/

#include <atomic>
#include <thread>
#include <cctype>
#include "calc_entropy_read.hpp"

using namespace std;
using namespace dk;

static const size_t iInputDataBufferSize{ 8192 };
static const size_t iNumBuffers{ 5 };

static struct {
	char         cBuffer[iInputDataBufferSize];
	size_t       iInputSize{ 0 };
	bool         bNotEOF{ true };
	atomic<bool> abBufferReady{ false };
} buffers[iNumBuffers];

static void countBytes(bool bBinary, CECounterType* pCounter) {
	size_t inxBuffer{ 0 };
	bool bNotEOF{ true };

	while (bNotEOF) {
		auto& buffer = buffers[inxBuffer];
		while (!bool(buffer.abBufferReady))
			std::this_thread::yield();

		bNotEOF = buffer.bNotEOF;
		size_t iInputSize = buffer.iInputSize;
		char* cBuffer = buffer.cBuffer;

		unsigned char symbol{ 0 };

		if (bBinary)
			for (size_t inx = 0; inx < iInputSize; inx++) {
				symbol = static_cast<unsigned char>(cBuffer[inx]);
				(*pCounter)[symbol] ++;
			}
		else
			for (size_t inx = 0; inx < iInputSize; inx++) {
				symbol = static_cast<unsigned char>(cBuffer[inx]);
				if (isprint(symbol))
					(*pCounter)[symbol] ++;
			}

		buffer.abBufferReady.store(false);

		inxBuffer++;
		if (inxBuffer == iNumBuffers)
			inxBuffer = 0;
	}
}

void dk::readDataFromFile(ifstream& inputFile, bool bBinary,
	CECounterType& counter) {

	thread thr(countBytes, bBinary, &counter);

	size_t inxBuffer{ 0 };
	while (inputFile) {
		auto& buffer = buffers[inxBuffer];
		while (bool(buffer.abBufferReady))
			std::this_thread::yield();

		inputFile.read(buffer.cBuffer, iInputDataBufferSize);

		buffer.bNotEOF = bool(inputFile);
		buffer.iInputSize = inputFile.gcount();
		buffer.abBufferReady.store(true);

		inxBuffer++;
		if (inxBuffer == iNumBuffers)
			inxBuffer = 0;
	}

	thr.join();
}
