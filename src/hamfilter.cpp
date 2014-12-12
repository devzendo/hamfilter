//============================================================================
// Name        : hamfilter.cpp
// Author      : Matt Gumbley
// Version     : 1.0
// Copyright   : All rights reserved, all wrongs denied.
// Description : Noise/bandpass filter for CW/SSB.
//============================================================================

#include <iostream>
#include <vector>
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

bool isCdQuality(RtAudio::DeviceInfo info) {
	bool cdQuality = false;
	for (unsigned int i=0; i<info.sampleRates.size(); i++) {
		if (info.sampleRates[i] == 44100) {
			cdQuality = true;
			break;
		}
	}
	return cdQuality;
}

void showDevices(unsigned int deviceCount, RtAudio& adc) {
	// Scan through devices for various capabilities
	RtAudio::DeviceInfo info;
	for (unsigned int i = 0; i < deviceCount; i++) {
		info = adc.getDeviceInfo(i);
		if (info.probed == true) {
			cout << "Device " << i << " '" << info.name << "', ";
			cout << "max output channels = " << info.outputChannels;
			cout << (info.isDefaultOutput ? " (default output)" : "");
			cout << ", max input channels = " << info.inputChannels;
			cout << (info.isDefaultInput ? " (default input)" : "");
			if (isCdQuality(info)) {
				cout << ", CD quality (44,100 Hz)";
			}
			cout << endl;
		}
	}
}

void usage() {
	cout << "Syntax: hamfilter [options]" << endl;
	cout << "Options:" << endl;
	//       12345678901234567890123456789012345678901234567890123456789012345678901234567890
	cout << "  -devices        - Shows input/output device details and their ID numbers" << endl;
	cout << "  -input <id>     - Uses device <id> as the audio input (see -devices)" << endl;
	cout << "  -output <id>    - Uses device <id> as the audio output (see -devices)" << endl;
}

void validateDevice(unsigned int deviceId, unsigned int deviceCount, RtAudio& adc, bool input) {
	if (deviceId >= deviceCount) {
		cout << "Device ID " << deviceId << " must be less than "
				<< deviceCount << endl;
		exit(0);
	}
	RtAudio::DeviceInfo info = adc.getDeviceInfo(deviceId);
	if (!info.probed) {
		cout << "Could not probe device ID " << deviceId << endl;
		exit(0);
	}
	if (!isCdQuality(info)) {
		cout << "Device ID " << deviceId << " is not CD quality" << endl;
		exit(0);
	}
	if (input) {
		if (info.inputChannels == 0) {
			cout << "Device ID " << deviceId << " has no input channels" << endl;
			exit(0);
		}
	} else {
		if (info.outputChannels == 0) {
			cout << "Device ID " << deviceId << " has no output channels" << endl;
			exit(0);
		}
	}
}

int main(const int argc, const char *argv[]) {
	RtAudio adc;
	unsigned int deviceCount = adc.getDeviceCount();
	if (deviceCount < 1) {
		cout << endl << "No audio devices found!" << endl;
		exit(0);
	}

	unsigned int inputDevice = adc.getDefaultInputDevice();
	unsigned int outputDevice = adc.getDefaultOutputDevice();
	for (int i=0; i<argc; i++) {
		if (strcmp(argv[i], "-devices") == 0) {
			// Scan through devices for various capabilities
			showDevices(deviceCount, adc);
			exit(0);
		}
		if (strcmp(argv[i], "-input") == 0) {
			if (i == argc-1) {
				usage();
				exit(0);
			}
			inputDevice=atoi(argv[++i]);
			validateDevice(inputDevice, deviceCount, adc, true);
		}
		if (strcmp(argv[i], "-output") == 0) {
			if (i == argc-1) {
				usage();
				exit(0);
			}
			outputDevice=atoi(argv[++i]);
			validateDevice(outputDevice, deviceCount, adc, false);
		}
	}

	RtAudio::StreamParameters parameters;
	parameters.deviceId = inputDevice;
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
