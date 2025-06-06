#ifndef KEYSTATE_H
#define KEYSTATE_H

#include <SDL3/SDL.h>

#include <array>

namespace GameEng
{
/**
 * \brief A static class for keeping track of the keyboard state.
 *
 */
class KeyState
{
  public:
    /**
     * \brief Peeps and processes all SDL keyboard events.
     * Should be used at the start of each frame.
     */
    static void update();

    /**
     * \brief Updates a key to be a given state.
     * Only SDL_EVENT_KEY_UP or SDL_EVENT_KEY_DOWN are acceptable.
     * \param key The key being updated.
     * \param keyEvent The keyboard event for the given key.
     */
    static void updateKey(SDL_Scancode key, SDL_EventType keyEvent);

    /**
     * \brief Gets the current state of the given key.
     *
     * \param key The key being checked.
     * \return SDL_EventType The state the key is currently in.
     */
    static SDL_EventType key(SDL_Scancode key);

    /**
     * \brief Gets whether the key has been pressed.
     *
     * \param key The key being checked.
     * \return bool Has this key been pressed?
     */
    static bool keyPressed(SDL_Scancode key);

    /**
     * \brief Gets whether the key has been released.
     *
     * \param key The key being checked.
     * \return bool Has this key been released?
     */
    static bool keyReleased(SDL_Scancode key);

    KeyState() = delete;
    KeyState(const KeyState&) = delete;
    KeyState& operator=(const KeyState&) = delete;

  private:
    static std::array<SDL_EventType, SDL_SCANCODE_COUNT> keyStateInit();

    static inline std::array<SDL_EventType, SDL_SCANCODE_COUNT> keys = KeyState::keyStateInit();
    static inline std::array<SDL_EventType, SDL_SCANCODE_COUNT> keysPrev = KeyState::keyStateInit();
};
} // namespace GameEng

#endif