class IRsend
{
public:
  IRsend() {}
  void sendRaw(unsigned int buf[], int len, int hz);
private:
  void enableIROut(int khz);
  void mark(int usec);
  void space(int usec);
};

// Some useful constants

#define USECPERTICK 50  // microseconds per clock interrupt tick
#define RAWBUF 100 // Length of raw duration buffer

#endif
