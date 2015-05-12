#ifdef DEBUG
#  define LOG(x) std::cerr << "[LOG] " << x << std::endl;
#else
#  define LOG(x)
#endif // DEBUG