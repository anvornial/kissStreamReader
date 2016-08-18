#ifndef kissStreamReader_h
#define kissStreamReader_h

#include <Arduino.h>

bool sendAndExpectString(
	Stream& stream,
	const String& sendStr, 
	const String& untilStr, 
	unsigned long waitTime=2000, 
	int retryCount = 1);

String sendAndReadStringUntil(
	Stream& stream,
	const String& sendStr, 
	const String& untilStr, 
	unsigned long waitTime=2000, 
	int retryCount = 1);

void streamClearInputBuffer(Stream& stream);
#endif
