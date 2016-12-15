#include "add_handler.h"

#include "function/help_function.hpp"

std::string AddHandler::run_string(const std::string& task) {
    auto i = std::stoi(task);
    return std::to_string(i + 50);
}

std::unique_ptr<Handler> Handler::create_unique() {
    return wzp::make_unique<AddHandler>();
}

std::shared_ptr<Handler> Handler::create_shared() {
    return std::make_shared<AddHandler>();
}
