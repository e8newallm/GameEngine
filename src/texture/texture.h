#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>

#include "datastore.h"
#include "graphics.h"
#include "texture_base.h"

namespace GameEng
{
/**
 * \brief An abstraction for SDL_GPUTexture* that contains texture width and height, as well as enable DataStore to
 * deconstruct it.
 *
 */
class GPUTexture
{
  public:
    /**
     * \brief Construct a new GPUTexture object.
     *
     * \param tex The texture being used.
     * \param width The width of the texture.
     * \param height The height of the texture.
     */
    GPUTexture(SDL_GPUTexture* tex, int width, int height) : tex(tex), width(width), height(height)
    {
    }

    /**
     * \brief Destroy the GPUTexture wrapper as well as the SDL_GPUTexture
     *
     */
    ~GPUTexture()
    {
        if(tex != nullptr)
        {
            SDL_ReleaseGPUTexture(GPUDevice::getGPU(), tex);
        }
    }

    /**
     * \brief Get the SDL_GPUTexture object.
     *
     * \return SDL_GPUTexture* The texture.
     */
    [[nodiscard]] SDL_GPUTexture* getTex()
    {
        return tex;
    };

    /**
     * \brief Get the texture's width.
     *
     * \return int The width of the texture.
     */
    [[nodiscard]] int getWidth() const
    {
        return width;
    };

    /**
     * \brief Get the texture's height.
     *
     * \return int The height of the texture.
     */
    [[nodiscard]] int getHeight() const
    {
        return height;
    };

    GPUTexture(const GPUTexture&) = delete;

  private:
    SDL_GPUTexture* tex; //< The texture that is loaded into memory.
    int width;           //< The width of the texture.
    int height;          //< The height of the texture.
};

/**
 * \brief A Texture_base derived class for drawing a simple texture.
 *
 */
class Texture : public Texture_base, public DataStore<GPUTexture, Texture>
{
  public:
    /**
     * \brief Construct an empty texture.
     *
     */
    Texture();

    /**
     * \brief Construct a new Texture object that uses an GPUTexture in the DataStore.
     *
     * \param name The name of the GPUTexture in the DataStore.
     */
    explicit Texture(const std::string& name);

    /**
     * \brief Inherited function that is not required for this class.
     * May be required for a derived class.
     * \param deltaT How much time has passed since the last update (in milliseconds).
     */
    void update(double deltaT) override
    {
        (void)deltaT;
    };

    /**
     * \brief Draws the texture to the world.
     *
     * \param world The world that is being drawn to.
     * \param buffer The buffer that contains the shader data.
     * \param renderPass The render pass for the current draw.
     */
    void draw(World* world, SDL_GPUBuffer* buffer, SDL_GPURenderPass* renderPass) override;

  protected:
    std::shared_ptr<GPUTexture> texture;
    SDL_FRect texturePosition;
};

/**
 * \brief Creates a new texture and uploaded it to the GPU.
 *
 * \param gpu The GPUDevice to upload the texture with.
 * \param surf The texture converted into a SDL_Surface to upload.
 * \param filename The filename of the texture being uploaded.
 * \return std::shared_ptr<GPUTexture> A pointer to the texture in memory.
 */
std::shared_ptr<GPUTexture> createTexture(SDL_GPUDevice* gpu, SDL_Surface* surf, const std::string& filename);
} // namespace GameEng

#endif
