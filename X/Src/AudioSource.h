////====================================================================================================
//// Filename:	AudioObject.h
//// Created by:	Craig Lacey
////====================================================================================================
//
//#ifndef INCLUDED_XENGINE_AUDIOOBJECT_H
//#define INCLUDED_XENGINE_AUDIOOBJECT_H
//
//#include "XAudioTypes.h"
//#include "Audio.h"
//#include <fmod.h>
//
//namespace X {
//
//	class AudioSource
//	{
//	public:
//		AudioSource();
//		AudioSource(Audio::AudioEngineType type, FMOD::System* system = nullptr);
//		~AudioSource();
//
//		void Load(FMOD::System* pFMODSystem, const char* filename);
//		void Load(DirectX::AudioEngine* pDXAudioEngine, const char* filename);
//		void Clear();
//		
//		void Play(bool loop) const;
//		void Play(bool loop, float vol, float pitch, float pan) const;
//
//		size_t GetDuration() const { return mSoundEffect != nullptr ? mSoundEffect->GetSampleDurationMS() : 0L; }
//		bool IsPlaying() const { return mSoundEffect != nullptr ? mSoundEffect->IsInUse() : false; }
//
//	private:
//		Audio::AudioEngineType mType;
//		mutable DirectX::SoundEffectInstance* mEffectInstance;
//		DirectX::SoundEffect* mSoundEffect;
//		mutable DirectX::SoundEffectInstance* mLoopingEffect;
//
//		FMOD::System* const mFMODSystem; // HACK
//		FMOD::Sound* mFMODSound;
//		FMOD::Channel* mFMODChannel; // mutable?
//	};
//
//} // namespace X
//
//#endif // #ifndef INCLUDED_XENGINE_AUDIOOBJECT_H
