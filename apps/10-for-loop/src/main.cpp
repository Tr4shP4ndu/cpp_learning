#include <iostream>
#include <array>

int main() {
    int arr[] = {1,3,5};
    std::array<int, 3> arr2{1,3,5};
    for(int i=0; i<arr2.size(); i=i+1){ // i=i+1 can be done this way i++
                                        // Loop runs 3 times by using the arr2.size
      std::cout << arr2[i] << std::endl;
    }
    return 0;
}
