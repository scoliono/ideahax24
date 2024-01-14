#ifndef DISP_HPP
#define DISP_HPP

class Display {
public:
    Display();
    ~Display();
    void loop();
private:
    void stateChange();
};

#endif
