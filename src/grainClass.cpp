//
//  grainClass.cpp
//  turtle_newClass_03_05_16
//
//  Created by Irving Angulo on 3/5/16.
//
//

#include "grainClass.h"

//--------------------------------------------------------------
grainClass::grainClass(){
    // default constructor
    mFreq = 400;
    mAmp = 0.1;
    mPan = 0.5;
    mDoSynth = false;
    dur = 48000;
    mWindowPos = 0;
    
    waveType = "sine";
    windowType = "hanning";
}

//--------------------------------------------------------------
grainClass::grainClass(double freq, double amp, double pan, bool doSynth, unsigned long lengthOfWindow) {
    //  overload constructor
    
    mFreq = freq;
    mAmp = amp;
    mPan = pan;
    mDoSynth = doSynth;
    mWindowPos = 0;
    dur = lengthOfWindow;
    
    waveType = "sine";
    windowType = "hanning";
}

//--------------------------------------------------------------
grainClass::~grainClass(){
    //  destructor
    
}
//--------------------------------------------------------------


//--------------------------------------------------------------
// Accessor functions

double grainClass::get_mFreq() const {
    //  accessor for mFreq; get mFreq
        return mFreq;
}
//--------------------------------------------------------------
long grainClass::get_mWindowPos() const {
    return mWindowPos; // accessor for mWindowPos; get mWindowPos
}
//--------------------------------------------------------------
double grainClass::get_mAmp() const{
    return mAmp;
}
//--------------------------------------------------------------
bool grainClass::get_mDoSynth() const{

    return mDoSynth;
}
//--------------------------------------------------------------
double grainClass::get_mPan() const{
    return mPan;
}
//--------------------------------------------------------------
unsigned long grainClass::get_dur() const{
    return dur;
}
//--------------------------------------------------------------
string grainClass::getWinType() const {
    return windowType;
}
//--------------------------------------------------------------
string grainClass::getWaveType() const {
    return waveType;
}

//--------------------------------------------------------------



//--------------------------------------------------------------
//  Mutator functions
//--------------------------------------------------------------
void grainClass::set_mFreq(double set_mFreq){
    mFreq = set_mFreq; // change/set mFreq
}
//--------------------------------------------------------------
void grainClass::set_mAmp(double set_mAmp){
    mAmp = set_mAmp; // change/set mAmp
}
//--------------------------------------------------------------
void grainClass::set_mWindowPos(long set_mWindowPos){
    mWindowPos = set_mWindowPos;
}
//--------------------------------------------------------------
void grainClass::set_mDoSynth(bool set_mDoSynth){
    mDoSynth = set_mDoSynth;
}
//--------------------------------------------------------------
void grainClass::set_mPan(double set_mPan){
    mPan = set_mPan;
}
//--------------------------------------------------------------
void grainClass::set_dur(unsigned long set_dur){
    dur = set_dur;
}
//--------------------------------------------------------------
void grainClass::setWinType(string mWindowType){
    windowType = mWindowType;
    
}
//--------------------------------------------------------------
void grainClass::setWaveType(string mWaveType){
    waveType = mWaveType;
}




