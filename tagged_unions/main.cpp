#include "Message.h"

#include <iostream>
#include <vector>
#include <cstdlib>

using namespace my_messages;

void ProcessData(const Payload1& o) {
    std::cout << "{a=" << o.a << ", b=" << o.b << "}" << std::endl;
}

void ProcessData(const Payload2& o) {
    std::cout << "{a=" << o.a << ", b=" << o.b << ", c=" << o.c << "}" << std::endl;
}

void ProcessMessage(const Message& msg) {
    switch (msg.GetType()) {
        case Message::Type::Type1:
            ProcessData(msg.GetData<Message::Type::Type1>());
            break;
        case Message::Type::Type2:
            ProcessData(msg.GetData<Message::Type::Type2>());
            break;
        default:
            throw std::runtime_error("message type not handled");
            break;
    };
}

std::vector<Message> ProduceMessages(size_t n) {
    std::vector<Message> ret;
    ret.reserve(n);
    for (int i = 0; i < n; ++i) {
        auto rnd = rand() % 100;
        if (rnd < 50) {
            ret.push_back(Message(Payload1{i, (double)rnd}));
        }
        else {
            ret.push_back(Message(Payload2{(double)rnd, i, std::to_string(rnd*i)}));
        }
    }
    return ret;
}

void ConsumeMessages(const std::vector<Message>& vec) {
    for (const auto& msg : vec) {
        ProcessMessage(msg);
    }
}

int main() {
    auto messages = ProduceMessages(100);
    ConsumeMessages(messages);
}
