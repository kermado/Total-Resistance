#ifndef WAVEFILE_H
#define WAVEFILE_H

#include <string>
#include <vector>

#include <Engine/IAudioSource.hpp>

namespace Engine
{
	class WaveFile : public IAudioSource
	{
	public:
		/**
		 * Constructor.
		 */
		WaveFile();

		/**
		 * Destructor.
		 */
		~WaveFile();

		/**
		 * Returns the total number of samples that compose the the audio
		 * source.
		 *
		 * @return Total number of samples.
		 */
		virtual unsigned long GetSamplesCount() const;

		/**
		 * Fills the specified data array with samples from the audio source,
		 * starting from the specified sample offset.
		 *
		 * @param data Array to populate with samples.
		 * @param dataSize Length of the data array to be populated.
		 * @param offsetSamples Offset (in samples) to start copying the audio
		 * source's samples from.
		 */
		void GetSamples(std::int16_t* data, std::size_t dataSize, std::size_t offsetSamples) const;

		/**
		 * Returns the number of channels used by the audio source.
		 *
		 * @return Number of channels.
		 */
		unsigned int GetChannelCount() const;

		/**
		 * Returns the sample rate of the audio source.
		 *
		 * @return Samples per second.
		 */
		unsigned int GetSampleRate() const;

		/**
		 * Returns the bit depth of the audio source.
		 *
		 * @return Bits per sample.
		 */
		unsigned int GetBitDepth() const;

		/**
		 * Loads the audio source from the specified file.
		 *
		 * @param filepath Path to the audio file.
		 * @return True if the audio source was loaded successfully.
		 */
		bool LoadFromFile(std::string filepath);

	private:
		/**
		 * Number of channels used.
		 */
		unsigned int m_channelCount;

		/**
		 * Number of samples per second.
		 */
		unsigned int m_sampleRate;

		/**
		 * The number of bits of information in each sample.
		 */
		unsigned int m_bitDepth;

		/**
		 * The samples that were read from the file.
		 */
		std::vector<std::int16_t> m_samples;
	};
}

#endif
