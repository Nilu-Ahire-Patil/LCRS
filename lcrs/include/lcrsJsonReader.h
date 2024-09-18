#ifndef LCRS_JSON_READER_H
#define LCRS_JSON_READER_H

LCRS_NAMESPACE_START

class lcrsJsonReader {
public:
    lcrsJsonReader();
    ~lcrsJsonReader();

private:
    static const std::string mFilePath;
}

LCRS_NAMESPACE_END

#endif