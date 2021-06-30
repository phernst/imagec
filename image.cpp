#include "image.h"

Image::Image(torch::Tensor data, std::unordered_map<std::string, std::string> meta)
    : data {data}
    , meta {meta}
{

}
