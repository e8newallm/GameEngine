#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <catch2/catch_test_macros.hpp>
#include <cstdlib>
#include <ios>
#include <rapidjson/document.h>
#include <rapidjson/schema.h>
#include <rapidjson/stringbuffer.h>

#include <cstdio>
#include <iostream>
#include <ctime>
#include <source_location>
#include <fstream>
#include <sstream>
#include <string>

#include "datastore.h"
#include "physicsobject.h"
#include "world.h"

#include "texture.h"
#include "spritemap.h"
#include "spritemapdata.h"

#include "mousestate.h"
#include "keystate.h"
#include "logging.h"
#include "geerror.h"

#include "catch2/catch_all.hpp"

using namespace GameEng;

extern const char* SpriteMapSchema;

uint64_t lineNumber;
void loggingTestFunc(std::string message)
{
    Logger::message(message);
    lineNumber = __LINE__ - 1;
}

TEST_CASE("Error", "[base][exceptions]")
{
    SECTION("Error testing")
    {
        try {
            throw GameEngineException(GEError::FILE_IO, "No error occurred");
        } catch (GameEngineException& err) {
            REQUIRE(err.code() == GEError::FILE_IO);
            REQUIRE(std::string(err.what()) == "No error occurred");
        }
    }
}

TEST_CASE("Store", "[base][store]")
{
    int* testValue = new int(12);
    DataStore<int, char> testStore;
    SECTION("Basic test")
    {
        REQUIRE_THROWS_WITH(testStore.get("MISSING ENTRY"),
                            "Requested entry MISSING ENTRY not found in char store");

        REQUIRE(!testStore.exists("test"));

        testStore.add(testValue, "test");
        REQUIRE(testStore.exists("test"));
        REQUIRE(*testStore.get("test") == 12);
        *testValue = 35;
        REQUIRE(*testStore.get("test") == 35);
    }
    delete testValue;
}

TEST_CASE("Logging", "[base][logging]")
{
    SECTION("Ostream test")
    {
        std::ostringstream testStream;
        Logger::init(&testStream);
        loggingTestFunc("TEST MESSAGE");
        loggingTestFunc("NEW MESSAGE");
        REQUIRE(testStream.str() == "INFO: TEST MESSAGE\r\n"
                                    "INFO: NEW MESSAGE\r\n");
    }

    std::string filename = "./testlog.log";
    std::fstream testFile;

    SECTION("File test")
    {
        testFile.open(filename, std::ios_base::out | std::ios_base::binary);
        Logger::init(&testFile);
        loggingTestFunc("TEST MESSAGE");
        loggingTestFunc("NEW MESSAGE");
        testFile.close();
        testFile.open(filename, std::ios_base::in | std::ios_base::binary);

        std::string line = "";
        std::getline(testFile, line);
        REQUIRE(line == "INFO: TEST MESSAGE\r");
        std::getline(testFile, line);
        REQUIRE(line == "INFO: NEW MESSAGE\r");
    }

    Logger::deinit();
    testFile.close();
    std::remove(filename.c_str());
}

TEST_CASE("Spritemap parse testing", "[spritemap]")
{
    REQUIRE(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS));
    Texture::add(new GPUTexture(), "testfiles/tex/spritemap.png");

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

    SECTION("JSON load/save sanity check", "[spritemap]")
    {
        SpriteMapData test, testTwo;
        std::string result = R"TEST({"Textures":["testfiles/tex/spritemap.png"],"Sprites":[{"name":"sprite01","texture":"testfiles/tex/spritemap.png","x":0,"y":0,"width":150,"height":150}],"Animations":[{"name":"explosion","FPS":5.0,"frames":["sprite01"]}]})TEST";

        REQUIRE_NOTHROW(test.loadFromFile("testfiles/json/spritemap/spritemap.json"));
        REQUIRE(test.serialise() == result);
        REQUIRE_NOTHROW(testTwo.loadFromString(result.c_str()));
        REQUIRE(test.serialise() == testTwo.serialise());
    }

    SECTION("SpriteMap schema test", "[spritemap][exceptions]")
    {
        REQUIRE_NOTHROW(SpriteMap("testfiles/json/spritemap/spritemap.json"));

        REQUIRE_THROWS_WITH(SpriteMap("UNKNOWN FILE"),
                            "Could not open Spritemap JSON \"UNKNOWN FILE\"");

        REQUIRE_THROWS_WITH(SpriteMap("testfiles/json/invalidjson.json"),
                            "\"testfiles/json/invalidjson.json\" is invalid JSON");

        REQUIRE_THROWS_WITH(SpriteMap("testfiles/json/spritemap/badspritemap.json"),
                            "\"testfiles/json/spritemap/badspritemap.json\" has failed to pass SpriteMap schema");

        REQUIRE_THROWS_WITH(SpriteMap("testfiles/json/spritemap/texturenotfound.json"),
                            "\"testfiles/json/spritemap/texturenotfound.json\" could not load texture file \"texspritemap.png\"");

        REQUIRE_THROWS_WITH(SpriteMap("testfiles/json/spritemap/mismatchedsprite.json"),
                            "\"testfiles/json/spritemap/mismatchedsprite.json\" has a sprite (sprite01) referencing a texture not named in the JSON");
        
        REQUIRE_THROWS_WITH(SpriteMap("testfiles/json/spritemap/mismatchedanimation.json"),
                            "\"testfiles/json/spritemap/mismatchedanimation.json\" has a animation (explosion) referencing a sprite not named in the JSON");
        
        REQUIRE_THROWS_WITH(SpriteMap("testfiles/json/spritemap/duplicatedsprite.json"),
                            "\"testfiles/json/spritemap/duplicatedsprite.json\" has two sprites named (sprite01)");

        REQUIRE_THROWS_WITH(SpriteMap("testfiles/json/spritemap/duplicatedanimation.json"),
                            "\"testfiles/json/spritemap/duplicatedanimation.json\" has two animations named (explosion)");
    }
}

TEST_CASE("KeyState testing", "[base]")
{
    KeyState::updateKey(SDL_SCANCODE_1, SDL_EVENT_KEY_DOWN);
    KeyState::updateKey(SDL_SCANCODE_2, SDL_EVENT_KEY_DOWN);
    KeyState::updateKey(SDL_SCANCODE_5, SDL_EVENT_KEY_DOWN);
    KeyState::updateKey(SDL_SCANCODE_A, SDL_EVENT_KEY_DOWN);

    REQUIRE(KeyState::key(SDL_SCANCODE_1) == SDL_EVENT_KEY_DOWN);
    REQUIRE(KeyState::key(SDL_SCANCODE_2) == SDL_EVENT_KEY_DOWN);
    REQUIRE(KeyState::key(SDL_SCANCODE_5) == SDL_EVENT_KEY_DOWN);
    REQUIRE(KeyState::key(SDL_SCANCODE_A) == SDL_EVENT_KEY_DOWN);

    KeyState::updateKey(SDL_SCANCODE_5, SDL_EVENT_KEY_UP);

    REQUIRE(KeyState::key(SDL_SCANCODE_1) == SDL_EVENT_KEY_DOWN);
    REQUIRE(KeyState::key(SDL_SCANCODE_2) == SDL_EVENT_KEY_DOWN);
    REQUIRE(KeyState::key(SDL_SCANCODE_5) == SDL_EVENT_KEY_UP);
    REQUIRE(KeyState::key(SDL_SCANCODE_A) == SDL_EVENT_KEY_DOWN);
}

TEST_CASE("MouseState testing", "[base]")
{

    MouseState::updateButton({
        SDL_EVENT_MOUSE_BUTTON_DOWN,
        0,
        0,
        0,
        0,
        SDL_BUTTON_LEFT,
        true,
        1,
        0,
        123,
        456
    });

    MouseState::updateMove({
        SDL_EVENT_MOUSE_MOTION,
        0,
        0,
        0,
        0,
        200,
        400,
        20,
        40,
        40
    });

    MouseState::updateWheel({
        SDL_EVENT_MOUSE_WHEEL,
        0,
        0,
        0,
        5,
        10,
        5.1,
        SDL_MOUSEWHEEL_FLIPPED,
        0,
        0
    });

    REQUIRE(MouseState::clicked(SDL_BUTTON_LEFT) == true);
    REQUIRE(MouseState::doubleClicked(SDL_BUTTON_LEFT) == false);
    REQUIRE(MouseState::clickPosition(SDL_BUTTON_LEFT).x == 123);
    REQUIRE(MouseState::clickPosition(SDL_BUTTON_LEFT).y == 456);
    REQUIRE(MouseState::buttonDown(SDL_BUTTON_LEFT) == true);

    REQUIRE(MouseState::mousePos().x == 400);
    REQUIRE(MouseState::mousePos().y == 20);
    REQUIRE(MouseState::mouseDelta().x == 40);
    REQUIRE(MouseState::mouseDelta().y == 40);

    REQUIRE(MouseState::scrollDelta() == -5);

    MouseState::reset();

    MouseState::updateButton({
        SDL_EVENT_MOUSE_BUTTON_UP,
        0,
        0,
        0,
        0,
        SDL_BUTTON_LEFT,
        false,
        0,
        0,
        123,
        456
    });

    REQUIRE(MouseState::clicked(SDL_BUTTON_LEFT) == false);
    REQUIRE(MouseState::doubleClicked(SDL_BUTTON_LEFT) == false);
    REQUIRE(MouseState::clickPosition(SDL_BUTTON_LEFT).x == 123);
    REQUIRE(MouseState::clickPosition(SDL_BUTTON_LEFT).y == 456);
    REQUIRE(MouseState::buttonDown(SDL_BUTTON_LEFT) == false);
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
    REQUIRE(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS));
    SDL_GPUDevice* gpu = SDL_CreateGPUDevice(0, false, NULL);

    View viewport( {1000, 1000}, {0, 0});
    viewport.setZoom(1.0);

    World world(gpu, viewport);

    SECTION("Interpolation calculation")
    {
        world.setGravity(0.0);
        PhysicsObject* box = new PhysicsObject({500, 500, 50, 50}, PhyObjFlag::Collide, new Texture());
        world.addObj(box);

        testRect(*box->getBody(), {500, 500, 50, 50});

        world.setPhyInterpolation(1.0f);
        box->velocityDelta(0.0, 100.0);
        box->runPhysics(1, world);
        box->update(1.0f, world);

        testRect(*box->getBody(), {500, 600, 50, 50});
    }

    SECTION("Box drop")
    {
        PhysicsObject* box = new PhysicsObject({500, 500, 10, 10}, PhyObjFlag::Collide, new Texture());
        world.addObj(box);

        box->velocityDelta(0.0, 10);
        REQUIRE(box->getBody()->x == 500);
        REQUIRE(box->getBody()->y == 500);

        world.setPhyInterpolation(0.5f);
        box->runPhysics(1, world);
        box->update(1, world);
        REQUIRE(box->getBody()->x == 500);
        REQUIRE(box->getBody()->y == 505);

        world.setPhyInterpolation(1.0f);
        box->update(1, world);
        REQUIRE(box->getBody()->x == 500);
        REQUIRE(box->getBody()->y == 510);
    }

    SECTION("Collision")
    {
        world.setGravity(0.0);
        PhysicsObject* box = new PhysicsObject({500, 500, 10, 10}, PhyObjFlag::Collide, new Texture());
        box->velocityDelta(11.0, 0.0);
        world.addObj(box);
        PhysicsObject* boxCollide = new PhysicsObject({520, 500, 10, 10}, PhyObjFlag::Collide, new Texture());
        world.addObj(boxCollide);

        //No collision
        box->runPhysics(0.5f, world);
        boxCollide->runPhysics(0.5f, world);

        world.setPhyInterpolation(0.5f);
        box->update(0.0f, world);
        boxCollide->update(0.0f, world);

        REQUIRE(box->getBody()->x == 502);
        REQUIRE(box->getBody()->y == 500);
        REQUIRE(box->getVelocity().y == 0.0);
        REQUIRE(box->getVelocity().x == 11.0);
        REQUIRE(boxCollide->getBody()->x == 520);
        REQUIRE(boxCollide->getBody()->y == 500);

        world.setPhyInterpolation(1.0f);
        box->update(0.0f, world);
        boxCollide->update(0.0f, world);

        REQUIRE(box->getBody()->x == 505);
        REQUIRE(box->getBody()->y == 500);
        REQUIRE(box->getVelocity().y == 0.0);
        REQUIRE(box->getVelocity().x == 11.0);
        REQUIRE(boxCollide->getBody()->x == 520);
        REQUIRE(boxCollide->getBody()->y == 500);

        //Collision
        box->runPhysics(1.0f, world);
        boxCollide->runPhysics(1.0f, world);

        world.setPhyInterpolation(0.5f);
        box->update(0.0f, world);
        boxCollide->update(0.0f, world);

        REQUIRE(box->getBody()->x == 507);
        REQUIRE(box->getBody()->y == 500);
        REQUIRE(box->getVelocity().y == 0.0);
        REQUIRE(box->getVelocity().x == 0.0);
        REQUIRE(boxCollide->getBody()->x == 520);
        REQUIRE(boxCollide->getBody()->y == 500);

        world.setPhyInterpolation(1.0f);
        box->update(0.0f, world);
        boxCollide->update(0.0f, world);

        REQUIRE(box->getBody()->x == 510);
        REQUIRE(box->getBody()->y == 500);
        REQUIRE(box->getVelocity().y == 0.0);
        REQUIRE(box->getVelocity().x == 0.0);
        REQUIRE(boxCollide->getBody()->x == 520);
        REQUIRE(boxCollide->getBody()->y == 500);
    }
}