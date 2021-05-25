// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_ATTRIBUTES_H_
#define INCLUDE_ATTRIBUTES_H_

#include <atomic>
#include <string>
#include <boost/process.hpp>
#include <boost/log/trivial.hpp>
struct MyChild {
  MyChild(bool tmp, boost::process::child&& other_child) {
    terminated.store(tmp);
    current_child = std::move(other_child);
  }

  void set_terminted(bool tmp) { terminated.store(tmp); }
  void set_child(boost::process::child&& other_child) {
    current_child = std::move(other_child);
  }

  std::atomic_bool terminated{};
  boost::process::child current_child;
};

class Process {
 public:
  const std::string BUILD_TARGET = "_builds";
  const std::string INSTALL_TARGET = "_install";

  explicit Process(std::string& config, bool install, bool package, size_t time)
      : _config(config), _install(install), _package(package), _time(time){};
  ~Process() = default;

  std::string get_config() { return _config; }
  [[nodiscard]] bool get_install() const { return _install; }
  [[nodiscard]] bool get_package() const { return _package; }
  [[nodiscard]] size_t get_time() const { return _time; }

  std::string get_commandline(const std::string& attribute) {
    if (attribute == "config") {
      return (" -B" + BUILD_TARGET + " -DCMAKE_INSTALL_PREFIX=" +
              INSTALL_TARGET + " -DCMAKE_BUILD_TYPE=" + attribute);
    } else {
      return ("--build " + BUILD_TARGET +
                          attribute == "build" ? "" : (" --target " + attribute));
    }
  }

 private:
  std::string _config;
  bool _install;
  bool _package;
  size_t _time;
};

#endif  // INCLUDE_ATTRIBUTES_H_
