#pragma once

class AudioManager
{
public:
	~AudioManager( );
	static AudioManager* get( )
	{
		static AudioManager instance;
		return &instance;
	}
public:
	void update( );
	FMOD::Sound* loadSound( String&& filename, bool stream );
	FMOD::Channel* playSound( FMOD::Sound* sound, const int loopCount = -1 );
	void updateListener( const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& forward, const glm::vec3& up );
private:
	AudioManager( );
private:
	FMOD::System* _audioSystem;
};