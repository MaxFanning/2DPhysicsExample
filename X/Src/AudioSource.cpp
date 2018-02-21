////====================================================================================================
//// Filename:	AudioSource.cpp
//// Created by:	Craig Lacey
////====================================================================================================
//
//#include "Precompiled.h"
//#include "AudioManager.h"
//#include "AudioSource.h"
//
//using namespace X;
//
//AudioSource::AudioSource()
//	:mFMODSystem(nullptr)
//{
//	mSoundEffect = nullptr;
//	mLoopingEffect = nullptr;
//	mType = Audio::AudioEngineType::UNKNOWN;	
//	mFMODSound = nullptr;
//	mFMODChannel = 0;
//}
//
////----------------------------------------------------------------------------------------------------
//
//AudioSource::AudioSource(Audio::AudioEngineType type, FMOD::System* fmodSystem)
//	:mFMODSystem(fmodSystem)
//{
//	mSoundEffect = nullptr;
//	mLoopingEffect = nullptr;
//	mType = type;
//	mFMODSound = nullptr;
//	mFMODChannel = 0;
//}
//
////----------------------------------------------------------------------------------------------------
//
//AudioSource::~AudioSource()
//{
//	if (mFMODSound)
//	{
//		mFMODSound->release();
//	}
//	if (mFMODChannel)
//	{
//		delete mFMODChannel;
//		mFMODChannel = nullptr;
//	}
//	// DO NOT DELETE THE FMOD SYSTEM !!!
//}
//
////----------------------------------------------------------------------------------------------------
//
//void AudioSource::Load(DirectX::AudioEngine* pDXAudioEngine, const char* filename)
//{
//	// convert char* file name into wchar_t*
//	const size_t size = strlen(filename)+1;
//	wchar_t* str = new wchar_t[size];
//	mbstowcs(str, filename, size);
//
//	mSoundEffect = new DirectX::SoundEffect(pDXAudioEngine, str);
//	delete[]str; // we shouldn't need it now... right?
//}
//
////----------------------------------------------------------------------------------------------------
//
//void AudioSource::Load(FMOD::System* pFMODSystem, const char* filename)
//{
//	if (FMOD_OK != pFMODSystem->createSound(filename, FMOD_DEFAULT, 0, &mFMODSound))
//	{
//		XLOG("[AudioSource] Error Loading Sound File -- Could not create fmod sound from file: %s", filename);
//		return;
//	}
//}
//
////----------------------------------------------------------------------------------------------------
//
//void AudioSource::Clear()
//{
//	if (mSoundEffect)
//	{
//		delete mSoundEffect;
//		mSoundEffect = nullptr;
//	}
//
//	if (mLoopingEffect)
//	{
//		delete mLoopingEffect;
//		mLoopingEffect = nullptr;
//	}
//
//	if (mFMODSound)
//	{
//		mFMODSound->release();
//	}
//}
//
////----------------------------------------------------------------------------------------------------
//
//void AudioSource::Play(bool loop) const
//{
//	switch (mType)
//	{
//	case Audio::AudioEngineType::DIRECTX:
//		if (loop)
//		{
//			// does not seem to work
//			auto effect = mSoundEffect->CreateInstance();
//			mEffectInstance = effect.get();
//			mEffectInstance->Play(true);
//
//			if (effect->IsLooped() && effect->GetState() != DirectX::SoundState::STOPPED)
//			{
//				XLOG("Playing Looped Audio");
//			}
//		}
//		else
//		{
//			mSoundEffect->Play();
//		}
//		break;
//	case Audio::AudioEngineType::FMOD:
//		if (loop)
//		{
//			mFMODSound->setMode(FMOD_LOOP_NORMAL);
//			FMOD::Channel* pChannel = 0; // FIXME - can't use &mFMODChannel because of default const qualifier
//			mFMODSystem->playSound(mFMODSound, 0, false, &pChannel);
//		}
//		else
//		{
//			mFMODSound->setMode(FMOD_LOOP_OFF);
//			FMOD::Channel* pChannel = 0; // FIXME - can't use &mFMODChannel because of default const qualifier
//			mFMODSystem->playSound(mFMODSound, 0, false, &pChannel);
//		}
//		break;
//	default:
//		XLOG("[AudioSource] Could not play audio because engine type is unhandled");
//		break;
//	}
//}
//
////----------------------------------------------------------------------------------------------------
//
//void AudioSource::Play(bool loop, float vol, float pitch, float pan) const
//{
//	if (!IsPlaying())
//	{
//		if (loop)
//		{
//			auto effect = mSoundEffect->CreateInstance();
//			effect->Play(true);
//			if (effect->IsLooped())
//			{
//				XLOG("Playing Looped Audio");
//			}
//		}
//		else
//		{
//			mSoundEffect->Play(vol, pitch, pan);
//		}
//	}
//}
//
////----------------------------------------------------------------------------------------------------