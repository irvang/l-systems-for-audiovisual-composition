//  Make a function to define time in ms instead of samples


#include "ofApp.h"

//  turtleClass
int numberOfTurtles = 1;
float stepSize = 5;// multiplied times the vector variables to adjust size of 'f'.
int polyphony = 64;
float theta = 60;// theta// angle of turn

//-------------------------
//  L-System
string currentLString = "f--f--f";
string resetCurrentLString = currentLString;    // for resetting
string newGen;
short generationNum = 0;
int stringCount = 0;
int triggerRest = 1; // trigger a rest every so many stringCounts. Never less than 1
float restDuration = 100; //  rest duration in ms. Counts so many samples of rest.
int restCounter = 0;
float storeRestDur;
bool rest = FALSE;  //  true when rest is on

int fCount = 0;

int countSameGrain = 1;// repeat the same grain this many times before changing it

bool oscGate = false;          // false to turn off Oscwith turtle. open sound control

int updateCounter = 0;
//-------------------------
//  Event timing
bool metronome = false;
bool drawGate = false;
float drawSpeedMs = 30;
bool bInfoText = true;

ofVec3f point1, point2, point3;
float rotateX, rotateY, rotateZ;

int mode;
//--------------------------------------------------------------
void ofApp::setup(){
    
    //  audio loop setup
    int bufferSize		= 512;
    sampleRate 			= 48000;
    soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
    sampleCount = 0;
    
    //  background and blend enable setup
    ofBackground(40);
    //    ofBackground(255, 255, 255, 255);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    camDist  = 300;            // set the camera distance
    cam.setDistance(camDist);
    
    //---------------------------------------
    //  turtles setup
    lookAt.set(1.0, 0.0, 0.0); heading.set(0.0, 1.0, 0.0);color.set( 50, 20, 100); startPos.set(0, 0, 0);
    cV.set(0.1, 0.25, 0.5); //  color vector
    color2.set(0.5, 0.5,0.5);
    alpha = 1;
    
    for(int i = 0; i < numberOfTurtles ; i++){
        //        stepSize = (ofRandom( 5, 10));
        
        turtles.push_back(myTurtleClass(theta, stepSize, startPos, lookAt, heading, color));
        turtleWKeys.push_back(myTurtleClass(theta, stepSize, startPos, lookAt, heading, color));
        
        startPos.set(ofRandom(-70, 70), ofRandom(-50, 50), ofRandom(-50, 50));
        //           turtles[i].setColor(1, 0, 0);
        turtles[i].setColor(ofRandom(0.25, 1.0), ofRandom(0.25, 1.0), ofRandom(0.25, 1.0));
        turtleWKeys[i].setColor(ofRandom(0.25, 1.0), ofRandom(0.25, 1.0), ofRandom(0.25, 1.0));// works with the keys
        //  myTurtleClass(float angleOfTurn, float sizeOfSteps, ofVec3f startingCoordinates, ofVec3f setLookAt, ofVec3f setHeading);
    }
    
    //---------------------------------------
    //  set synth params setup
    //  duration, freqencies, window typp, wave type
    
    dur = 100;  durS = dur;
    freqInLoop = 250;   freqInLoopS = freqInLoop;
    amp = 0.05;     ampS = amp;
    waveType = "sawnWave";  waveTypeS = waveType;
    windowType = "gaussian";    windowTypeS = windowType;
    
    arraysCounter = 0;
    rhythm = {50, 50, 100, 100, 100, 500, 100, 50, 30, 20};
    //    rhythm = {250};
    //    durations = {1000};
    durations = {100, 30, 25, 90, 100, 48, 32, 70, 50};
    frequencies = {145, 240, 800, 300, 500, 600, 900, 2000};
    
    //    frequencies = {ofRandom(145, 240), ofRandom(2500, 3000)};
    
    amplitudes = {0.5, 0.6, 0.3, 0.8, 0.35, 0.5};
    
    rhythmSize = rhythm.size(); durationsSize = durations.size();
    freqsSize = frequencies.size(); ampSize = amplitudes.size();
    //  size stored here to avoid calculating every time in the loops
    
    //  vector of synths  setup
    for(int i=0; i< polyphony; i++){
        
        synths.push_back(grainClass(ofRandom(400) + 400 , ofRandom(0.05), 0.5, false, 240.0));
        // frequency not doing much now
        //        (double freq, double amp, double pan, bool doSynth)
    }
    
    //---------------------------------------
    //  Gui setup
    gui.setup();
    
    //    gui.add(drawSpeedLowGui.setup("drawSpeedMsLo", 1, 1.0, 100));     // control drawing speed
    //    gui.add(drawSpeedHighGui.setup("drawSpeedMsHi", drawSpeedMs, 1.0, 200));
    gui.add(scaleDrawSpeed.setup("scaleSpeed", 1, 0.001, 1.5));
    gui.add(durGui.setup("scaleDurationArray", 1, 0.01, 2));
    gui.add(restDurGui.setup("restDuration", 250, 0.05 , 1000));
    gui.add(charForRestGui.setup("charForRest", 10, 1, 50));
    gui.add(repeatGrainGui.setup("repeatGrain", 1, 1, 50));
    
    gui.add(stepSizeGui.setup("stepsSize", stepSize, 0.0, 60));         //control size of steps
    gui.add(thetaGui.setup("theta", theta, 0.0, 360));     // change theta
    gui.add(thetaGuiRand.setup("thetaRand", 0.0, 0.0, 180));// change random around theta
    gui.add(rotateXGui.setup("rotateXSpeed", 0.0, 0.0, 1.0));
    gui.add(rotateYGui.setup("rotateYSpeed", 0.0, 0.0, 1.0));
    gui.add(rotateZGui.setup("rotateZSpeed", 0.0, 0.0, 1.0));
    
    gui.add(dupVertex.setup("dupVertex", true));
    
    //    gui.add(colorGui.setup("color", ofColor(0, 150, 255), ofColor(0, 0), ofColor(255, 255)));
    gui.add(screenSize.setup("screen size", ofToString(ofGetWidth())+ "x" + ofToString(ofGetHeight())));
    
    gui2.setup();
    
    //---------------------------------------
    //  Spheres
    
    for(int i = 0; i < 6 ; i++){
        spheres.push_back(*new ofSpherePrimitive);
        spheres[i].setRadius(150);
        spheres[i].enableColors();
        ofSetColor(200, 100, 100, 20);
    }
    
    sDFC = 500; //  sphereDistanceFromCenter
    
    point1.set(1 * sDFC, 0 * sDFC, 0 * sDFC );
    point2.set(0 * sDFC, 1 * sDFC, 0 * sDFC );
    point3.set(0 * sDFC, 0 * sDFC, 1  * sDFC);
    
    spheres[0].setPosition(point1 );
    spheres[1].setPosition(point1 * -1);
    spheres[2].setPosition(point2  );
    spheres[3].setPosition(point2 * -1);
    spheres[4].setPosition(point3 );
    spheres[5].setPosition(point3 * -1);
    
    rotateX = rotateXGui; rotateY = rotateYGui; rotateZ = rotateZGui;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    for(int i = 0; i < numberOfTurtles ; i++){
        
        turtles[i].setDupVertex(dupVertex);
        turtleWKeys[i].setDupVertex(dupVertex);
    }
    
    //  my thinking is that this counter will go from updating 60 times per second to 12 times per second, and so be more efficient. It will have though to count the modulo and evaluate the if
    if(updateCounter % 5 == 0){
        
        triggerRest = charForRestGui;
        restDuration = restDurGui;
        
        if(updateCounter >= 59){
            
            //  the modulo here to avoid calculating 60 times/s
            //  updateCounter 0 to 59. 0, 5, 10, ..., 55
            updateCounter = updateCounter % 60;
        }
    }
    updateCounter++;
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    cam.begin();
    ofRotateX(ofRadToDeg(0.5));
    ofRotateY(ofRadToDeg(-0.5));
    
//    
    ofRotateX(rotateX);
    ofRotateY(rotateY);
    ofRotateY(rotateZ);
//        ofRotateX(ofRadToDeg(rotateX));
//        ofRotateY(ofRadToDeg(rotateY));
//        ofRotateY(ofRadToDeg(rotateY));
    
    //  set draw of turtle
    for(int i=0; i< numberOfTurtles; i++){
        turtles[i].draw();
        turtleWKeys[i].draw();
    }
    
    //  draw spheres or not
    if(bHide){
        for(int i = 0; i < 6 ; i++){
            ofSetColor(200, 100, 100, 30);
            spheres[i].draw();
        }
    }
    
    cam.end();
    
    if(!bInfoText){
        gui.draw();  /*gui2.draw();*/
        
        ofSetColor(255);

        stringstream ss;
        
        ss << "Press 'next Generation' button \nseveral times"<<endl<< ""<<endl <<"(s): Run program"<<endl<<"(d): Visualize shape "<<endl<<"(c) Clear"<< endl<<"(z) Clear & redraw" << endl<<"(v): scramble rhythm dur freq amp"<< endl;
        ss <<"(C): Reset generations " <<endl;
        ss <<"(0-10 ): Drawing modes" <<endl;
        ss <<"cam: click and drag" <<endl;
        ss <<"cam: control + click and drag" <<endl;
        ss <<"cam: alt + click and drag" <<endl;
        
        ss <<"(b): Draw space ref " <<endl;
        ss <<"(0-10 ): Drawing modes" <<endl;
        ss <<"(n): Info Text/Gui"<<endl<< "" << endl;
        
        ss << "Generation "  << generationNum  << endl;
        ss << "lenght is "<< currentLString.length() << endl;
        ss << "Framerate: " << ofToString(ofGetFrameRate(),0) << "\n";
        
        ofDrawBitmapString(ss.str().c_str(), 20, 350);
    }
    rotateX += rotateXGui; rotateY += rotateYGui; rotateZ += rotateZGui;
}
//--------------------------------------------------------------
void ofApp::lSystem(bool generate){
    
    //  Generate L-system string
    
    if(generate){
        
        //  sets "newGen" to " " (empty)
        //  next needs to be here so it starts anew (empty, reset, nil) when space bar is pressed
        newGen = "";
        
        //  iterate over current LString and do the appropriate replacements
        //  once the for loop has ended the string that is in "newGen" is stored in "currentLString"
        for (int i = 0; i < currentLString.length(); i++){
            
            switch(currentLString[i])
            {
                    //  next = next + production
                    //  adds whichever case is selected to the end of the string "newGen"
                    
                    //------------------------------
                    //  Place rules below this space
                    //------------------------------
                    
                    
                case 'f':
                    newGen += "f+f--f+f/-";
                    break;
                    
                    //------------------------------
                    //Place rules above this space
                    //------------------------------
                    
                    // if no substitution is found, whatever is on "currentLString" at i is passed back to "newGen"
                default:{newGen += currentLString[i];}
                    break;
            }
        }
        generationNum++; // augment only number generations
        
        //  once the for loop has ended the string that is in "newGen" is stored in "currentLString"
        currentLString = newGen;
        
        //  print out generation number and length
        cout << "//-----------------------------" << endl;
        cout << "Generation "  << generationNum  << endl;
        cout << "lenght is "<< currentLString.length() << endl;
        if (currentLString.length() <= 1500){ cout << currentLString << endl; }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key)
    {
            //----------------------------------------
            //  space bar generates next string
        case 32:
            //  sets the function to true in order to generate the new string
            lSystem(true);
            
            break;
            //--------------------------------
            //  draw the shape
        case 'd':
            //  reads through Linden String.
            for (int i = 0; i<currentLString.length(); i++){
                
                
                //--------------
                for(int j=0; j < numberOfTurtles; j++){
                    
                    turtles[j].setTheta(ofRandom(thetaGui - thetaGuiRand, thetaGui + thetaGuiRand));
                    turtles[j].setStepSize(stepSizeGui);
                    turtles[j].setColor(drunk(color2.x, 0.05, 0.1, 1.0), drunk(color2.y, 0.05, 0.1, 1.0), drunk(color2.z, 0.05, 0.1, 1.0), drunk(alpha, 0.05, 0.8, 1.0));
                    
                    //  this function passes the drawing commands to the turtles
                    turtles[j].commandTurtle(currentLString[i]);
                }
            }
            break;
            //--------------------------------
            //  clear and redraw the shape
        case 'z':
            
            for(int j=0; j < numberOfTurtles; j++){
                //  clear
                turtles[j].clearReset(true);
                turtleWKeys[j].clearReset(true);
                
                
            }
            //    reads through Linden String.
            for (int i = 0; i<currentLString.length(); i++){
                
                
                //--------------
                //  draw
                for(int j=0; j < numberOfTurtles; j++){
                    
                    turtles[j].setColor(drunk(color2.x, 0.007, 0.1, 1.0), drunk(color2.y, 0.007, 0.1, 1.0), drunk(color2.z, 0.007, 0.1, 1.0), drunk(alpha, 0.007, 0.8, 1.0));   // colors
                    turtles[j].setStepSize(stepSizeGui);    //  stepsize
                    turtles[j].setTheta(ofRandom(thetaGui - thetaGuiRand, thetaGui + thetaGuiRand));//theta
                    
                    //  this function passes the drawing commands to the turtles
                    turtles[j].commandTurtle(currentLString[i]);    // commandTurtle
                }
            }
            break;
            //--------------------------------
            //  clear the turtle
        case 'c':
            
            for(int j=0; j < numberOfTurtles; j++){
                
                turtles[j].clearReset(true);
                turtleWKeys[j].clearReset(true);
                stringCount = 0;
            }
            break;
            //--------------------------------
            //  clear the turtle
        case 'C':
            
            currentLString = resetCurrentLString;
            generationNum = 0;
            //  print out generation number and length
            cout << "//-----------------------------" << endl;
            cout << "Generation "  << generationNum  << endl;
            cout << "lenght is "<< currentLString.length() << endl;
            if (currentLString.length() <= 1500){ cout << currentLString << endl; }
            
            break;
            //--------------------------------
            //  reset the turtle
        case 'r':
            
            for(int j=0; j < numberOfTurtles; j++){
                
                turtles[j].reset(true);
                turtleWKeys[j].reset(true);
                stringCount = 0;
            }
            break;
            //--------------------------------
        case 'm': characterCounter(true); break; // counts characters on the string. counts only commands to turtle
            //--------------------------------
        case 115: drawGate = !drawGate;      // key 's': flag to pass or not the audio info
            break;
            //--------------------------------
        case 'n': bInfoText = !bInfoText;   // hide Gui
            break;
            //--------------------------------
        case 'b': bHide = !bHide;   // hide balls
            break;
            //--------------------------------
            //--------------------------------
        case 'v':
            scrambleArrays(true);
            //  scramble the arrays containing durations, freqs, etc
            break;
            //--------------------------------
        case 'q': waveType = "sineWave";
            break;    //--------------------------------
        case 'w': waveType = "triWave";
            break;    //--------------------------------
        case 'e': waveType = "sawnWave";
            break;    //--------------------------------
            //        case 'r': waveType = "squareWave";
            break;    //--------------------------------
        case 'Q': windowType = "hanning";
            break;    //--------------------------------
        case 'W': windowType = "blackH";
            break;    //--------------------------------
        case 'E': windowType = "gaussian";
            break;    //--------------------------------
        case 'R': windowType = "triWin";
            break;
            //--------------------------------
        default:    // default switch(key)
        { }
            break;
    }       //  end switch (key)
    //--------------------------------------------------------
    //  turtles
    for(int j=0; j< numberOfTurtles; j++){
        
        //  this function sets the primitives in the turtles
        turtles[j].setPrimitive(key);
    }
    //--------------------------------------------------------
    //  turtleWKeys
    //  these turtles only work with the keys
    for(int j=0; j< numberOfTurtles; j++){
        
        
        turtleWKeys[j].setStepSize(stepSizeGui);
        turtleWKeys[j].commandTurtle(key);
        turtleWKeys[j].setTheta(ofRandom(thetaGui - thetaGuiRand, thetaGui + thetaGuiRand));
        turtleWKeys[j].setPrimitive(key);
        
    }
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels){
    
    
    for (int i = 0; i < bufferSize; i++){
        
        leftChan = 0;
        rightChan = 0;
        
        if(rest == true) {
            //             restCounter++;
            
            // if I change it, make 0 on next count of buffersize
            if(restCounter >= msToSamp(restDuration) -1){
                rest = false;
            }
            restCounter++;
        }
        
        
        //  draw speed in ms
        sampleCount = sampleCount % (msToSamp(drawSpeedMs));
        
        if(rest == false)
        {
            if(sampleCount == (msToSamp(drawSpeedMs) - 1) && drawGate == true){
                
                //  drawSpeedLoop is the function which sets the synthesizer parser
                drawSpeedLoop(true);
                
                //            if(fsOrNotFs == true){
                //                if(currentLString[stringCount] == 'f' || currentLString[stringCount] == 'g' || currentLString[stringCount] == 'h')
                {
                    // only draw if 'f' , 'h' , or 'g' is received
                    
                    for(int j = 0; j < polyphony; j++){
                        
                        // the first synth that returns false, make true, and break loop
                        if(synths[j].get_mDoSynth() == false){
                            
                            // turns synth on
                            synths[j].set_mDoSynth(true);
                            
                            
                            // set synth values
                            //  not sure if I can set these outside of the loop because of the way they are set with the for loop
                            if((arraysCounter % countSameGrain) < (countSameGrain - 1) ){
                                
                                synths[j].set_mPan(ofRandom(1.0));
                                synths[j].set_mFreq(freqInLoopS);
                                synths[j].set_mAmp(ampS);
                                //                        drunk( dur, 5, 1, 25);
                                synths[j].set_dur(msToSamp(durS));
                                //                        synths[j].set_dur(msToSamp(durGui));
                                synths[j].setWaveType(waveTypeS);
                                synths[j].setWinType(windowTypeS);
                                
                                //  the idea with this is that it would only change sound every so many synths
                                //  countSameGrain is the number of times to repeat each grain setting
                            } else if ((arraysCounter % countSameGrain) == countSameGrain - 1){
                                
                                
                                synths[j].set_mPan(ofRandom(1.0));
                                
                                //                                freqInLoop = frequencies.at(arraysCounter % freqsSize);
                                synths[j].set_mFreq(freqInLoop);
                                amp = amplitudes.at(arraysCounter % ampSize);
                                synths[j].set_mAmp(amp);
                                //                        drunk( dur, 5, 1, 25);
                                dur = durations.at(arraysCounter % durationsSize) * durGui;
                                synths[j].set_dur(msToSamp(dur));
                                //                        synths[j].set_dur(msToSamp(durGui));
                                synths[j].setWaveType(waveType);
                                synths[j].setWinType(windowType);
                                
                                drawSpeedMs = rhythm.at(arraysCounter % rhythmSize) * scaleDrawSpeed;
                                
                                // same type of grain will repeat this many times
                                countSameGrain = round(ofRandom(1, repeatGrainGui));
                                
                                //  these values are stored in case I want to repeat a type of grain x many times
                                freqInLoopS = freqInLoop; ampS = amp; durS = dur; waveTypeS = waveType; windowTypeS = windowType;
                            }
                            break;
                        }       //  end if mdosynth == false
                    }           //  end for synths[j]
                }               //  end if lindenchar == f, g, h
                
            }                   //  end if sampleCount == drawspeedms
        }                       //  end if rest == false
        sampleCount++;
        
        for(int j =0; j< polyphony; j++){
            
            //  if synth [j] is true
            if(synths[j].get_mDoSynth() == true){
                
                //  if window position its at its end
                //  perhaps this statement should happen after so it is evaluated only once
                if(synths[j].get_mWindowPos() == synths[j].get_dur()){
                    
                    
                    synths[j].set_mDoSynth(false);
                    synths[j].set_mWindowPos(0);
                    auto it = synths.begin() + j;
                    rotate(it, it + 1, synths.end());
                    j = j-1;
                    
                } else {
                    //  if window position is not yet at its end
                    
                    //-----------------------------
                    //  First state type of wave (getWaveType()) with freq X (get_mFreq())
                    //  getWaveType from class, so that each specific synth at(j) is assigned one type of waveform
                    
                    if(synths[j].getWaveType() == "triWave"){currentSample = synths[j].oscInSynth.triangle(synths[j].get_mFreq());}
                    
                    else if(synths[j].getWaveType() == "squareWave"){currentSample = synths[j].oscInSynth.square(synths[j].get_mFreq());}
                    
                    else if(synths[j].getWaveType() == "sineWave"){currentSample = synths[j].oscInSynth.sinewave(synths[j].get_mFreq());}
                    
                    else if(synths[j].getWaveType() == "sawnWave"){currentSample = synths[j].oscInSynth.sawn(synths[j].get_mFreq());}
                    
                    //                    triWave, rectWave, squareWave, sinWave, rectWin, hannWin, blackHWin, gaussWin, triWind;
                    
                    //----------------------------------
                    //  second mult by env
                    //  getWinType from class, so that each specific synth at(j) is assigned one type of envelope
                    
                    if(synths[j].getWinType() == "hanning"){currentSample = currentSample * han(synths[j].get_dur(), synths[j].get_mWindowPos());}
                    
                    else if(synths[j].getWinType() == "blackH"){currentSample = currentSample * blackH(synths[j].get_dur(), synths[j].get_mWindowPos());}
                    
                    else if(synths[j].getWinType() == "gaussian"){currentSample = currentSample * gaus(synths[j].get_dur(), synths[j].get_mWindowPos());}
                    
                    else if(synths[j].getWinType() == "triWin"){currentSample = currentSample * triWin(synths[j].get_dur(), synths[j].get_mWindowPos());}
                    
                    
                    
                    //  set grain's amplitude
                    currentSample = currentSample * synths[j].get_mAmp();
                    
                    //  give it a pan pos;
                    synths[j].mixerInSynth.stereo (currentSample, outputs, synths[j].get_mPan());
                    
                    //  output the sample
                    leftChan += outputs[0];
                    rightChan += outputs[1];
                    
                    //  set new sample to previous plus one. increment by 1
                    synths[j].set_mWindowPos (synths[j].get_mWindowPos() +1);
                    
                }
            }
            else
            {
                break;
            }
        }       //  end for loop
        
        output[i * nChannels] = leftChan;
        output[i * nChannels + 1] = rightChan;
    }           //  end for loop
}

//-------------------------------------------------------
//  Trigger loop. Here the events are triggered with a boolean from within the audio loop.
//  These are triggered every so many amples, passed in values of ms from the array

void ofApp::drawSpeedLoop(bool metroBang){
    // there seems to be no need to turn it off
    
    if(metroBang){
        //  drawgate switches to true with s (for start)
        //  stringCount is incremented at end of this statement
        
        //  read the character from the string, then first tell the turtle what to do, afterwards tell the synthesizer what to do
        
        // turtleClass ------------------------------------------------
        for(int j =0; j< numberOfTurtles; j++){
            
            turtles[j].setTheta( ofRandom(thetaGui - thetaGuiRand, thetaGui + thetaGuiRand));
            
                        turtles[j].setColor(color.x, color.y, color.z, drunk(alpha, 0.05, 1, 1.0));
//            turtles[j].setColor(drunk(color2.x, 0.05, 0.1, 1.0), drunk(color2.y, 0.05, 0.1, 1.0), drunk(color2.z, 0.05, 0.1, 1.0), drunk(alpha, 0.05, 0.8, 1.0));
            //            turtles[j].setColor(0,0,0,1);
            
            turtles[j].commandTurtle(currentLString[stringCount]);
            
        }
        //-------------
        
        //  ------------------------------------------------
        //  Synth parser
        //  These commands do not affect the turtle, only the synth. No commandTurtle function is present
        switch (currentLString[stringCount])
        {
                //-------------------------------------
            case 'f':   // 102:    // f, draw forward
            case 'h':
            case 'g':
                
                waveType = "triWave";
                windowType = "blackH";
                drawSpeedMs = rhythm.at(arraysCounter % rhythmSize) * scaleDrawSpeed;
                arraysCounter++;
                
                for(int j =0; j< numberOfTurtles; j++){
                    turtles[j].setStepSize(drunk(stepSize, 0, 5, 5) * stepSizeGui);// change step sizes dynamically
                    //                    cout<< theta << ", ";
                }
                freqInLoop = frequencies.at( 10 % freqsSize);
                
                break;
                //-------------------------------------
            case '+':
                
                waveType = "sawnWave";
                windowType = "blackH";
                
                freqInLoop = ofRandom((frequencies.at( 0 % freqsSize) - 20), (frequencies.at( 0 % freqsSize) + 20));
                color.set(1, 0, 0);
                alpha = 0.5;
                
                break;
                //-------------------------------------
            case '-':
                
                windowType = "hanning";
                waveType = "square";
                freqInLoop = ofRandom((frequencies.at( 1 % freqsSize) - 20), (frequencies.at( 1 % freqsSize) + 20));
                
                color.set(0.0, 1.0, 0.0);
                alpha = 0.5;
                
                //-------------------------------------
            case '|':   //124:// |  // rotates 180¼ around Ru (- +)
                
                waveType = "triWave";
                windowType = "triWin";
                
                freqInLoop = frequencies.at( 2 % freqsSize);
                
                break;
                //-------------------------------------
            case 'u':   //117:// u, up
                
                waveType = "triWave";
                windowType = "triWin";
                freqInLoop = ofRandom((frequencies.at( 3 % freqsSize) - 20), (frequencies.at( 3 % freqsSize) + 20));
                
                
                color.set(0, 0, 1);
                alpha = 0.5;
                
                break;
                //-------------------------------------
            case 'd':   //100:// d, up
                
                waveType = "sawnWave";
                windowType = "hanning";
                
                color.set(1, 1, 1);
                alpha = 0.5;
                
                freqInLoop = ofRandom((frequencies.at( 4 % freqsSize) - 20), (frequencies.at( 4 % freqsSize) + 20));
                
                
                break;
                //-------------------------------------
            case 'l':   // l  // , roll left
                
                
                waveType = "square";
                windowType = "blackH";
                
                freqInLoop = frequencies.at( 6 % freqsSize);
                break;
                //-------------------------------------
            case '/':   //47:// /, roll right
                
                waveType = "triWave";
                windowType = "hanning";
                freqInLoop = frequencies.at( 5 % freqsSize);
                
                
                break;
                //-------------------------------------
            case 'F':   //70:// F, move forward but not draw
                
                
                freqInLoop = ofRandom((frequencies.at( 7 % freqsSize) - 20), (frequencies.at( 7 % freqsSize) + 20));
                break;
                //-------------------------------------
            case 'G':   //71:// G, move forward but not draw
                
                waveType = "square";
                windowType = "hanning";
                
                
                
                break;
                //-------------------------------------
            case '[':   //91:// [   push matrix
                
                waveType = "sawnWave";
                windowType = "triWin";
                freqInLoop = ofRandom((frequencies.at( 8 % freqsSize) - 20), (frequencies.at( 8 % freqsSize) + 20));
                break;
                //-------------------------------------
            case ']':   // 93:// ]   pop matrix
                
                waveType = "square";
                windowType = "blackH";
                freqInLoop = ofRandom((frequencies.at( 9 % freqsSize) - 20), (frequencies.at( 9 % freqsSize) + 20));
                break;
                //-------------------------------------
            case 'a':
                
                //                waveType = "sineWave";
                
                
                freqInLoop = frequencies.at( 11 % freqsSize);
                
                break;
                //-------------------------------------
            case 'b':
                //                waveType = "sawnWave";
                //                windowType = "rectWin";
                freqInLoop = frequencies.at( 12 % freqsSize);
                
                break;
                //-------------------------------------
            case 'c':
                
                //                waveType = "square";
                
                freqInLoop = frequencies.at( 13 % freqsSize);
                break;
                //-------------------------------------
            case 'e':
                
                //                waveType = "triWave";
                
                
                freqInLoop = frequencies.at( 14 % freqsSize);
                
                break;
                //-------------------------------------
                
                //-------------------------------------
            case 'j':   // 93:// ]   pop matrix
                
                color.set(0,1,1);
                alpha = 0.5;
                drawSpeedMs = 100 * scaleDrawSpeed;
                
                break;
                //-------------------------------------
            case 'k':   // 93:// ]   pop matrix
                
                color.set(1,0,1);
                alpha = 0.75;
                drawSpeedMs = 200 * scaleDrawSpeed;
                
                break;
                
                //-------------------------------------
            case 'm':   // 93:// ]   pop matrix
                
                color.set(0.5,0.5,1);
                alpha = 0.75;
                //                drawSpeedMs = 400 * scaleDrawSpeed;
                break;
                //-------------------------------------
            default: {
                //                sineSynth = sineSynth;
                //                squareSynth = squareSynth;
            }
                break;
        }   // end switch
        
        // Increment here so stringCount[0] (index 0 on stringCount) is counted.
        stringCount++;
        
        
        if((stringCount % triggerRest ) ==  (triggerRest - 1))
        {
            //  triggerRest every so many characters. sets rest to true, which passes the rest on the audio loop
            
            rest = true;
            restCounter = 0;
            
            if(restDurGui <= 0.06){rest = false;} //  if less than 0.06 rest is false, no rest
            
        }
        
        
    }
    
    if(stringCount == currentLString.length()){     // important that is here so it counts to the end. Tested.
        drawGate = false;                           // close gate
        stringCount = 0;                            // reset string to 0
        cout<< " done drawing. " << endl;           // notify string has ended
    }
}
//-------------------------------------------------------
float ofApp::drunk(float& center, float wander, float lowerLimit, float higherLimit){
    // random walker. passed by reference to "center" that way it updates the variable automatically
    
    /* -syntax is now: drunk(someVariable, wander, loLimit, hiLimit)
     the variable is automatically updated.
     -I have integrated the limits to the function to save space in the code */
    
    center = (ofRandom(center - wander, center + wander));
    
    //  if goes below limit set to lowerLimit
    if (center < lowerLimit) {center = lowerLimit;}
    
    //  if goes above limit set to higherLimit
    else if (center > higherLimit) {center = higherLimit;}
    
    return center;
}

//-------------------------------------------------------
unsigned long ofApp::msToSamp(double ms){
    //  convert from ms to samples
    
    samples = round(ms * (sampleRate / 1000));  // rounds to nearest and returns an integer
    if(samples < 1) {samples = 1;}
    return samples;
}
//-------------------------------------------------------

void ofApp::characterCounter(bool countCharF){
    // this function counts how many turtle commands of each are in the string, it is just a convenience function
    
    if (countCharF){
        
        // reads through Linden String.
        for (int i = 0; i<currentLString.length(); i++){
            
            //--------------
            if(currentLString[i] == 'f'){ fCount++;}        // count number of fs
            else if(currentLString[i] == '-'){ minusCount++;}
            else if(currentLString[i] == '+'){ plusCount++;}
            else if(currentLString[i] == 'u'){ uCount++;}
            else if(currentLString[i] == 'd'){ dCount++;}
            else if(currentLString[i] == 'l'){ rollLCount++;}
            else if(currentLString[i] == '/'){ rollRCount++;}
            else if(currentLString[i] == '['){ pushCount++;}
            else if(currentLString[i] == ']'){ popCount++;}
            if(currentLString[i] == 'a'){ aCount++;}
            if(currentLString[i] == 'b'){ bCount++;}
            if(currentLString[i] == 'c'){ cCount++;}
            if(currentLString[i] == 'e'){ eCount++;}
            
            // outputs number of commands on string counted, not needed for the program
            if(i == currentLString.length() -1){
                
                cout << "//-----------------------------" << endl;
                cout << " fs = " << fCount << endl;
                cout << " - = " << minusCount
                << ", + = " << plusCount
                << ",  u = " << uCount
                << ", d = " << dCount << endl;
                cout << "  l = " << rollLCount
                << ", / = " << rollRCount
                << ",    [ = " << pushCount
                << ",    ] = " << popCount<< endl;
                
                cout << " a = " << aCount<< "  b = " << bCount<< "  c = " << cCount << "  e = " << eCount<< endl;
                
                fCount =0; minusCount =0; plusCount =0; pushCount =0; popCount =0; uCount =0; dCount = 0, rollLCount = 0 , rollRCount = 0;
                aCount = 0; bCount = 0; cCount = 0; eCount = 0;
                
                cout <<" _numVertices = "<< turtles[0].meshInClass.getNumVertices()<< endl;
            }
        }
    }
}
//-------------------------------------------------------
void ofApp::scrambleArrays(bool scramble){
    
    if(scramble){
        
        //  scramble the arrays holding rhythm, durations, frequencies, and amplitudes
        random_shuffle(rhythm.begin(), rhythm.end());
        random_shuffle(durations.begin(), durations.end());
        random_shuffle(frequencies.begin(), frequencies.end());
        random_shuffle(amplitudes.begin(), amplitudes.end());
        
        for(int i = 0; i <  rhythmSize; i++ )
        {cout << rhythm[i] << ", ";
        }
        cout << "__" << endl;
        for(int i = 0; i <  durationsSize; i++ )
        {cout << durations[i] << ", ";
        }
        cout << "__" << endl;
        for(int i = 0; i <  freqsSize; i++ )
        {cout << frequencies[i] << ", ";
        }
        cout << "__" << endl;
        cout << "------------"<< endl;
    }
}
//-------------------------------------------------------

