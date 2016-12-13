
class Animation {
public:
  Animation(const char* frames, int frameCount, int width, int height, int speed);
  const char* getFrame();
  bool hasEnded();
  void restartAnimation();

  const char* _frames;
  int _frameCount;
  int _width;
  int _height;
  int _speed;

private:
  int _currentFrame;
  unsigned long _lastFrameTime;
};
