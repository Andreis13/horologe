
#ifndef _HOROLOGE_AUDIO_PLAYER_H
#define _HOROLOGE_AUDIO_PLAYER_H

class AudioPlayer {

public:
  AudioPlayer(uint8_t analog_pin, uint8_t enable_pin);
  void initialize();
  void play(AudioClip clip);
  void stop();
  void update(uint32_t time);
private:

};

#endif // _HOROLOGE_AUDIO_PLAYER_H
