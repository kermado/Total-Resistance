#ifndef IAUDIOSOURCE_H
#define IAUDIOSOURCE_H

#include <string>
#include <array>

namespace Engine
{
	class IAudioSource
	{
	public:
		/**
		 * Constructor.
		 */
		IAudioSource();

		/**
		 * Destructor.
		 */
		~IAudioSource();

		/**
		 * Returns the total number of samples that compose the the audio
		 * source.
		 *
		 * @return Total number of samples.
		 */
		virtual unsigned long GetSamplesCount() const = 0;

		/**
		 * Fills the specified data array with samples from the audio source,
		 * starting from the specified sample offset.
		 *
		 * @param data Array to populate with samples.
		 * @param dataSize Length of the data array to be populated.
		 * @param offsetSamples Offset (in samples) to start copying the audio
		 * source's samples from.
		 */
		virtual void GetSamples(std::int16_t* data, std::size_t dataSize, std::size_t offsetSamples) const = 0;

		/**
		 * Returns the number of channels used by the audio source.
		 *
		 * @return Number of channels.
		 */
		virtual unsigned int GetChannelCount() const = 0;

		/**
		 * Returns the sample rate of the audio source.
		 *
		 * @return Samples per second.
		 */
		virtual unsigned int GetSampleRate() const = 0;

		/**
		 * Returns the bit depth of the audio source.
		 *
		 * @return Bits per sample.
		 */
		virtual unsigned int GetBitDepth() const = 0;
	};
}

#endif
