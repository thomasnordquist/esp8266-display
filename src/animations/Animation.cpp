#include <Arduino.h>
#include "Animation.hpp"
#include "../Logging.hpp"

Animation::Animation(const char* frames, int frameCount, int width, int height, int speed) {
  _frames = frames;
  _frameCount = frameCount;
  _width = width;
  _height = height;
  _currentFrame = 0;
  _speed = speed;
}

bool Animation::hasEnded() {  
  return _currentFrame >= (_frameCount - 1);
}

void Animation::restartAnimation() {
  _currentFrame = 0;
}

const char* Animation::getFrame() {
  if(!this->hasEnded() && millis() - _lastFrameTime > _speed) {
    _currentFrame = (_currentFrame + 1) % (_frameCount);
    _lastFrameTime = millis();
  }

  int charOffset = ceil(_width / 8.0) * _height;
  return _frames + charOffset * _currentFrame;
}
