
#ifndef _HOROLOGE_AUDIO_CLIP_H
#define _HOROLOGE_AUDIO_CLIP_H

class AudioClip {

public:
  AudioClip(uint8_t analog_pin, uint8_t enable_pin);
  void play(AudioClip clip);
  void stop();
  void update(uint32_t time);
private:

};

#endif // _HOROLOGE_AUDIO_CLIP_H
