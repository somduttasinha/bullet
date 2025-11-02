#pragma once

#include <cstdio>
#include <cstring>
#include <iostream>
#include <ostream>
#include <unordered_map>
#include <vector>

template <typename T> void printArray(const std::vector<T> &vec) {
  for (int i = 0; i < vec.size(); i++) {
    const auto elem = vec[i];
    std::cout << "Line " << i << ": " << elem << std::endl;
  }
}

template <typename K, typename V>
void printMap(const std::unordered_map<K, V> map) {
  for (auto entry : map) {
    std::cout << entry.first << ": " << entry.second << std::endl;
  }
}
