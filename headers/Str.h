template<class T>
 typename std::enable_if<std::is_fundamental<T>::value, std::string>::type str(const T& t)
  {
    return std::to_string(t);
  }

template<class T>
  typename std::enable_if<!std::is_fundamental<T>::value, std::string>::type str(const T& t)
  {
    return std::string(t);
  }
