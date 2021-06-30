#pragma once

#include <unordered_map>
#include <string>
#undef slots
#include <torch/torch.h>
#define slots Q_SLOTS

class Image
{
public:
    Image(torch::Tensor, std::unordered_map<std::string, std::string>);

    static Image random();

private:
    torch::Tensor data;
    std::unordered_map<std::string, std::string> meta;
};
