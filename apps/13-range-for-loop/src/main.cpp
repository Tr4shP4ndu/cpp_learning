#include <iostream>
#include <array>

int main() {
    int arr[] = {1,3,5};
    std::array<int, 3> arr2{1,3,5};
    for(int element: arr2){ // simply change int to auto& to auto detect the primitive
      std::cout << element << std::endl;
    }
    return 0;
}
