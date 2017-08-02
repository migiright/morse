#include "RingBuffer.h"

#define CAPACITY 30

static MorseTuple buffer[CAPACITY];
static MorseTuple *first = buffer, *end = buffer;

MorseTuple *forward(MorseTuple *pos, int n){
	pos += n;
	if(pos - buffer >= CAPACITY){
		pos = buffer + (pos - buffer) % CAPACITY;
	}
	return pos;
}

void ringBuffer_add(MorseTuple object)
{
	*end = object;
	end = forward(end, 1);
}

void ringBuffer_removeFirst()
{
	first = forward(first, 1);
}

MorseTuple *ringBuffer_read(unsigned index)
{
	return forward(first, index);
}

int ringBuffer_size()
{
	return (CAPACITY + (end - first)) % CAPACITY;
}
