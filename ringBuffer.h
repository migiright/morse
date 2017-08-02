#pragma once

typedef struct {
	char morse;
	const char *textPos;
} MorseTuple;

void ringBuffer_add(MorseTuple object);
void ringBuffer_removeFirst();
MorseTuple *ringBuffer_read(unsigned index);
int ringBuffer_size();
