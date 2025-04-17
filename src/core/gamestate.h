namespace GameEng
{
    /**
     * \brief Keeps track of the current game state.
     */
    class GameState
    {
        public:
            /**
            * \brief Pauses the game.
            */
            inline static void pause() { paused = true; }

            /**
            * \brief Unpauses the game.
            */
            inline static void unpause() { paused = false; }

            /**
            * \brief Toggles the pause state of the game.
            */
            inline static void togglepause() { paused = !paused; }

            /**
            * \brief Checks if the game is paused.
            *
            * \return bool Is the game paused?
            */
            inline static bool gamePaused() { return paused; }

            /**
            * \brief Closes the game.
            */
            inline static void closeGame() { closing = true; };

            /**
            * \brief Checks if the game is closing.
            *
            * \return bool Is the game closing?
            */
            inline static bool gameClosing() { return closing; };

        private:
            static bool paused;
            static bool closing;
    };
}