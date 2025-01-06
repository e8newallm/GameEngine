class GameState
{
    public:

        inline static void pause() { paused = true; }
        inline static void unpause() { paused = false; }
        inline static void togglepause() { paused = !paused; }
        inline static bool gamePaused() { return paused; }

        inline static void closeGame() { closing = true; };
        inline static bool gameClosing() { return closing; };

    private:
        inline static bool paused = false;
        inline static bool closing = false;
};