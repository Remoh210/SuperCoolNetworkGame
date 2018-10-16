#include "Buffer.h"
#define MAX_BUFFER_SIZE 1500
#include <algorithm>
#include <iostream>


Buffer::Buffer(unsigned int size) : mWriteIndex(0), mReadIndex(0) {
	mBuffer.resize(size);
}

void Buffer::setBuffer(char* rcvBuffer, int bufferLenght) {
	this->mBuffer.resize(bufferLenght);
	for (int i = 0; i < bufferLenght; i++) {
		this->mBuffer[i] = rcvBuffer[i];
	}

	this->mReadIndex = 0;
	this->mWriteIndex = 0;
}

char* Buffer::getBuffer() {
	this->retBuffer = new char[this->mBuffer.size() + 1];
	for (int i = 0; i < this->mBuffer.size(); i++) {
		retBuffer[i] = this->mBuffer.at(i);
	}
	retBuffer[this->mBuffer.size()] = '\0';
	return this->retBuffer;
}

void Buffer::clearBuffer() {
	this->mBuffer = "";
	this->mReadIndex = 0;
	this->mWriteIndex = 0;
}