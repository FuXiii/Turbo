
#include <string>
#include <vector>
#include <cstdint>

std::string ReadTextFile(const std::string &filename);
std::vector<uint8_t> ReadBinaryFile(const std::string &filename, const uint32_t count = 0);