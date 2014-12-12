//============================================================================
// Name        : hamfilter.cpp
// Author      : Matt Gumbley
// Version     : 1.0
// Copyright   : All rights reserved, all wrongs denied.
// Description : Noise/bandpass filter for CW/SSB.
//============================================================================

#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

#include <RtAudio.h>

int record(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
		double streamTime, RtAudioStreamStatus status, void *userData) {
	cout << "record callback triggered" << endl;
	if (status)
		cout << "Stream overflow detected!" << endl;
	// Do something with the data in the "inputBuffer" buffer.
	return 0;
}

void showDevices(unsigned int deviceCount, RtAudio& adc) {
	// Scan through devices for various capabilities
	RtAudio::DeviceInfo info;
	for (unsigned int i = 0; i < deviceCount; i++) {
		info = adc.getDeviceInfo(i);
		if (info.probed == true) {
			// Print, for example, the maximum number of output channels for each device
			cout << "device = " << i;
			cout << ": maximum output channels = " << info.outputChannels
					<< endl;
		}
	}
	exit(0);
}

int main(const int argc, const char *argv[]) {
	RtAudio adc;
	unsigned int deviceCount = adc.getDeviceCount();
	cout << "Audio device count: " << deviceCount << endl;
	if (deviceCount < 1) {
		cout << endl << "No audio devices found!" << endl;
		exit(0);
	}

	for (int i=0; i<argc; i++) {
		if (strcmp(argv[i], "-devices") == 0) {
			// Scan through devices for various capabilities
			showDevices(deviceCount, adc);
		}
	}
	RtAudio::StreamParameters parameters;
	parameters.deviceId = adc.getDefaultInputDevice();
	parameters.nChannels = 2;
	parameters.firstChannel = 0;
	unsigned int sampleRate = 44100;
	unsigned int bufferFrames = 256; // 256 sample frames
	try {
		adc.openStream( NULL, &parameters, RTAUDIO_SINT16, sampleRate,
				&bufferFrames, &record);
		adc.startStream();
	} catch (RtAudioError& e) {
		e.printMessage();
		exit(0);
	}
	char input;
	std::cout << "\nRecording ... press <enter> to quit.\n";
	std::cin.get(input);
	try {
		// Stop the stream
		adc.stopStream();
	} catch (RtAudioError& e) {
		e.printMessage();
	}
	if (adc.isStreamOpen())
		adc.closeStream();
	return 0;
}
