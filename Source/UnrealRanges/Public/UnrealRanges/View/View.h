#pragma once

namespace Ur {
    namespace View {
        class FView
        {
        };
    }
    namespace Misc {
        struct FFromViewTag
        {
        };
    }
    // break/continue support for the internal iteration over views
    enum class ELoop
    {
        Break,
        Continue
    };
}