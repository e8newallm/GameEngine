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
        },
        {
            "name": "sprite02",
            "texture": "spritemap.png",
            "x": 0,
            "y": 150,
            "height": 150,
            "width": 150
        },
        {
            "name": "sprite03",
            "texture": "spritemap.png",
            "x": 0,
            "y": 300,
            "height": 150,
            "width": 150
        },
        {
            "name": "sprite04",
            "texture": "spritemap.png",
            "x": 450,
            "y": 300,
            "height": 150,
            "width": 150
        }
    ],
    "Animations": [
        {
            "name": "explosion",
            "frames": [
                "sprite01",
                "sprite02",
                "sprite03",
                "sprite04"
            ],
            "FPS": 5.0
        }
    ]
}
)";