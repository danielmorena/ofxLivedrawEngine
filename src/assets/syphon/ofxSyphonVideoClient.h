/*==============================================================================
 
 Copyright (c) 2010, 2011, 2012 Christopher Baker <http://christopherbaker.net>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 ==============================================================================*/

#pragma once

#include "ofxSyphonClient.h"

class ofxSyphonVideoClient : public ofxSyphonClient, 
                             public ofBaseVideo

{
public:
    ofxSyphonVideoClient() {}
    virtual ~ofxSyphonVideoClient() {}
    
    void update() {
    }
    
    unsigned char* getPixels() {
        if(isFrameNew()) {
            fillPixels();
        }
        return pixels.getPixels();
    }
    
	virtual ofPixelsRef getPixelsRef() {
        if(isFrameNew()) {
            fillPixels();
        }
        return pixels;
    }

    virtual bool isFrameNew() {
        // TODO: no way to check this just yet
        return true;
    }
    
    void open() {
        // TODO: not implemented
    }
    
    void close() {
        // TODO: not implemented
    }

    bool isConnected() {
        // TODO: not implemented
        return true;
    }
    
protected:
    void fillPixels() {
        // TODO: this should only be called as needed
        // there is likely no reason to do a readback here.  
        // probably can just bind to the texture directly.
        // but we need to bring it into the shared pointer
        
        // TODO: this all goes back to the issue with creating
        // a more efficient and all around useful ofImage
        mTex.readToPixels(pixels);
    }
    
    ofPixels pixels;

};
