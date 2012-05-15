//
//  ofxLivedrawEngine.h
//  ofxLivedrawEngineExample
//
//  Created by Christopher Baker on 5/13/12.
//  Copyright (c) 2012 School of the Art Institute of Chicago. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "AssetManager.h"
#include "OscManager.h"
#include "FrameBuffer.h"

class BufferManager : public OscNodeListener {
public:
    
    BufferManager();
    virtual ~BufferManager();
    
    void update();
    void processOscMessage(string pattern, ofxOscMessage& m);

    FrameBuffer* getNewVideoBuffer(int _bufferSize = 1);
    
    //int getTotalFramesBuffered();
    //int getTotalBytesBuffered();
    
protected:
    AssetManager*   assetManager;

private:
    vector<ofxVideoBuffer> buffers;
    
};