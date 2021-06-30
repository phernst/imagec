#include "image.h"

#include <torch/torch.h>

Image::Image(torch::Tensor data, std::unordered_map<std::string, std::string> meta)
    : data {data}
    , meta {meta}
{

}

Image Image::random()
{
    return {torch::rand({256, 256}), {}};
}
