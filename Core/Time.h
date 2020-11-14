#pragma once
class Time
{
    public:
        Time(float mTime=0.0f): time(mTime){

        }

        operator float() const  { return time; } //helper for casting to float 
        float GetSeconds() const { return time / 1000.0f; }
        float GetMiliseconds() const { return time; }


    private:
        float time;
};

