//
// Created by vladislav on 24.05.2021.
//

// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_BUILDER_HPP_
#define INCLUDE_BUILDER_HPP_

#include <boost/program_options.hpp>
#include <string>
#include "Attributes.hpp"
class Builder {
 public:
  explicit Builder(boost::program_options::variables_map& mp);
  ~Builder();
  void Start();
  static void TerminateProcess(MyChild& childProcess);
  bool RunProcess(const std::string& attribute, MyChild& myChild);

 private:
  Process* _process;
};

#endif  // INCLUDE_BUILDER_HPP_
