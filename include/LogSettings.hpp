// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_LOGSETTINGS_HPP_
#define INCLUDE_LOGSETTINGS_HPP_

#include <string>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>


[[maybe_unused]]void Init_logger(const std::string& sev_level);

#endif  // INCLUDE_LOGSETTINGS_HPP_
