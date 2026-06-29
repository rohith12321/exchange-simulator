#pragma once

#include <cstddef>

enum class Symbol
{
    AAPL,
    GOOG,
    MSFT,
    NVDA,
    TSLA
};

constexpr std::size_t toIndex(Symbol symbol)
{
    return static_cast<std::size_t>(symbol);
}