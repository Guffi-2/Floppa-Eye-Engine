#pragma once

#include <al.h>
#include <alc.h>

#pragma comment(lib, "OpenAL32.lib")

bool Init() {
	auto Device = alcOpenDevice(NULL); // select the "preferred device"
	if (Device) {
		auto Context = alcCreateContext(Device, NULL);
		alcMakeContextCurrent(Context);
	}
	// Check for EAX 2.0 support
	auto g_bEAX = alIsExtensionPresent("EAX2.0");
	// Generate Buffers
	alGetError(); // clear error code
	alGenBuffers(NUM_BUFFERS, g_Buffers);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DisplayALError("alGenBuffers :", error);
		return;
	}
	// Load test.wav
	loadWAVFile("test.wav", &format, &data, &size, &freq, &loop);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DisplayALError("alutLoadWAVFile test.wav : ", error);
		alDeleteBuffers(NUM_BUFFERS, g_Buffers);
		return;
	}
	// Copy test.wav data into AL Buffer 0
	alBufferData(g_Buffers[0], format, data, size, freq);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DisplayALError("alBufferData buffer 0 : ", error);
		alDeleteBuffers(NUM_BUFFERS, g_Buffers);
		return;
	}
	// Unload test.wav
	unloadWAV(format, data, size, freq);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DisplayALError("alutUnloadWAV : ", error);
		alDeleteBuffers(NUM_BUFFERS, g_Buffers);
		return;
	}
	// Generate Sources
	alGenSources(1, source);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DisplayALError("alGenSources 1 : ", error);
		return;
	}
	// Attach buffer 0 to source
}