
#include "BufferManager.h"

//--------------------------------------------------------------
BufferManager::BufferManager() : OscNodeListener("/buffers") {
    addOscCommand("new");
}

//--------------------------------------------------------------
BufferManager::~BufferManager() {
    
}

//--------------------------------------------------------------
void BufferManager::update() {
}

//--------------------------------------------------------------
void BufferManager::processOscMessage(string pattern, ofxOscMessage& m) {
    
}

//--------------------------------------------------------------
FrameBuffer* BufferManager::getNewVideoBuffer(int _bufferSize) {
    
}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------