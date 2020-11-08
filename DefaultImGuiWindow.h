#pragma once
class DefaultImGuiWindow
{
public:
    DefaultImGuiWindow(const char* _title = "title") : title(_title) {
        show = true;
    }
    ~DefaultImGuiWindow();
    virtual void Update() {};

    const bool getShow() {
        return show;
    }
protected:
    bool begin();
    void end();
    bool show;
    const char* title;
};

