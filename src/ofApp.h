#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxGui.h"
//#include "ofxOsc.h"
#include "myTurtleClass.h"
#include "grainClass.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void characterCounter(bool countCharF); // get character count
    void scrambleArrays(bool scramble);
    void audioOut(float * output, int bufferSize, int nChannels);// audio loop
    void drawSpeedLoop(bool metroBang);// trigger outside of loop
    void lSystem(bool generate);    // lSystem function
    float drunk(float& center, float wander, float lowerLimit, float higherLimit);  // my drunk
    unsigned long msToSamp(double ms);// miliseconds to samples
    void countCharForRest(int &charForRestGui);
    
    
    //sound, audio, etc
    ofSoundStream soundStream;
    unsigned bufferSize;
    int sampleRate;
    int sampleCount;
    double currentSample;
    double outputs[2];
    double leftChan;
    double rightChan;
    unsigned long samples;
    ofSoundDevice soundDevice;
    ofSoundBuffer buffer;
    
    // grains
    vector<grainClass> synths; // this is the new vector, no pointers!
    hannWinFunctor han;
    hammingWinFunctor hamm;
    blackmanHarrisWinFunctor blackH;
    gaussianWinFunctor gaus;
    rectWinFunctor rectWin;
    triangleWinFunctor triWin;
    
    float dur, amp; double freqInLoop;
    string waveType, windowType;
    float durS, ampS; double freqInLoopS;// store for repetition of grains
    string waveTypeS, windowTypeS;
    
    //count
    int arraysCounter;
    unsigned long rhythmSize, durationsSize, freqsSize, ampSize;// size of vectors. to avoid extra calculations in loops
    vector < float > rhythm; //  in drawSpeedLoop function
    vector <float > durations;
    vector <float> frequencies;
    vector <float> amplitudes;
    
    
    int stringCount;             // used inside audoiOut
    bool metronome, drawGate;    // used inside audioOut
    
//    ofxOscSender sender;         //not being used at the moment
    
    //turtle
    vector<myTurtleClass> turtles;
    vector<myTurtleClass> turtleWKeys;
    ofVec3f startPos, lookAt, heading, color, color2;
    ofVec3f cV;         // color vector test
    float alpha;
    unsigned int fCount, minusCount, plusCount, pushCount, popCount, uCount, dCount, rollLCount, rollRCount, aCount, bCount, cCount, eCount;
    
    bool countChar;
    
    //cam
    ofEasyCam cam;
    float camDist;
    
    ofxPanel gui;
    ofxPanel gui2;
    
    // Gui --------------
    ofxFloatSlider stepSizeGui, drawSpeedLowGui, drawSpeedHighGui, thetaGuiRand, thetaGui, durGui, scaleDrawSpeed, restDurGui;
    ofxIntSlider charForRestGui, repeatGrainGui;
    ofxFloatSlider rotateXGui, rotateYGui, rotateZGui;
    ofxToggle dupVertex;

    ofxLabel screenSize;
    bool bHide, bInfoText; // hides panel when 'n' is pressed

    //------------
    vector< ofSpherePrimitive > spheres;
    float sDFC; //  sphereDistanceFromCenter
    
//    ofSpherePrimitive sphere;
//    storeNumber storedNumber;
};
