class MP3
{
    private:
        uint8_t id;
    public:
        MP3(const uint8_t id);
        bool play();
        bool pause();
        bool volumeUp();
        bool volumeDown();
        uint8_t getVolume();
}