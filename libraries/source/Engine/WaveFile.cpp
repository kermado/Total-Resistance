#include <Engine/WaveFile.hpp>

#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>

namespace Engine
{
	WaveFile::WaveFile()
	: m_channelCount(0)
	, m_sampleRate(0)
	, m_bitDepth(0)
	, m_samples()
	{
		// Nothing to do.
	}

	WaveFile::~WaveFile()
	{
		// Nothing to do.
	}

	unsigned long WaveFile::GetSamplesCount() const
	{
		return m_samples.size();
	}

	void WaveFile::GetSamples(std::int16_t* data, std::size_t dataSize, std::size_t offsetSamples) const
	{
		std::memset(data, 0, sizeof(std::int16_t) * dataSize);
		std::memcpy(data, &m_samples[offsetSamples], sizeof(std::int16_t) * std::min(dataSize, m_samples.size() - offsetSamples));
	}

	unsigned int WaveFile::GetChannelCount() const
	{
		return m_channelCount;
	}

	unsigned int WaveFile::GetSampleRate() const
	{
		return m_sampleRate;
	}

	unsigned int WaveFile::GetBitDepth() const
	{
		return m_bitDepth;
	}

	bool WaveFile::LoadFromFile(std::string filepath)
	{
		std::fstream file(filepath, std::ios::in | std::ios::binary);
		if (!file.is_open())
		{
			std::cerr << "ERROR: Unable to open wave file \"" + filepath + "\"" << std::endl;
			return false;
		}

		while (file.peek() != std::char_traits<char>::eof())
		{
			std::uint32_t chunkID;
			std::uint32_t chunkSize;

			file.read(reinterpret_cast<char*>(&chunkID), sizeof(std::uint32_t));
			file.read(reinterpret_cast<char*>(&chunkSize), sizeof(std::uint32_t));

			switch(chunkID)
			{
				case 'FFIR':
				{
					std::uint32_t format;
					file.read(reinterpret_cast<char*>(&format), sizeof(std::uint32_t));

					if (format != 'EVAW')
					{
						std::cerr << "ERROR: Unable to load wave file \"" + filepath + "\"" << std::endl;
						std::cerr << "Not a valid WAVE file" << std::endl;
						return false;
					}

					break;
				}
				case ' tmf':
				{
					std::uint16_t audioFormat;
					file.read(reinterpret_cast<char*>(&audioFormat), sizeof(std::uint16_t));

					if (audioFormat != 1)
					{
						std::cerr << "ERROR: Unable to load wave file \"" + filepath + "\"" << std::endl;
						std::cerr << "WAVE file was not in PCM format" << std::endl;
						return false;
					}

					std::uint16_t numChannels;
					file.read(reinterpret_cast<char*>(&numChannels), sizeof(std::uint16_t));

					std::uint32_t sampleRate;
					file.read(reinterpret_cast<char*>(&sampleRate), sizeof(std::uint32_t));

					std::uint32_t byteRate;
					file.read(reinterpret_cast<char*>(&byteRate), sizeof(std::uint32_t));

					std::uint16_t blockAlign;
					file.read(reinterpret_cast<char*>(&blockAlign), sizeof(std::uint16_t));

					std::uint16_t bitDepth;
					file.read(reinterpret_cast<char*>(&bitDepth), sizeof(std::uint16_t));

					if (bitDepth % 2 != 0)
					{
						std::cerr << "ERROR: Unable to load wave file \"" + filepath + "\"" << std::endl;
						std::cerr << "Invalid number of bits per sample" << std::endl;
						return false;
					}

					if (byteRate != (sampleRate * numChannels * bitDepth / 8))
					{
						std::cerr << "ERROR: Unable to load wave file \"" + filepath + "\"" << std::endl;
						std::cerr << "Invalid byte rate" << std::endl;
						return false;
					}

					if (blockAlign != (numChannels * bitDepth / 8))
					{
						std::cerr << "ERROR: Unable to load wave file \"" + filepath + "\"" << std::endl;
						std::cerr << "Invalid block align" << std::endl;
						return false;
					}

					m_channelCount = static_cast<unsigned int>(numChannels);
					m_sampleRate = static_cast<unsigned int>(sampleRate);
					m_bitDepth = static_cast<unsigned int>(bitDepth);

					break;
				}
				case 'atad':
				{
					const unsigned int numSamples = chunkSize / sizeof(std::int16_t);
					m_samples.resize(numSamples);
					for (std::size_t i = 0; i < numSamples; ++i)
					{
						std::int16_t sample;
						file.read(reinterpret_cast<char*>(&sample), sizeof(std::int16_t));
						m_samples[i] = sample;
					}

					break;
				}
				default:
				{
					file.ignore(chunkSize);

					break;
				}
			}
		}

		return true;
	}
}
