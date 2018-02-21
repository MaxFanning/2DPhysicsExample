////====================================================================================================
//// Filename:	AudioManager.cpp
//// Created by:	Craig Lacey
////====================================================================================================
//
//#include "Precompiled.h"
//#include "AudioManager.h"
//
//using namespace X;
//
//namespace
//{
//	uint32_t ComputeHash(const char* str)
//	{
//		// Based on djb2 implementation:
//		// http://www.cse.yorku.ca/~oz/hash.html
//
//		uint32_t hash = 5381;
//		int c;
//
//		while (c = *str++)
//		{
//			hash = ((hash << 5) + hash) + c;
//		}
//		return hash;
//	}
//
//	AudioManager* sAudioManager = nullptr;
//	std::string sPath;
//}
//
//void AudioManager::StaticInitialize(Audio::AudioEngineType type)
//{
//	XASSERT(sAudioManager == nullptr, "[AudioManager] System already initialized!");
//	sAudioManager = new AudioManager(type);
//}
//
////----------------------------------------------------------------------------------------------------
//
//void AudioManager::StaticTerminate()
//{
//	if (sAudioManager != nullptr)
//	{
//		sAudioManager->Clear();
//		SafeDelete(sAudioManager);
//	}
//}
//
////----------------------------------------------------------------------------------------------------
//
//AudioManager* AudioManager::Get()
//{
//	XASSERT(sAudioManager != nullptr, "[AudioManager] No system registered.");
//	return sAudioManager;
//}
//
////----------------------------------------------------------------------------------------------------
//
//void AudioManager::SetPath(const char* path)
//{
//	sPath = path;
//}
//
////----------------------------------------------------------------------------------------------------
//
//AudioManager::AudioManager()
//	:mType(Audio::AudioEngineType::DIRECTX)
//{
//	mpFMODSystem = nullptr;
//
//#ifdef _DEBUG
//	mDXEngineFlags = mDXEngineFlags | DirectX::AudioEngine_Debug;
//#endif
//	mpDXAudioEngine = new DirectX::AudioEngine(mDXEngineFlags);
//	XASSERT(mpDXAudioEngine != nullptr, "[AudioManager] Failed to Create AudioEngine");
//}
//
////----------------------------------------------------------------------------------------------------
//
//AudioManager::AudioManager(Audio::AudioEngineType type)
//	:mType(type)
//{
//	// DirectX Audio Init
//	if (type == Audio::AudioEngineType::DIRECTX)
//	{
//		mpFMODSystem = nullptr;
//
//#ifdef _DEBUG
//		mDXEngineFlags = mDXEngineFlags | DirectX::AudioEngine_Debug;
//#endif
//		mpDXAudioEngine = new DirectX::AudioEngine(mDXEngineFlags);
//		XASSERT(mpDXAudioEngine != nullptr, "[AudioManager] Failed to Create AudioEngine");
//	}
//	else // FMOD Audio Init
//	{
//		mpDXAudioEngine = nullptr;
//		// Create FMOD system
//		FMOD_RESULT result;
//		result = FMOD::System_Create(&mpFMODSystem);
//		if(result != FMOD_OK)
//		{
//			XLOG("[AudioEngine] Failed to create FMOD system.");
//		}
//
//		// Initialize FMOD system
//		result = mpFMODSystem->init(512, FMOD_INIT_NORMAL, 0);
//		if (result != FMOD_OK)
//		{
//			XLOG("[AudioEngine] Failed to initialize FMOD system.");
//			return;
//		}
//	}
//}
//
////----------------------------------------------------------------------------------------------------
//
//AudioManager::~AudioManager()
//{
//	if (mpDXAudioEngine)
//	{
//		delete mpDXAudioEngine;
//		mpDXAudioEngine = nullptr;
//	}
//
//	if (mpFMODSystem)
//	{
//		mpFMODSystem->close();
//		mpFMODSystem->release();
//	}
//}
//
////----------------------------------------------------------------------------------------------------
///*
//DirectX::AudioEngine::Update() returns false if no audio is actually playing
//(either due to there being no audio device on the system at the time AudioEngine was created,
//or because XAudio2 encountered a Critical Error--typically due to speakers being unplugged).
//Calls to various DirectXTK for Audio methods can still be made in this state but no actual audio processing will take place.
//https://directxtk.codeplex.com/wikipage?title=AudioEngine&referringTitle=Audio
//*/
//void AudioManager::Update()
//{
//	if (mpDXAudioEngine)
//	{
//		if (!mpDXAudioEngine->Update())
//		{
//			if (mpDXAudioEngine->IsCriticalError())
//			{
//				XLOG("[AudioManager] AudioEngine Critical Error"); // maybe we don't want to log per frame?
//			}
//		}
//	}
//	if (mpFMODSystem)
//	{
//		if (FMOD_OK != mpFMODSystem->update())
//		{
//			XLOG("[AudioManager] FMOD system update error");
//		}
//	}
//}
////----------------------------------------------------------------------------------------------------
//
//int AudioManager::LoadFromFile(const char* filename)
//{
//	uint32_t hash = ComputeHash(filename);
//	for (int i = 0; i < (int)mInventory.size(); ++i)
//	{
//		if (hash == mInventory[i].hash)
//		{
//			return i;
//		}
//	}
//
//	std::string fullname = sPath + "/" + std::string(filename);
//	mInventory.push_back(Entry());
//	mInventory.back().hash = hash;
//
//	switch (mType)
//	{
//	case Audio::AudioEngineType::DIRECTX:
//		mInventory.back().audio = new AudioSource(mType, nullptr);
//		mInventory.back().audio->Load(mpDXAudioEngine, fullname.c_str());
//		break;
//	case Audio::AudioEngineType::FMOD:
//		mInventory.back().audio = new AudioSource(mType, mpFMODSystem); // Passing a ref to the whole system is just ... wrong.
//		mInventory.back().audio->Load(mpFMODSystem, fullname.c_str());
//		break;
//	default:
//		XLOG("[AudioManager] LoadFromFile -- Unknown AudioEngineType");
//		exit(EXIT_FAILURE);
//	}
//
//	return mInventory.size() - 1;
//}
//
////----------------------------------------------------------------------------------------------------
//
//void AudioManager::Clear()
//{
//	for (auto& item : mInventory)
//	{
//		item.audio->Clear();
//		SafeDelete(item.audio);
//	}
//	mInventory.clear();
//}
//
////----------------------------------------------------------------------------------------------------
//
//AudioSource* AudioManager::GetAudio(int audioId)
//{
//	if (audioId < (int)mInventory.size())
//	{
//		return mInventory[audioId].audio;
//	}
//
//	return nullptr;
//}