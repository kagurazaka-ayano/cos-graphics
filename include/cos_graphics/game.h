#pragma once
#include "cos_graphics/graphics.h"

class CGGame
{
    /**
     * @brief The instance of the game.
     */
    inline static CGGame* game_instance = nullptr;

    /**
     * @brief The window for the game to run on.
     */
    CGWindow* game_window = nullptr;
    
    /**
     * @brief The properties of the window.
     */
    struct WindowProperties
    {
        unsigned int width;
        unsigned int height;
        const char* title;
        CG_BOOL resizable;
        CG_BOOL fullscreen;

        WindowProperties(unsigned int p_width, unsigned int p_height, const char* p_title, 
                CG_BOOL p_fullscreen = CG_FALSE, CG_BOOL p_resizable = CG_TRUE) 
            : width(p_width), height(p_height), title(p_title), resizable(p_resizable), fullscreen(p_fullscreen)
        {}
    } window_properties;

    /**
     * @brief Construct a new CGGame object
     */
    CGGame();
public:
    /**
     * @brief Get the instance of the game. If the game is not started, this will return nullptr.
     * 
     * @return CGGame* The instance of the game.
     */
    static CGGame* GetInstance();

    /**
     * @brief Start the game.
     */
    static void StartGame(
        unsigned int p_width = 640, 
        unsigned int p_height = 480, 
        const char* p_title = "", 
        CG_BOOL p_fullscreen = false, 
        CG_BOOL p_resizable = true);

    /**
     * @brief Destroy the CGGame object
     */
    virtual ~CGGame();
    
private:
    /**
     * @brief Called once when the game is created.
     */
    void Ready();

    /**
     * @brief Game loop.
     */
    void GameLoop();

    /**
     * @brief Update, called every frame.
     */
    void Update(float delta);

    /**
     * @brief Render loop
     */
    void Render();
};