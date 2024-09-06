#pragma once

namespace Ur::View {
    class FView
    {
    };

    namespace Misc {
        struct FFromViewTag
        {
        };

        // break/continue support for the internal iteration over views
        enum class ELoop
        {
            Break,
            Continue
        };
    }
}