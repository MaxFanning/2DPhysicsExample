//
////====================================================================================================
//// Filename:	AudioManager.h
//// Created by:	Craig Lacey
////====================================================================================================
//
//#ifndef INCLUDED_XENGINE_AUDIOMANAGER_H
//#define INCLUDED_XENGINE_AUDIOMANAGER_H
//
//#include <fmod_studio.hpp>
//#include <fmod.hpp>
//
//#include "XCore.h"
//#include "XAudioTypes.h"
//#include "Audio.h"
//#include "AudioSource.h"
//
//namespace X {
//
//	//====================================================================================================
//	// Class Declarations
//	//====================================================================================================
//	class AudioManager
//	{
//		XNONCOPYABLE(AudioManager);
//
//	public:
//		static void StaticInitialize(Audio::AudioEngineType engineType);
//		static void StaticTerminate();
//		static AudioManager* Get();
//
//		static void SetPath(const char* path);
//
//		static const int skNumAudioChannels = 32;
//
//	public:
//		AudioManager();
//		AudioManager(Audio::AudioEngineType type);
//		~AudioManager();
//
//		void Update();
//		void Clear();
//
//		int LoadFromFile(const char* filename);
//		AudioSource* GetAudio(int audioId);
//
//	private:
//		const Audio::AudioEngineType mType;
//		FMOD::System* mpFMODSystem;
//
//		DirectX::AUDIO_ENGINE_FLAGS mDXEngineFlags = DirectX::AudioEngine_Default;
//		DirectX::AudioEngine* mpDXAudioEngine;
//
//		struct Entry
//		{
//			uint32_t hash;
//			AudioSource* audio;
//		};
//		std::vector<Entry> mInventory;
//	};
//
//} // namespace 
//
//#endif // #ifndef INCLUDED_XENGINE_AUDIOMANAGER_H