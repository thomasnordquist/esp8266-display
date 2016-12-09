#include <Arduino.h>
#include "Animation.hpp"
#include "../Logging.hpp"

Animation::Animation(const char* frames[], int frameCount, int width, int height, int speed) {
  _frames = frames;
  _frameCount = frameCount;
  _width = width;
  _height = height;
  _currentFrame = 0;
  _speed = speed;
}

const char* Animation::getFrame() {
  if(millis() - _lastFrameTime > _speed) {
    _currentFrame = (_currentFrame + 1) % (_frameCount - 1);
    _lastFrameTime = millis();
  }
  return _frames[_currentFrame];
}
