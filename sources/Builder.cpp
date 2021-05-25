//
// Created by vladislav on 24.05.2021.
//
// Copyright 2020 Your Name <your_email>

#include "Builder.hpp"
Builder::~Builder() { delete _process; }

Builder::Builder(boost::program_options::variables_map& mp) {
  bool install = false, package = false;
  size_t time = 0;
  std::string config{"Debug"};

  if (mp.count("config")) {
    config = mp["config"].as<std::string>();
  }
  if (mp.count("install")) {
    install = true;
  }
  if (mp.count("package")) {
    package = true;
  }
  if (mp.count("timeout")) {
    time = mp["timeout"].as<size_t>();
  }
  _process = new Process(config, install, package, time);
}

void Builder::Start() {
  MyChild myChild{false, boost::process::child()};

  if (_process->get_time() != 0) {
    std::thread timerThread([this, &myChild] {
      std::this_thread::sleep_for(std::chrono::seconds(_process->get_time()));
      TerminateProcess(myChild);
    });
    timerThread.detach();
  }

  try {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::shared_future<bool> continueProc =
        std::async([this, &myChild]() -> bool {
          return this->RunProcess("config", myChild);
        });

    continueProc.wait();
    if (continueProc.get()) {
      continueProc = std::async([this, &myChild]() -> bool {
        return this->RunProcess("build", myChild);
      });
    }

    continueProc.wait();
    if (continueProc.get() && _process->get_install()) {
      continueProc = std::async([this, &myChild]() -> bool {
        return this->RunProcess("install", myChild);
      });
    }

    continueProc.wait();
    if (continueProc.get() && _process->get_package()) {
      continueProc = std::async([this, &myChild]() -> bool {
        return this->RunProcess("package", myChild);
      });
    }
    continueProc.wait();

  } catch (std::exception& e) {
    BOOST_LOG_TRIVIAL(error) << "Error in processing: " << e.what();
  }
}

void Builder::TerminateProcess(MyChild& childProcess) {
  BOOST_LOG_TRIVIAL(debug) << "Timeout, stopping all child processes...";
  try {
    if (childProcess.current_child.running()) {
      childProcess.current_child.terminate();
    }
    childProcess.set_terminted(true);
    BOOST_LOG_TRIVIAL(debug)
        << R"(childProcess's "terminated" set:)" << childProcess.terminated;
  } catch (std::exception& exception) {
    BOOST_LOG_TRIVIAL(fatal)
        << "Terminating method fall with exception: " << exception.what();
  }
}
bool Builder::RunProcess(const std::string& attribute, MyChild& myChild) {
  if (myChild.terminated) {
    return false;
  }
  boost::process::ipstream stream;
  auto cmakePath = boost::process::search_path("cmake");
  BOOST_LOG_TRIVIAL(info) << "Found cmake: " << cmakePath.string();
  boost::process::child child(std::string(cmakePath.string() + " " +
                                          _process->get_commandline(attribute)),
                              boost::process::std_out > stream);
  myChild.set_terminted(false);
  myChild.set_child(std::move(child));

  myChild.current_child.wait();
  auto ec = myChild.current_child.exit_code();

  if (ec != 0) {
    BOOST_LOG_TRIVIAL(error) << "Non zero exit code. Exiting...";
    myChild.set_terminted(true);
    return false;
  } else {
    return true;
  }
}
