#include "AudioEngine/AudioDevice.h"

using namespace ci;
using namespace ci::app;
using namespace std;

AudioDevice::AudioDevice(int pDeviceID){
	mDeviceID = pDeviceID;
}
AudioDevice::AudioDevice(){
	mDeviceID = -1;
}
int AudioDevice::setup()
{
	mSystem					= 0;
	
	// Basic FMOD system initialization
	FMOD::System_Create( &mSystem );
	if ( mSystem == 0 ) {
		console() << "Unable to create system" << endl;
		return -1;
	}
	FMOD_OUTPUTTYPE type;
	mSystem->getOutput(&type);
	console() <<"output type " << type << endl;
	if(mDeviceID != -1)
		mSystem->setDriver(mDeviceID);
	if ( mSystem->init( 32, FMOD_INIT_NORMAL, 0 ) != FMOD_OK ) {
		console() << "Unable to initialize system. deviceid " << mDeviceID << endl;
		return -1;
	}
	
	return 0;
}
int AudioDevice::playSound(SoundItem pSoundItem ){
	int soundindex = 0;
	for(int i=0; i<mSounds.size(); i++){
		if(mSounds[i].id == pSoundItem.id){
			soundindex = i;
			break;
		}
	}
	FMOD::Channel         *channel = 0;
	int result = mSystem->playSound(FMOD_CHANNEL_FREE, mSounds[soundindex].sound, false, &mSounds[soundindex].channel);
	//ERRCHECK((FMOD_RESULT)result);
	console()<<"playSound id " << mSounds[soundindex].id << " result " << result << endl;
	if(result == 0){
		mSounds[soundindex].playing = true;
	}
	return result;

}
int AudioDevice::createSound(SoundItem pSoundItem){
	SoundItem sound;
	mSounds.push_back(sound);
	SoundItem snd = mSounds[mSounds.size()-1];
	mSounds[mSounds.size()-1].filename = pSoundItem.filename;
	mSounds[mSounds.size()-1].id = pSoundItem.id;
	mSounds[mSounds.size()-1].sound = 0;
	mSounds[mSounds.size()-1].channel = 0;
	mSounds[mSounds.size()-1].playing = false;
	int result = mSystem->createSound(("../assets/sounds/"+mSounds[mSounds.size()-1].filename).c_str(), FMOD_SOFTWARE, 0, &mSounds[mSounds.size()-1].sound);
	ERRCHECK((FMOD_RESULT)result);
	console()<<"createSound deviceID " << mDeviceID << " result " << result << endl;
	
	return result;
}
int AudioDevice::shutDown()
{
	for ( auto snd : mSounds ){
		int result = snd.sound->release();
		ERRCHECK((FMOD_RESULT)result);
	}
	int result = mSystem->close();
	ERRCHECK((FMOD_RESULT)result);
	result = mSystem->release();
	ERRCHECK((FMOD_RESULT)result);
	return result;
}
void AudioDevice::update()
{
	for ( int i = 0; i< mSounds.size(); i++ ){
		bool prevPlay = mSounds[i].playing;
		int result = mSounds[i].channel->isPlaying(&mSounds[i].playing);
		if(prevPlay != mSounds[i].playing){
			if(mSounds[i].playing == false){
				AudioEngineEvent event;
				event.sigType = SOUND_STOPPED_PLAYING;
				event.soundID = mSounds[i].id;
				event.deviceID = mDeviceID;
				mAudioEngineEventCallbackMgr.call( event );
			}
		}
	}
}
 