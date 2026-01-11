#include <iostream>
#include <numeric>
#include <iterator>
#include <array>

int main(){
  std::array<int, 100> ids;
  std::iota(std::begin(ids), std::end(ids), 0);
  // ids.at(10000) = 9; // this is would gives us out of range error
  ids.at(99) = 9; // all values from 0 to 108 plus 9
  for(int i=0; i < 100; i++){
    std::cout << ids[i] << std::endl;
  }
  return 0;
}
