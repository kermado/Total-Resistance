#ifndef AUDIO_H
#define AUDIO_H

#include <memory>
#include <vector>
#include <functional>

#include <portaudio.h>

#include <Engine/NonCopyable.hpp>
#include <Engine/IAudioSource.hpp>

namespace Engine
{
	class Audio : private NonCopyable
	{
	public:
		/**
		 * Returns a reference to the singleton instance.
		 *
		 * @return Reference to the singleton instance.
		 */
		static Audio& GetInstance();

		/**
		 * Starts playing the specified audio source.
		 *
		 * @param source Shared pointer to the audio source to play.
		 */
		void Play(std::shared_ptr<IAudioSource> source,
			std::function<void(std::shared_ptr<IAudioSource>)> callback = [](std::shared_ptr<IAudioSource>) {});

		/**
		 * Immediately ends all currently playing audio sources.
		 *
		 * @note Callback functions are not executed.
		 */
		void StopAll();

	private:
		/**
		 * Structure that represents the state of an audio source.
		 */
		struct Track
		{
			std::shared_ptr<IAudioSource> source;
			std::function<void(std::shared_ptr<IAudioSource>)> callback;
			unsigned long currentPositionInSamples;
		};

	private:
		/**
		 * Constructor.
		 */
		Audio();

		/**
		 * Destructor.
		 */
		~Audio();

		/**
		 * Audio stream callback function.
		 * This function is exected by PortAudio whenever it needs more audio
		 * data for output.
		 *
		 * @param inputBuffer Input buffer.
		 * @param outputBuffer Output buffer.
		 * @param framesPerBuffer Frames per buffer.
		 * @param timeInfo Timing information for the buffers.
		 * @param statusFlags Bit field used to indicate I/O status.
		 * @param userPointer User pointer passed when opening the stream.
		 * @return Status (0 = continue, 1 = complete, 2 = abort).
		 */
		static int StreamCallback(const void *inputBuffer, void *outputBuffer,
			unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
			PaStreamCallbackFlags statusFlags, void* userPointer);

	private:
		/**
		 * The audio stream.
		 */
		PaStream* m_stream;

		/**
		 * The audio sources currently being played.
		 */
		std::vector<Track> m_tracks;
	};
}

#endif
