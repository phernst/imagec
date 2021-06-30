#pragma once

#include <unordered_map>
#include <string>
#include <torch/torch.h>

class Image
{
public:
    Image(torch::Tensor, std::unordered_map<std::string, std::string>);

private:
    torch::Tensor data;
    std::unordered_map<std::string, std::string> meta;
};
