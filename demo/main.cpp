#include <iostream>
#include <boost/process.hpp>

//int main(int argc, char** argv) {
//  std::cout << "Hello world" << std::endl;
//}

int main() {
  std::thread thread([](){
    std::this_thread::sleep_for(std::chrono::seconds(10));
    std::cout << "Wake up" << std::endl;
  });
//  thread.detach();
  std::cout << "BYE!!!" << std::endl;
  thread.join();
}