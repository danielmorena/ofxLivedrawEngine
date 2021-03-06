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
    
    engine = NULL;
    
    addOscNodeAlias("lay");
    addOscNodeAlias("l");
 
    addOscMethod("new");
    addOscMethod("delete");
    addOscMethod("dump");
}

//--------------------------------------------------------------
LayerManager::~LayerManager() {
    for (it = layers.begin(); it != layers.end(); it++) {
        delete (*it).second; // free the layer
    }
    layers.clear(); // clear it;
}

//--------------------------------------------------------------
void LayerManager::processOscCommand(const string& command, const ofxOscMessage& m) {
    
    if(isMatch(command,"new")) {
        // /livedraw/canvas/layer/new      LAYER_NAME [X_POSITION Y_POSITION [Z_POSITION]]
        // /livedraw/canvas/layer/new      LAYER_NAME [X_POSITION Y_POSITION [Z_POSITION]] [WIDTH HEIGHT]
        if(validateOscSignature("s[fi][fi][fi]?[fi]?[fi]?", m)) {
            string layerName = getArgAsStringUnchecked(m,0);
            vector<float> args = getArgsAsFloatArray(m,1);
            size_t nArgs = args.size();
            
            if (nArgs == 5) {
                ofPoint p(args[0],args[1],args[2]);
                float width  = args[3];
                float height = args[4];
                addLayer(layerName, p, width, height);
            } else if(nArgs == 4) {
                ofPoint p(args[0],args[1]);
                float width  = args[2];
                float height = args[3];
                addLayer(layerName, p, width, height);
            } else if(nArgs == 3) {
                ofPoint p(args[0],args[1],args[2]);
                addLayer(layerName, p);
            } else if(nArgs == 2) {
                ofPoint p(args[0],args[1]);
                addLayer(layerName, p);
            } else {
                ofLogError("LayerManager") << "Invalid number of arguments.";
            }
        }
    } else if(isMatch(command, "delete")) {
        // /livedraw/canvas/layer/new      LAYER_NAME
        if(validateOscSignature("s", m)) {
            string layerName = getArgAsStringUnchecked(m,0);
            // delete a layer
            queueDestroyLayer(layerName);
        }
    } else if(isMatch(command, "dump")) {
        dump();
    }
    
}

//--------------------------------------------------------------
Layer* LayerManager::addLayer(const string& layerName, const ofPoint& point, Layer* parentLayer) {
    string validLayerName = validateAlias(layerName);
    Layer* layer = new Layer(this,validLayerName,point,parentLayer); // MAKE SURE THESE ARE DELETED
    registerLayer(layer);
    return layer;
}


//--------------------------------------------------------------
Layer* LayerManager::addLayer(const string& layerName, const ofPoint& point, int width, int height, Layer* parentLayer) {
    string validLayerName = validateAlias(layerName);
    Layer* layer = new Layer(this,validLayerName,point,width,height,parentLayer); // MAKE SURE THESE ARE DELETED
    registerLayer(layer);
    return layer;
}

//--------------------------------------------------------------
bool LayerManager::queueAddLayer(Layer* layer) {
    layer->setNodeActive(false);
    return registerQueue.insert(layer).second;
}

//--------------------------------------------------------------
bool LayerManager::queueDestroyLayer(const string& alias) {
    Layer* toDelete = getLayer(alias);
    if(toDelete != NULL) {
        return queueDestroyLayer(toDelete);
    } else {
        return false;
    }
}

//--------------------------------------------------------------
bool LayerManager::queueDestroyLayer(Layer* layer) {
    if(layer != NULL) {
        layer->setNodeActive(false);
        return unregisterQueue.insert(layer).second;
    } else {
        ofLogError("LayerManager") << "queueDestroyLayer: Layer was NULL.";
        return false;
    }
}

////--------------------------------------------------------------
//bool LayerManager::addLayerAsRoot(Layer* layer) {
//    renderTree.push_back(layer);
//}


//--------------------------------------------------------------
bool LayerManager::hasLayer(const string& alias) const {
    return layers.find(alias) != layers.end();
}

//--------------------------------------------------------------
Layer* LayerManager::getLayer(const string& alias) {
    it = layers.find(alias);
    
    if(it != layers.end()) {
        return it->second;
    } else {
        return NULL;
    }
}

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
        
    for (renderTreeIter  = renderTree.begin();
         renderTreeIter != renderTree.end();
         renderTreeIter++ ) {
		(*renderTreeIter)->draw();
	}
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
    ofLogNotice("LayerManager") << "bringLayerForward() " << layer->getName() << endl;
    // TODO
    return false;
}

//--------------------------------------------------------------
bool LayerManager::sendLayerBackward(Layer* layer) {
    ofLogNotice("LayerManager") << "sendLayerBackward() " << layer->getName() << endl;
    // TODO
    return false;
}

//--------------------------------------------------------------
bool LayerManager::sendLayerToBack(Layer* layer) {
    ofLogNotice("LayerManager") << "sendLayerToBack() " << layer->getName() << endl;
    // TODO
    return false;
}

//--------------------------------------------------------------
bool LayerManager::bringLayerToFront(Layer* layer) {
    ofLogNotice("LayerManager") << "bringLayerToFront() " << layer->getName() << endl;
    // TODO
    return false;
}

//--------------------------------------------------------------
bool LayerManager::sendLayerTo(Layer* layer, int targetLayerIndex) {
    ofLogNotice("LayerManager") << "sendLayerTo() " << layer->getName() << " to " << targetLayerIndex << endl;
    // TODO
    return false;
}

//--------------------------------------------------------------
void LayerManager::setLayerSolo(Layer* layer, bool solo) {
    ofLogNotice("LayerManager") << "setLayerSolo() " << layer->getName() << " solo = " << solo << endl;
    // TODO
}

//--------------------------------------------------------------
void LayerManager::setLayerLock(Layer* layer, bool lock) {
    ofLogNotice("LayerManager") << "setLayerLock() " << layer->getName() << " lock =" << lock <<  endl;
    // TODO
}

//--------------------------------------------------------------
bool LayerManager::registerLayer(Layer* layer) {
   
    if(layer == NULL) {
        ofLogError("LayerManager") << "registerLayer - layer is NULL";
        return false;
    }
    
    string layerName = layer->getName();
    
    if(!hasLayer(layerName)) {
        layers[layerName] = layer; // add layer with given name
    
        // add it as an osc child
        if(!addOscChild(layer)) {
            ofLogError("LayerManager") << "registerLayer - failed to add as an osc child node";
            queueDestroyLayer(layer);
            return false;
        }

        // turn it on
        layer->setNodeActive(true);

        // if it is a null parent, then it belongs in the render tree
        if(layer->getLayerParent() == NULL) {
            renderTree.push_back(layer);
        }

        return true;
    } else {
        ofLogError("LayerManager") << "registerLayer - alias already exists";
        queueDestroyLayer(layer);
        return false;
    }
    
}

//--------------------------------------------------------------
bool LayerManager::unregisterLayer(Layer* layer) {
    // TODO: this needs a bug check
    
    // is there a there there?
    if(layer == NULL) {
        ofLogWarning("LayerManager") << "unregisterLayer - asset is NULL";
        return false;
    }
    
    // tell the object to dispose of itself (free connections, kill other things, etc)
    if(!layer->dispose()) {
        ofLogWarning("LayerManager") << "unregisterLayer - unable to dispose " << layer->getName();
        return false;
    }
    
    if(hasOscChild(layer) && !removeOscChild(layer)) {
        ofLogError("LayerManager") << "unregisterLayer - failed to remove as an osc child node";
        return false;
    }
    
    // get rid of the alias
    if(hasLayer(layer->getName())) { // double check
        layers.erase(layer->getName());
    }

    // erase it from the render tree
    bool foundIt = false;
    int i = ofFind(renderTree, layer);
    if(i < renderTree.size()) {
        foundIt = true;
        renderTree.erase(renderTree.begin() + i);
    }
    
    if(layer->getLayerParent() == NULL && !foundIt) {
        ofLogError("LayerManager") << "A layer had a null parent, but wasn't found in the render tree root.";
    }
    
    // free the memory
    delete layer; // free the memory
    layer = NULL;
    // success
    return true;
}

//--------------------------------------------------------------
void LayerManager::processQueues() {
    // clear register queues
    set<Layer*/*,layerSetOrder*/>::iterator it;

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
        (*it).second->update();
    }
}

//--------------------------------------------------------------
string LayerManager::validateAlias(const string& name) {
    // get the normalized name for OSC purposes
    string normName = normalizeOscNodeName(name);

    it = layers.find(normName);
    
    // if it needs a suffix, it will be added in here
    if(it != layers.end()) {
        int maxSuffix = -1;
        
        // pick up where find left off
        for (; it != layers.end(); it++ ) {
            string thisName = (*it).second->getName();
            if(isMatch(normName,thisName.substr(0,normName.length()))) {
                // found prefix match
                if(thisName.length() > normName.length()) {
                    // this match has a suffix
                    if(thisName[normName.length()] == '_') {
                        string number = thisName.substr(normName.length() + 1);
                        if(number.length() > 0) {
                            maxSuffix = MAX(maxSuffix,ofToInt(number));
                        }
                    }
                }
            } else {
                // there shouldn't be any more after this given our ordering
                break;
            }
        }

        string newName = normName + '_' + ofToString(maxSuffix + 1);
        ofLogWarning("LayerManager") << "generateAssetId(): " << name << " produced variant : " << newName << ".";
        return newName;
    } else {
        // the normName was unique.
        return normName;
    }
    
}


//--------------------------------------------------------------
void LayerManager::dump() {

    ofLogNotice("LayerManager") << "-------------------";
    ofLogNotice("LayerManager") << "Layer Manager Dump:";
    ofLogNotice("LayerManager") << "layers:";
    for(it = layers.begin(); it != layers.end(); it++) {
        ofLogNotice("LayerManager") << setw(4) << (*it).second->toString();
    }
    
    ofLogNotice("LayerManager") << "renderTree:";
    for(renderTreeIter = renderTree.begin();
        renderTreeIter != renderTree.end();
        renderTreeIter++) {
        ofLogNotice("LayerManager") << setw(4) << (*renderTreeIter)->toString();
    }

}

