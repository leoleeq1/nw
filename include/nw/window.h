#pragma once
#include "window_desc.h"

#include <cstdint>
#include <memory>

namespace nw
{
    class Window
    {
    public:
        Window();
        ~Window();

        void Create(WindowDesc desc);

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };
}