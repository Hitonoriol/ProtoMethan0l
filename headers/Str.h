struct desclist {
    bool operator()(std::string const& l, std::string const& r) const {
        if (l.size() < r.size())
            return false;
        if (l.size() > r.size())
            return true;
        return l > r;
    };
};

typedef std::map<std::string, std::string, desclist> varlist_t;

template <typename T>
std::string strprec(const T a_value, int n) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

template<class T>
typename std::enable_if<std::is_fundamental<T>::value, std::string>::type str(const T& t, int prc = 6) {
    return strprec(t, prc);
}

template<class T>
typename std::enable_if<!std::is_fundamental<T>::value, std::string>::type str(const T& t) {
    return std::string(t);
}
