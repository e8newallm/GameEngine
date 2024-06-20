#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_thread.h>

#include <rapidjson/document.h>
#include <rapidjson/schema.h>
#include <rapidjson/stringbuffer.h>

#include <iostream>
#include <ctime>
#include <source_location>
#include <fstream>
#include <sstream>

#include "object.h"
#include "physicsobject.h"
#include "context.h"

#include "texture.h"
#include "spritemap.h"
#include "spritemapdata.h"

#include "mousestate.h"
#include "keystate.h"

#include "Catch2/src/catch2/catch_all.hpp"

extern const char* SpriteMapSchema;

TEST_CASE("Spritemap parse testing", "[basic]")
{
    REQUIRE(SDL_Init(SDL_INIT_EVERYTHING) == 0);
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* rend = SDL_CreateRenderer(nullptr, -1, render_flags);

    SECTION("Schema sanity check")
    {
        const char* spriteMapJSON = R"(
            {
                "Textures": "spritemap.png",
                "Sprites": [
                    {
                        "name": "sprite01",
                        "texture": "spritemap.png",
                        "x": 0,
                        "y": 0,
                        "height": 150,
                        "width": 150
                    }
                ],
                "Animations": [
                    {
                        "name": "explosion",
                        "frames": [
                            "sprite01"
                        ],
                        "FPS": 5.0
                    }
                ]
            }
        )";

        rapidjson::Document d;
        d.Parse(spriteMapJSON);
        REQUIRE(!d.HasParseError());

        rapidjson::Document schema;
        schema.Parse(SpriteMapSchema);
        REQUIRE(!schema.HasParseError());
        rapidjson::SchemaDocument schemaDoc(schema);
        rapidjson::SchemaValidator validator(schemaDoc);
        REQUIRE(d.Accept(validator));
    }

    SECTION("JSON load/save sanity check")
    {
        SpriteMapData test, testTwo;
        test.loadFromFile(rend, "json/spritemap/spritemap.json");
        std::string result = R"TEST({"Textures":["tex/spritemap.png"],"Sprites":[{"name":"sprite01","texture":"tex/spritemap.png","x":0,"y":0,"width":150,"height":150}],"Animations":[{"name":"explosion","FPS":5.0,"frames":["sprite01"]}]})TEST";
        REQUIRE(test.serialise() == result);
        REQUIRE_NOTHROW(testTwo.loadFromString(rend, result.c_str()));
    }

    SECTION("SpriteMap schema test")
    {
        REQUIRE_NOTHROW(SpriteMap(rend, "json/spritemap/spritemap.json"));

        REQUIRE_THROWS_WITH(SpriteMap(rend, "UNKNOWN FILE"), 
                            "Could not open Spritemap JSON \"UNKNOWN FILE\"");

        REQUIRE_THROWS_WITH(SpriteMap(rend, "json/invalidjson.json"), 
                            "\"json/invalidjson.json\" is invalid JSON");

        REQUIRE_THROWS_WITH(SpriteMap(rend, "json/spritemap/badspritemap.json"), 
                            "\"json/spritemap/badspritemap.json\" has failed to pass SpriteMap schema");

        REQUIRE_THROWS_WITH(SpriteMap(rend, "json/spritemap/texturenotfound.json"), 
                            "\"json/spritemap/texturenotfound.json\" could not load texture file \"texspritemap.png\"");

        REQUIRE_THROWS_WITH(SpriteMap(rend, "json/spritemap/mismatchedsprite.json"), 
                            "\"json/spritemap/mismatchedsprite.json\" has a sprite (sprite01) referencing a texture not named in the JSON");
        
        REQUIRE_THROWS_WITH(SpriteMap(rend, "json/spritemap/mismatchedanimation.json"), 
                            "\"json/spritemap/mismatchedanimation.json\" has a animation (explosion) referencing a sprite not named in the JSON");
        
        REQUIRE_THROWS_WITH(SpriteMap(rend, "json/spritemap/duplicatedsprite.json"), 
                            "\"json/spritemap/duplicatedsprite.json\" has two sprites named (sprite01)");

        REQUIRE_THROWS_WITH(SpriteMap(rend, "json/spritemap/duplicatedanimation.json"), 
                            "\"json/spritemap/duplicatedanimation.json\" has two animations named (explosion)");
    }
}

TEST_CASE("KeyState testing", "[basic]")
{
    KeyState& keyState = KeyState::get();
    keyState.update(SDL_SCANCODE_1, SDL_KEYDOWN);
    keyState.update(SDL_SCANCODE_2, SDL_KEYDOWN);
    keyState.update(SDL_SCANCODE_5, SDL_KEYDOWN);
    keyState.update(SDL_SCANCODE_A, SDL_KEYDOWN);

    REQUIRE(keyState[SDL_SCANCODE_1] == SDL_KEYDOWN);
    REQUIRE(keyState[SDL_SCANCODE_2] == SDL_KEYDOWN);
    REQUIRE(keyState[SDL_SCANCODE_5] == SDL_KEYDOWN);
    REQUIRE(keyState[SDL_SCANCODE_A] == SDL_KEYDOWN);

    keyState.update(SDL_SCANCODE_5, SDL_KEYUP);

    REQUIRE(keyState[SDL_SCANCODE_1] == SDL_KEYDOWN);
    REQUIRE(keyState[SDL_SCANCODE_2] == SDL_KEYDOWN);
    REQUIRE(keyState[SDL_SCANCODE_5] == SDL_KEYUP);
    REQUIRE(keyState[SDL_SCANCODE_A] == SDL_KEYDOWN);
}

TEST_CASE("MouseState testing", "[basic]")
{
    MouseState& mouseState = MouseState::get();

    mouseState.updateButton({
        SDL_MOUSEBUTTONDOWN,
        0,
        0,
        0,
        SDL_BUTTON_LEFT,
        SDL_PRESSED,
        1,
        0,
        123,
        456
    });

    mouseState.updateMove({
        SDL_MOUSEMOTION,
        0,
        0,
        0,
        0,
        200,
        400,
        20,
        40
    });

    mouseState.updateWheel({
        SDL_MOUSEWHEEL,
        0,
        0,
        0,
        5,
        10,
        SDL_MOUSEWHEEL_FLIPPED,
        5.1,
        10.2,
        0,
        0
    });

    REQUIRE(mouseState.clicked(SDL_BUTTON_LEFT) == true);
    REQUIRE(mouseState.doubleClicked(SDL_BUTTON_LEFT) == false);
    REQUIRE(mouseState.clickPosition(SDL_BUTTON_LEFT).x == 123);
    REQUIRE(mouseState.clickPosition(SDL_BUTTON_LEFT).y == 456);
    REQUIRE(mouseState.buttonDown(SDL_BUTTON_LEFT) == true);

    REQUIRE(mouseState.mousePos().x == 200);
    REQUIRE(mouseState.mousePos().y == 400);
    REQUIRE(mouseState.mouseDelta().x == 20);
    REQUIRE(mouseState.mouseDelta().y == 40);

    REQUIRE(mouseState.scrollDelta() == -10);

    mouseState.reset();

    mouseState.updateButton({
        SDL_MOUSEBUTTONUP,
        0,
        0,
        0,
        SDL_BUTTON_LEFT,
        SDL_RELEASED,
        0,
        0,
        123,
        456
    });

    REQUIRE(mouseState.clicked(SDL_BUTTON_LEFT) == false);
    REQUIRE(mouseState.doubleClicked(SDL_BUTTON_LEFT) == false);
    REQUIRE(mouseState.clickPosition(SDL_BUTTON_LEFT).x == 123);
    REQUIRE(mouseState.clickPosition(SDL_BUTTON_LEFT).y == 456);
    REQUIRE(mouseState.buttonDown(SDL_BUTTON_LEFT) == false);
}

void testRect(SDL_Rect first, SDL_Rect second, const std::source_location location = std::source_location::current())
{
    std::string line = location.file_name();
    line += ":";
    line += std::to_string(location.line());
    CAPTURE(line);
    REQUIRE(first.x == second.x);
    REQUIRE(first.y == second.y);
    REQUIRE(first.h == second.h);
    REQUIRE(first.w == second.w);
}

TEST_CASE("Basic functionality", "[physics]")
{
    REQUIRE(SDL_Init(SDL_INIT_EVERYTHING) == 0);
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* rend = SDL_CreateRenderer(nullptr, -1, render_flags);
    //KeyState& keyState = KeyState::get();
    //MouseState& mouseState = MouseState::get();

    View viewport( {1000, 1000}, {0, 0});
    viewport.setZoom(1.0);

    Context state(rend, &viewport);

    PhysicsContext* phyContext = state.getPhysicsContext();

    SECTION("Box drop")
    {
        PhysicsObject* box = new PhysicsObject({500, 500, 10, 10}, PHYOBJ_COLLIDE, new Texture(rend, nullptr));
        phyContext->addPhyObj(box);

        box->velocityDelta(0.0, 0.5);
        box->preUpdate();
        box->update(10.0, phyContext);
        REQUIRE(box->getBody()->x == 500);
        REQUIRE(box->getBody()->y == 505);
        box->preUpdate();
        box->update(2.0, phyContext);
        REQUIRE(box->getBody()->x == 500);
        REQUIRE(box->getBody()->y == 506);

        box->preUpdate();
        box->update(1.0, phyContext);
        REQUIRE(box->getBody()->x == 500);
        REQUIRE(box->getBody()->y == 506);
        box->preUpdate();
        box->update(1.0, phyContext);
        REQUIRE(box->getBody()->x == 500);
        REQUIRE(box->getBody()->y == 507);

        box->velocityDelta(15.0, 0.0);
        box->preUpdate();
        box->update(4.0, phyContext);
        REQUIRE(box->getBody()->x == 559);
        REQUIRE(box->getBody()->y == 509);

        box->moveDelta(10.0, 10.0);
        box->preUpdate();
        box->update(45.0, phyContext);
        REQUIRE(box->getBody()->x == 1244);
        REQUIRE(box->getBody()->y == 541);
    }

    SECTION("Collision")
    {
        phyContext->setGravity(0.0);
        PhysicsObject* box = new PhysicsObject({500, 500, 10, 10}, PHYOBJ_COLLIDE, new Texture(rend, nullptr));
        phyContext->addPhyObj(box);
        PhysicsObject* boxCollide = new PhysicsObject({520, 500, 10, 10}, PHYOBJ_COLLIDE, new Texture(rend, nullptr));
        phyContext->addPhyObj(boxCollide);

        box->velocityDelta(1.0, 0.0);
        box->preUpdate();
        box->update(5.0, phyContext);
        boxCollide->preUpdate();
        boxCollide->update(5.0, phyContext);
        REQUIRE(box->getBody()->x == 505);
        REQUIRE(box->getBody()->y == 500);
        REQUIRE(box->getVelocity().y == 0.0);
        REQUIRE(box->getVelocity().x == 1.0);
        REQUIRE(boxCollide->getBody()->x == 520);
        REQUIRE(boxCollide->getBody()->y == 500);
        
        box->preUpdate();
        box->update(6.0, phyContext);
        boxCollide->preUpdate();
        boxCollide->update(6.0, phyContext);
        REQUIRE(box->getBody()->x == 510);
        REQUIRE(box->getBody()->y == 500);
        REQUIRE(box->getVelocity().y == 0.0);
        REQUIRE(box->getVelocity().x == 0.0);
        REQUIRE(boxCollide->getBody()->x == 520);
        REQUIRE(boxCollide->getBody()->y == 500);
    }

    SECTION("Correct interpolation and viewpoint adjustment for drawing")
    {
        phyContext->setGravity(0.0);
        PhysicsObject* box = new PhysicsObject({500, 500, 50, 50}, PHYOBJ_COLLIDE, new Texture(rend, nullptr));
        phyContext->addPhyObj(box);

        testRect(box->getInterBody(0), {500, 500, 50, 50});
        testRect(box->getInterBody(0.5), {500, 500, 50, 50});
        testRect(box->getInterBody(1.0), {500, 500, 50, 50});
        testRect(box->calcDrawBody(0, viewport), {500, 500, 50, 50});
        testRect(box->calcDrawBody(0.5, viewport), {500, 500, 50, 50});
        testRect(box->calcDrawBody(1.0, viewport), {500, 500, 50, 50});

        box->preUpdate();
        box->moveDeltaInstantly(100.0, 100.0);

        testRect(box->getInterBody(0), {500, 500, 50, 50});
        testRect(box->getInterBody(0.5), {550, 550, 50, 50});
        testRect(box->getInterBody(1.0), {600, 600, 50, 50});
        testRect(box->calcDrawBody(0, viewport), {500, 500, 50, 50});
        testRect(box->calcDrawBody(0.5, viewport), {550, 550, 50, 50});
        testRect(box->calcDrawBody(1.0, viewport), {600, 600, 50, 50});

        viewport.setPosition({200, 150});
        
        testRect(box->getInterBody(0), {500, 500, 50, 50});
        testRect(box->getInterBody(0.5), {550, 550, 50, 50});
        testRect(box->getInterBody(1.0), {600, 600, 50, 50});
        testRect(box->calcDrawBody(0, viewport), {700, 350, 50, 50});
        testRect(box->calcDrawBody(0.5, viewport), {750, 400, 50, 50});
        testRect(box->calcDrawBody(1.0, viewport), {800, 450, 50, 50});

        viewport.setZoom(0.27);
        
        testRect(box->getInterBody(0), {500, 500, 50, 50});
        testRect(box->getInterBody(0.5), {550, 550, 50, 50});
        testRect(box->getInterBody(1.0), {600, 600, 50, 50});
        testRect(box->calcDrawBody(0, viewport), {554, 459, 14, 14});
        testRect(box->calcDrawBody(0.5, viewport), {567, 473, 14, 14});
        testRect(box->calcDrawBody(1.0, viewport), {581, 486, 14, 14});
    }
}