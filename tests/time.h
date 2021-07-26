#ifndef time_h
#define time_h

#include <chrono>

typedef std::chrono::time_point<std::chrono::high_resolution_clock> high_res_clock;


class Timer
{
private:
  high_res_clock m_start = std::chrono::high_resolution_clock::now();
  float m_frequency;
  
public:
  Timer() {}
  
  void reset() {
    m_start = std::chrono::high_resolution_clock::now();
  }
  
  float elapsed() {
    std::chrono::duration<float> diff = std::chrono::high_resolution_clock::now() - m_start;
    m_frequency = diff.count();
    return m_frequency;
  }
};


#endif /* time_h */
