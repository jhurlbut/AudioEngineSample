#pragma once

#include "cinder/Utilities.h"
#include "cinder/app/App.h"
#include "FMOD.hpp"
#include "fmod_errors.h"

static void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		cinder::app::console()<<"FMOD error! result "<< result << " error string: " << FMOD_ErrorString(result) << std::endl;
	}
}

struct SoundItem {
	std::string id;
	std::string filename;
	FMOD::Sound      *sound;
	FMOD::Channel	*channel;
	bool playing;
};