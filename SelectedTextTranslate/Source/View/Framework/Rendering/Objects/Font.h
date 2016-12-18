#pragma once

class Font
{
private:
    HFONT handle;

public:
    Font(HFONT handle);
    ~Font();

    HFONT GetHandle() const;
};