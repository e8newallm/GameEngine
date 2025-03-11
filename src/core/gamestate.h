namespace GameEng
{
    class GameState
    {
        public:
            /**
            * \brief Pauses the game
            */
            inline static void pause() { paused = true; }

            /**
            * \brief Unpauses the game
            */
            inline static void unpause() { paused = false; }

            /**
            * \brief Toggles the pause state of the game
            */
            inline static void togglepause() { paused = !paused; }

            /**
            * \brief Checks if the game is paused
            *
            * \return true Game is paused
            * \return false Game is not paused
            */
            inline static bool gamePaused() { return paused; }

            /**
            * \brief Closes the game
            */
            inline static void closeGame() { closing = true; };

            /**
            * \brief Checks if the game is closing
            *
            * \return true Game is closing
            * \return false Game is not closing
            */
            inline static bool gameClosing() { return closing; };

        private:
            static bool paused;
            static bool closing;
    };
}