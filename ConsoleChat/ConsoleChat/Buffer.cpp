#include "Buffer.h"
#define MAX_BUFFER_SIZE 1500
#include <algorithm>
#include <iostream>

#define MAX_GROW 100

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


void Buffer::WriteInt32LE(unsigned int index, int value) {
	// Is it large enough?
	if (index <= this->mBuffer.size() - 4) this->mBuffer.resize(index + MAX_GROW);

	char firstByteBE = (value & 0x000000FF) >> 0;
	char secondByteBE = (value & 0x0000FF00) >> 8;
	char thirdByteBE = (value & 0x00FF0000) >> 16;
	char fourthByteBE = (value & 0xFF000000) >> 24;

	this->mBuffer[index + 0] = firstByteBE;
	this->mBuffer[index + 1] = secondByteBE;
	this->mBuffer[index + 2] = thirdByteBE;
	this->mBuffer[index + 3] = fourthByteBE;
}

void Buffer::WriteInt32LE(int value) {
	// Is large enough?
	if (this->mWriteIndex <= this->mBuffer.size() - 4)
		this->mBuffer.resize(this->mWriteIndex + MAX_GROW);

	char firstByteBE = (value & 0x000000FF) >> 0;
	char secondByteBE = (value & 0x0000FF00) >> 8;
	char thirdByteBE = (value & 0x00FF0000) >> 16;
	char fourthByteBE = (value & 0xFF000000) >> 24;

	this->mBuffer[this->mWriteIndex + 0] = firstByteBE;
	this->mBuffer[this->mWriteIndex + 1] = secondByteBE;
	this->mBuffer[this->mWriteIndex + 2] = thirdByteBE;
	this->mBuffer[this->mWriteIndex + 3] = fourthByteBE;

	this->mWriteIndex += 4;
}

void Buffer::WriteInt16LE(unsigned int index, short value) {
	// Is large enough?
	if (index <= this->mBuffer.size() - 2) this->mBuffer.resize(index + MAX_GROW);

	char firstByteBE = (value & 0x00FF) >> 0;
	char secondByteBE = (value & 0xFF00) >> 8;

	this->mBuffer[index + 0] = firstByteBE;
	this->mBuffer[index + 1] = secondByteBE;
}

void Buffer::WriteInt16LE(short value) {
	// Is large enough?
	if (this->mWriteIndex <= this->mBuffer.size() - 2)
		this->mBuffer.resize(this->mWriteIndex + MAX_GROW);

	char firstByteBE = (value & 0x00FF) >> 0;
	char secondByteBE = (value & 0xFF00) >> 8;

	this->mBuffer[mWriteIndex + 0] = firstByteBE;
	this->mBuffer[mWriteIndex + 1] = secondByteBE;

	this->mWriteIndex += 2;
}