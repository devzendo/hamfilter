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

int inout(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
		double streamTime, RtAudioStreamStatus status, void *userData) {
//	cout << "record callback triggered" << endl;

	if (status & RTAUDIO_INPUT_OVERFLOW != 0)
		cout << "Stream input overflow detected!" << endl;
	if (status & RTAUDIO_OUTPUT_UNDERFLOW != 0)
		cout << "Stream output underflow detected!" << endl;

	// If allocate pool buffer OK, copy inputBuffer to it, pass into DSP chain.

	// Try to take from end of DSP chain. If anything available, copy outputBuffer and free.
	// Else, zero outputBuffer.

	// For now...
	unsigned int *bytes = (unsigned int *) userData;
	if (*bytes != 0) {
	//	cout << "obuf " << outputBuffer << " ibuf " << inputBuffer << " nframes " << nBufferFrames
	//			<< " userData " << userData << " bytes " << *bytes << endl;
		memcpy(outputBuffer, inputBuffer, *bytes);
	}

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

	// Initialise buffer management
	// Initialise DSP thread
	// Initialise GUI

	unsigned int sampleRate = 44100;
	unsigned int bufferFrames = 512;
	unsigned int bufferBytes = 0;
	RtAudio::StreamParameters inputParameters;
	inputParameters.deviceId = inputDevice;
	inputParameters.nChannels = 2;
	inputParameters.firstChannel = 0;

	RtAudio::StreamParameters outputParameters;
	outputParameters.deviceId = outputDevice;
	outputParameters.nChannels = 2;
	outputParameters.firstChannel = 0;

	try {
		adc.openStream(&outputParameters, &inputParameters, RTAUDIO_SINT16, sampleRate,
				&bufferFrames, &inout, &bufferBytes);
		adc.startStream();
	} catch (RtAudioError& e) {
		e.printMessage();
		exit(0);
	}
	// adc.openStream could have adjusted the bufferFrames.
	// Set the user data buffer to the sample buffer size in bytes, so that the
	// inout callback function knows how much data to copy. The example code
	// uses this - 2 is Stereo, 4 is signed int (4 bytes on OSX)
	bufferBytes = bufferFrames * 2 * 4;

	// Can now initialise buffer management. inout could have been asking for
	// buffers but buffer management won't give them until it has been
	// initialised.

	cout << "buffer size in bytes is " << bufferBytes << endl;

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
