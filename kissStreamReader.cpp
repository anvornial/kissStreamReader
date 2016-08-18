#include <Arduino.h>
#include "kissStreamReader.h"


// Send string to stream object and return true if pattern string is found or return false
// if timeout is reach
// Use case : query an answer from AT command
bool sendAndExpectString(
	Stream& stream,
	const String& sendStr, 
	const String& untilStr, 
	unsigned long waitTime, 
	int retryCount){
	
	if(!untilStr.length()) return false;
	
	int indexComp = 0; //comparison index
	unsigned long previousMillis = 0;
	
	while(retryCount){
		if(sendStr.length())
			stream.print(sendStr);
		
		previousMillis = millis();
		while(millis() - previousMillis <= waitTime){
			if(stream.available()) {
				char inchar = (char) stream.read();				
				if(untilStr.charAt(indexComp) == inchar){
					//char position is equal
					indexComp = indexComp + 1;        
					if((indexComp + 1) > untilStr.length()){
						//its done
						return true;
					}
				} else if(untilStr.charAt(0) == inchar){
					//not equal, but it equal with first char
					//reset and +1
					indexComp = 1;
				} else {
					//not equal at all
					indexComp = 0;
				}
			}
			yield(); 
		}
		retryCount--;	
	}
	
	return false;
	
}


// Send string to stream object and return string read if pattern string is found or return empty
// string if timeout is reach
// Use case : send and read answer string from AT command
String sendAndReadStringUntil(
	Stream& stream,
	const String& sendStr, 
	const String& untilStr, 
	unsigned long waitTime, 
	int retryCount)
{
	if(!untilStr.length()) return "";
	
	String retString;
	retString.reserve(32);
	retString = "";
	
	int indexComp = 0; //comparison index
	unsigned long previousMillis = 0;
	
	while(retryCount){
		if(sendStr.length())
			stream.print(sendStr);
		previousMillis = millis();
		while(millis() - previousMillis <= waitTime){
			if(stream.available()) {
				char inchar = (char) stream.read();
				retString = retString + String(inchar);
				
				if(untilStr.charAt(indexComp) == inchar){
					//char position is equal					
					indexComp = indexComp + 1;        
					if((indexComp + 1) > untilStr.length()){
						//its done
						//trim if available
						if(retString.length() > untilStr.length()){					 
							retString.remove(retString.length()-untilStr.length());
							return retString;
						} else 
							return "";
					}
				} else if(untilStr.charAt(0) == inchar){
					//not equal, but it equal with first char
					//reset +1
					indexComp = 1;
				} else {
					//not equal at all
					indexComp = 0;
				}
			}
			yield(); 
		}
		retryCount--;
	}
	
	return "";
}

// forcefully clear input buffer from stream object
void streamClearInputBuffer(Stream& stream){
	int bufsize = stream.available();
	while(bufsize) {
		stream.read();
		bufsize--;
		yield();
	}
}
