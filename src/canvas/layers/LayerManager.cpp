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

#include "LayerManager.h"

//--------------------------------------------------------------
LayerManager::LayerManager() : ofxOscRouterNode("layers") {
    addOscNodeAlias("lay");
    addOscNodeAlias("l");
 
    addOscMethod("new");
    addOscMethod("delete");
}

//--------------------------------------------------------------
LayerManager::~LayerManager() {
    for (it = layers.begin(); it != layers.end(); it++) {
        delete *it;
        layers.erase(it);
    }
}


//--------------------------------------------------------------
//void LayerManager::setEffectsManager(EffectsManager* _effectsManager) {
//    effectsManager = _effectsManager;
//}

//--------------------------------------------------------------
void LayerManager::processOscCommand(const string& command, const ofxOscMessage& m) {
    
    cout << "LayerManager::const string& pattern, const ofxOscMessage& m(const string& pattern, ofxOscMessage& m)" << endl;
    
    
    if(isMatch(command,"new")) {
        // /livedraw/canvas/layer/new      LAYER_NAME [X_POSITION Y_POSITION [Z_POSITION]]
        if(validateOscSignature("s[fi][fi][fi]?", m)) {

            ofPoint p(0.0f,0.0f,0.0f);

            string layerName = m.getArgAsString(0);
            
            if(m.getNumArgs() > 1) {
                p.x = m.getArgAsFloat(1);
                p.y = m.getArgAsFloat(2);
                if(m.getNumArgs() > 3) {
                    p.z = m.getArgAsFloat(3);
                }
            }

            // make a new layer
            addLayer(layerName, p);
            
        }
    } else if(isMatch(command, "delete")) {
        // /livedraw/canvas/layer/new      LAYER_NAME
        if(validateOscSignature("s", m)) {
            string layerName = m.getArgAsString(0);
            // delete a layer
            queueUnregisterLayer(layerName);
        }
    }
    
}


//--------------------------------------------------------------
Layer* LayerManager::addLayer(const string& layerName, ofPoint point, Layer* parentLayer) {

    cout << "making new layer |"<< layerName << "| " << point << " parent=" << (parentLayer == NULL ? "X" : parentLayer->getName()) << endl;
    
    // rename if needed
    string validLayerName = validateAlias(layerName);

    Layer* cl = new Layer(this,validLayerName,point,parentLayer); // MAKE SURE THESE ARE DELETED
    
//
    cl->setPosition(point);
//   cl->setAssetManager(assetManager);
//    cl->setEffectsManager(effectsManager);

    //LayerTransform* xform = cl->getTransform();
    //ofPoint p = xform->getPosition();

    layers.insert(cl);

    addOscChild(cl);

    return cl;
}


//--------------------------------------------------------------
bool LayerManager::queueRegisterLayer(Layer* asset) {
    
}

//--------------------------------------------------------------
bool LayerManager::queueUnregisterLayer(const string& alias) {
    Layer* toDelete = getLayer(alias);
    if(toDelete != NULL) {
        return queueUnregisterLayer(toDelete);
    } else {
        return false;
    }
}

//--------------------------------------------------------------
bool LayerManager::queueUnregisterLayer(Layer* asset) {
    
}


////--------------------------------------------------------------
//bool LayerManager::addLayerAsRoot(Layer* layer) {
//    renderTree.push_back(layer);
//}


//--------------------------------------------------------------
bool LayerManager::hasAlias(const string& alias) {
    return aliases.find(alias) != aliases.end();
}

//--------------------------------------------------------------
Layer* LayerManager::getLayer(const string& alias) {
    map<string,Layer*>::iterator iter = aliases.find(alias);
    
    if(iter!=aliases.end()) {
        return iter->second;
    } else {
        return NULL;
    }
}


//--------------------------------------------------------------
//bool LayerManager::deleteLayer(const string& alias) {
//    bool success = false;
//        
//    for ( it=layers.begin(); it != layers.end(); it++ ) {
//        if(isMatch(alias,(*it)->getName())) {
//            break;
//        }
//    }
//
//    // delete first, then erase from the vector
//    if(it != layers.end()) {
//        removeOscChild(*it); // remove it from the OSC tree!
//        delete *it;       // free the memory
//        *it = NULL;
//        layers.erase(it); // erase it from the vector
//        success = true;
//    } else {
//        success = false; // something went wrong
//    }
//    
//    return success;
//
//}


//--------------------------------------------------------------
void LayerManager::update() {
    // process all de/registration queues
    processQueues();
    
    // update assets
    updateLayers();

}

//--------------------------------------------------------------
void LayerManager::draw() {
    // we are going to draw onto the caller's drawing context (namely, canvas renderer)
    
    // TODO: perhaps best to manage groups within layers manager????
    // /livedraw/canvas/layer/group/(GROUP_NAME)/add/layer
    // everything automatically shows up in a group manager
    
    // reset it to white
//    ofSetColor(255,255,255,80);
    
    for ( it=layers.begin(); it != layers.end(); it++ ) {
		
		Layer* layer = *it;
		
		LayerTransform* xform = layer->getTransform();
		ofPoint a = xform->getAnchorPoint();
		ofPoint p = xform->getPosition();
		ofPoint r = xform->getRotation();
		ofPoint s = xform->getScale();
        int opacity = xform->getOpacity();

		
        int w = xform->getWidth();
        int h = xform->getHeight();
        
        //cout << layer->getName() << ": " << p.x << "/" << p.y << "/" << p.z << endl;
        
		glPushMatrix();
		
        if (opacity < 255) ofEnableAlphaBlending();
        
        ofTranslate(p.x, p.y, p.z);

        ofRotateX(r.x);
		ofRotateY(r.y);
		ofRotateZ(r.z);
        
        ofScale(s.x, s.y, s.z);
        
		// do effects
		
		// get image reference and draw it
		//layer->getSource()->getImage()->draw(0,0);
		
        //        layer->draw();
        
        //layer->getFbo()->begin();

        ofSetColor(255,255,255,opacity);

        
 //       ofFill();
        
        if(layer->getSourcePlayer()->isLoaded()) {
            layer->getSourcePlayer()->draw(-a.x, -a.y);
        } else {
            ofPushStyle();
            ofSetColor(127);
            ofNoFill();
            ofRect(-a.x, -a.y,w,h);
            ofLine(-a.x, -a.y, -a.x + w, -a.y + h );
            ofLine(-a.x, -a.y + h, -a.x + w, -a.y );
            ofPopStyle();
        }
            
        if(layer->getMaskPlayer()->isLoaded()) {
            layer->getMaskPlayer()->draw(-a.x, -a.y);
        } else {
            ofPushStyle();
            ofSetColor(127);
            ofNoFill();
            ofRect(-a.x, -a.y,w,h);
            ofLine(-a.x, -a.y, -a.x + w, -a.y + h );
            ofLine(-a.x, -a.y + h, -a.x + w, -a.y );
            ofPopStyle();
        }


        
        //        ofRect(-a.x, -a.y,0,w,h);
//        ofNoFill();
        //layer->getFbo()->end();
        
        //layer->getFbo()->draw(0,0);

        if (opacity < 255) ofDisableAlphaBlending();

        
		glPopMatrix();
         
        
        
	}
    
    ofSetColor(255);
    
}

//--------------------------------------------------------------
int LayerManager::getLayerIndex(const string& alias) {
    return getLayerIndex(getLayer(alias));
}

//--------------------------------------------------------------
int  LayerManager::getLayerIndex(Layer* layer) {
    if(layer == NULL) {
        return -1;
    } else {
        // TODO : work on thi
        return 0;
    }
}

//--------------------------------------------------------------
bool LayerManager::bringLayerForward(Layer* layer) {
    cout << "sending layer forward " << layer->getName() << endl;
}

//--------------------------------------------------------------
bool LayerManager::sendLayerBackward(Layer* layer) {
    cout << "sending layer backward " << layer->getName() << endl;
}

//--------------------------------------------------------------
bool LayerManager::sendLayerToBack(Layer* layer) {
    cout << "sending layer to back " << layer->getName() << endl;
}

//--------------------------------------------------------------
bool LayerManager::bringLayerToFront(Layer* layer) {
    cout << "sending layer to front " << layer->getName() << endl;
}

//--------------------------------------------------------------
bool LayerManager::sendLayerTo(Layer* layer, int targetLayerIndex) {
    cout << "sending layer " << layer->getName() << " to " << targetLayerIndex << endl;
}

//--------------------------------------------------------------
void LayerManager::setLayerSolo(Layer* layer, bool solo) {
    cout << "solo layer " << layer->getName() << " solo = " << solo << endl;
}
//--------------------------------------------------------------
void LayerManager::setLayerLock(Layer* layer, bool lock) {
    cout << "lock layer " << layer->getName() << " lock =" << lock <<  endl;
}

//--------------------------------------------------------------
bool LayerManager::registerLayer(Layer* layer) {
    if(!hasAlias(layer->getName())) {
        aliases[layer->getName()] = layer;
    } else {
        ofLog(OF_LOG_ERROR, "LayerManager::registerLayer - alias already exists");
        return false;
    }
    
    if(!addOscChild(layer)) {
        ofLog(OF_LOG_ERROR, "LayerManager::registerLayer - failed to add as an osc child node");
        return false;
    }
    
    // turn it on
    layer->setNodeActive(true);

    // add it to the collection
    layers.insert(layer);
    
    return true;
    
}

//--------------------------------------------------------------
bool LayerManager::unregisterLayer(Layer* layer) {
    
    cout << "unregistering layer --- " << layer->getName() << endl;
    
    // is there a there there?
    if(layer == NULL) {
        ofLogWarning() << "LayerManager::unregisterLayer - asset is NULL";
        return false;
    }
    
    // tell the object to dispose of itself (free connections, kill other things, etc)
    if(!layer->dispose()) {
        ofLogWarning() << "LayerManager::unregisterLayer - unable to dispose " << layer->getName();
        return false;
    }
    
    // get rid of the alias
    if(hasAlias(layer->getName())) { // double check
        aliases.erase(layer->getName());
    }
    
    
    if(hasOscChild(layer) && !removeOscChild(layer)) {
        ofLogError() << "LayerManager::unregisterLayer - failed to remove as an osc child node";
        return false;
    }
    
    // remove the asset pointer from the assets set
    layers.erase(layer);
    
    // free the memory
    delete layer; // free the memory
    layer = NULL;
    // success
    return true;
}

//--------------------------------------------------------------
void LayerManager::processQueues() {
    // clear register queues
    if(!registerQueue.empty()) {
        for(it = registerQueue.begin();
            it != registerQueue.end();
            it++) {
            registerLayer(*it);
        }
        registerQueue.clear(); // done!
    }
    
    // clear unregister queue
    if(!unregisterQueue.empty()) {
        for(it = unregisterQueue.begin();
            it != unregisterQueue.end();
            it++) {
            unregisterLayer(*it);
        }
        unregisterQueue.clear(); // done!
    }
}

//--------------------------------------------------------------
void LayerManager::updateLayers() {
    for ( it=layers.begin(); it != layers.end(); it++ ) {
        (*it)->update();
    }
}


//--------------------------------------------------------------
string LayerManager::validateAlias(const string& name) {
    // get the original asset name
    string assetName = name;
    
    // get the normalized name for OSC purposes
    string assetId = normalizeOscNodeName(assetName);
    
    // if the alias exists, add a random suffix
    if(hasAlias(assetId)) assetId +=  ("_" + ofToString((int)ofRandom(10000)));
    
    // toss a warning if the asset id differs from the asset's original name
    if(!isMatch(assetName, assetId)) {
        ofLogWarning() << "LayerManager::generateAssetId() - " << assetName << " produced variant : " << assetId + ".";
    }
    
    // return the generated asset id
    return assetId;
}


//--------------------------------------------------------------
void LayerManager::dump() {

    cout << "layers:" << endl;
    for(it = layers.begin(); it != layers.end(); it++) {
        cout << (*it)->getName() << endl;
    }
    
    cout << "renderTree:" << endl;
    vector<Layer*>::iterator it2;

    for(it2 = renderTree.begin(); it2 != renderTree.end(); it2++) {
        cout << (*it)->getName() << endl;
    }

}
