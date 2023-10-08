#ifndef ANIMATION_H
#define ANIMATION_H


class TimeInterpolator {
  public:
    virtual float interpolate (float t) = 0;
};

class LinearInterpolator : public TimeInterpolator {
  public:
    float interpolate (float t) {
      return t;
    }
};

// class EaseInOutInterpolator : public TimeInterpolator {
//   public:
//     float interpolate (float t) {
//       return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t;
//     }
// };

class EaseInOutCubicInterpolator : public TimeInterpolator {
public:
    float interpolate(float t) {
        if (t < 0.5) {
            return 4.0 * t * t * t;
        } else {
            float f = 2.0 * t - 2.0;
            return 0.5 * f * f * f + 1.0;
        }
    }
};

class Animation {
  public:
    float duration;
    float delay;
    float time;
    bool loop;
    bool running;
    TimeInterpolator * interpolator;

    Animation (float duration, float delay, bool loop, TimeInterpolator * interpolator) {
      this->duration = duration;
      this->delay = delay;
      this->loop = loop;
      this->interpolator = interpolator;
      this->time = 0;
      this->running = false;
    }

    void start () {
      this->running = true;
    }

    void stop () {
      this->running = false;
    }

    void update (float dt) {

      if (this->running) {
        this->time += dt;

        if (this->time >= this->duration) {
          if (this->loop) {
            this->time = 0;
          } else {
            this->time = this->duration;
            this->running = false;
          }
        }

      }
    }

    float getProgress () {
      float progress = this->time / this->duration;
      
      if (progress > 1) {
        progress = 1;
      }
      return this->interpolator->interpolate(progress);
    }

    bool isFinished () {
      return this->time >= this->duration;
    }
};



#endif // ANIMATION_H