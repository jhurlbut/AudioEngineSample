#pragma once

#include "cinder/app/AppBasic.h"
#include "FMOD.hpp"
#include "AudioEngine/SoundItem.h"
#include "Events/Events.h"

class AudioDevice;
typedef boost::shared_ptr< AudioDevice > AudioDeviceRef;

class AudioDevice
{
    public:

		static AudioDeviceRef create( int pDeviceID )
		{ return (AudioDeviceRef)(new AudioDevice(pDeviceID)); }

		static AudioDeviceRef create()
		{ return (AudioDeviceRef)(new AudioDevice()); }

        AudioDevice(int pDeviceID);
		AudioDevice();
        void update();
		int setup();
		FMOD::System*	getSystem() { return mSystem; }
		int				getDeviceID(){ return mDeviceID; }
		int				playSound(SoundItem pSoundItem );
		int				createSound(SoundItem pSoundItem);
		int				shutDown();

		 template< typename listener_t >
        ci::CallbackId					registerEventListener( listener_t* obj, void ( listener_t::*callback )( const AudioEngineEvent& ) )
        {
            return mAudioEngineEventCallbackMgr.registerCb( std::bind( callback, obj, std::placeholders::_1 ) );
        }
    private:
		 //external callback manager
        typedef ci::CallbackMgr< void( const AudioEngineEvent& ) >		AudioEngineEventCallbackMgr;
        AudioEngineEventCallbackMgr										mAudioEngineEventCallbackMgr;
		

        FMOD::System*				mSystem;
		std::vector<SoundItem>		mSounds;
		int							mDeviceID;
		void						createDsp( FMOD_DSP_TYPE type, FMOD::DSP** dsp );
		float						mSoundLevel;
		float						mSoundSpeed;
};
