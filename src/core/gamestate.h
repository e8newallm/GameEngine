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
    static void pause()
    {
        paused = true;
    }

    /**
     * \brief Unpauses the game.
     */
    static void unpause()
    {
        paused = false;
    }

    /**
     * \brief Toggles the pause state of the game.
     */
    static void togglepause()
    {
        paused = !paused;
    }

    /**
     * \brief Checks if the game is paused.
     *
     * \return bool Is the game paused?
     */
    static bool gamePaused()
    {
        return paused;
    }

    /**
     * \brief Closes the game.
     */
    static void closeGame()
    {
        closing = true;
    };

    /**
     * \brief Checks if the game is closing.
     *
     * \return bool Is the game closing?
     */
    static bool gameClosing()
    {
        return closing;
    };

  private:
    static bool paused;
    static bool closing;
};
} // namespace GameEng