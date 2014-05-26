#include <Engine/Audio.hpp>

#include <cstring>
#include <cassert>
#include <limits>
#include <iostream>

namespace Engine
{
	Audio& Audio::GetInstance()
	{
		static Audio instance;
		return instance;
	}

	void Audio::Play(std::shared_ptr<IAudioSource> source,
		std::function<void(std::shared_ptr<IAudioSource>)> callback)
	{
		if (source)
		{
			Track track;
			track.source = source;
			track.callback = callback;
			track.currentPositionInSamples = 0;
			m_tracks.push_back(track);
		}
	}

	void Audio::StopAll()
	{
		m_tracks.clear();
	}

	Audio::Audio()
	: m_tracks()
	{
		PaError status;

		// Try to initialize PortAudio.
		status = Pa_Initialize();
		if (status != paNoError)
		{
			std::cerr << "Error: Unable to initialize PortAudio" << std::endl;
			std::cerr << Pa_GetErrorText(status) << std::endl;
			exit(1); // We can't recover from this.
		}

		// Try to open an audio I/O stream.
		status = Pa_OpenDefaultStream(
			&m_stream,
			0, // No input channels
			2, // Default stereo output (2 channels)
			paInt16, // Samples are 16-bit integers
			44100, // Sample rate
			paFramesPerBufferUnspecified, // Let PortAudio choose the number of frames passed to the stream callback function
			&Audio::StreamCallback, // Pointer to the stream callback function
			this // User pointer
		);
		if (status != paNoError)
		{
			std::cerr << "Error: Failed to open a PortAudio stream" << std::endl;
			std::cerr << Pa_GetErrorText(status) << std::endl;
			exit(1); // We can't recover from this.
		}

		// Try to start the stream.
		status = Pa_StartStream(m_stream);
		if (status != paNoError)
		{
			std::cerr << "Error: Failed to start a PortAudio stream" << std::endl;
			std::cerr << Pa_GetErrorText(status) << std::endl;
			exit(1); // We can't recover from this.
		}
	}

	Audio::~Audio()
	{
		PaError status;

		// Try to stop the stream.
		status = Pa_StopStream(m_stream);
		if (status != paNoError)
		{
			std::cerr << "Error: Failed to successfully stop the PortAudio stream" << std::endl;
			std::cerr << Pa_GetErrorText(status) << std::endl;
		}

		// Try to terminate PortAudio.
		status = Pa_Terminate();
		if (status != paNoError)
		{
			std::cerr << "Error: Failed to successfully terminate PortAudio" << std::endl;
			std::cerr << Pa_GetErrorText(status) << std::endl;
		}
	}

	int Audio::StreamCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags, void* userPointer)
	{
		// Cast the user pointer.
		Audio* audio = static_cast<Audio*>(userPointer);

		// Cast the output buffer to the type specified when opening the stream.
		float* out = static_cast<float*>(outputBuffer);
		(void) inputBuffer; // Prevent "unused variable" warnings.

		// Create a temporary array to hold mixed sample data and initialize
		// to zeros.
		std::int16_t* mixedSamples = new std::int16_t[framesPerBuffer * 2];
		std::memset(mixedSamples, 0, sizeof(std::int16_t) * framesPerBuffer * 2);

		// Create a temporary array to hold each track's samples.
		std::int16_t* trackSamples = new std::int16_t[framesPerBuffer * 2];

		// Get the number of tracks currently playing.
		const unsigned int numTracks = audio->m_tracks.size();

		// Iterate over all of the tracks currently playing.
		auto iter = audio->m_tracks.begin();
		while (iter != audio->m_tracks.end())
		{
			// Get the track.
			Track& track = *iter;

			// Check whether the track has finished.
			if (track.currentPositionInSamples >= track.source->GetSamplesCount())
			{
				track.callback(track.source);
				iter = audio->m_tracks.erase(iter);
			}
			else
			{
				// Populate the track's samples array.
				track.source->GetSamples(trackSamples, framesPerBuffer * 2, track.currentPositionInSamples);

				// Mix the track's samples.
				for (std::size_t i = 0; i < framesPerBuffer * 2; ++i)
				{
					// We divide the track sample by a suitable value so
					// that we don't clip the samples very frequently.
					const int sampleSum = static_cast<int>(mixedSamples[i]) + static_cast<int>(trackSamples[i]) / 5;

					// Now perform sample clipping.
					if (sampleSum >= std::numeric_limits<std::int16_t>::max())
					{
						mixedSamples[i] = std::numeric_limits<std::int16_t>::max();
					}
					else if (sampleSum <= std::numeric_limits<std::int16_t>::min())
					{
						mixedSamples[i] = std::numeric_limits<std::int16_t>::min();
					}
					else
					{
						mixedSamples[i] = static_cast<std::int16_t>(sampleSum);
					}
				}

				// Increment the track's position.
				track.currentPositionInSamples += framesPerBuffer * 2;

				// Increment the track iterator.
				++iter;
			}
		}

		// Copy the mixed samples over to the output buffer.
		std::memcpy(out, mixedSamples, sizeof(std::int16_t) * framesPerBuffer * 2);

		// Delete our temporary data arrays.
		delete[] trackSamples;
		delete[] mixedSamples;

		// Continue streaming...
		return 0;
	}
}
