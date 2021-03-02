#pragma once

namespace infofile
{
    bool IsNumber(char c);

    bool IsAlpha(char c);

    bool IsIdentChar(char c, bool first);

    bool IsWhitespace(char c);

    bool IsHex(char c);

    bool IsBinary(char c);
}
