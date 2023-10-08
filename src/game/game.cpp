#include "cos_graphics/game.h"
#include "cos_graphics/log.h"
#include "cos_graphics/component/component.h"
#include "cos_graphics/component/camera.h"
#include "cos_graphics/game_factory.h"

CGGame::CGGame() : window_properties(WindowProperties(640, 480, CGSTR(""), CG_FALSE, CG_FALSE, CG_FALSE, CG_FALSE, CG_FALSE))
{
    
}

CGGame::~CGGame()
{
    game_initialized = false;
}

CGGame* CGGame::GetInstance()
{
    CG_ERROR_COND_EXIT(game_instance == nullptr, -1, CGSTR("The game instance is NULL. Please initialize the game before getting game instance."));
    return game_instance;
}

void CGGame::InitGame(unsigned int p_width, unsigned int p_height, const CGString& p_title, 
    CG_BOOL p_fullscreen, CG_BOOL p_resizable, CG_BOOL p_boarderless, CG_BOOL p_transparent, CG_BOOL p_topmost)
{
    InitGame(new CGGameFactory(), p_width, p_height, p_title, p_fullscreen, p_resizable, p_boarderless, p_transparent, p_topmost);
}

void CGGame::InitGame(unsigned int p_width, unsigned int p_height, const CGString& p_title, 
    CG_BOOL p_fullscreen, CG_BOOL p_resizable)
{
    InitGame(p_width, p_height, p_title, p_fullscreen, p_resizable, CG_FALSE, CG_FALSE, CG_FALSE);
}

void CGGame::InitGame(unsigned int p_width, unsigned int p_height, const CGString& p_title)
{
    InitGame(p_width, p_height, p_title, CG_FALSE, CG_TRUE, CG_FALSE, CG_FALSE, CG_FALSE);
}

void CGGame::InitGame(CGGameFactory*&& p_factory, unsigned int p_width, unsigned int p_height, const CGString& p_title, 
    CG_BOOL p_fullscreen, CG_BOOL p_resizable, CG_BOOL p_boarderless, CG_BOOL p_transparent, CG_BOOL p_topmost)
{
    CG_PRINT(CGSTR("Initializing game..."));
    if (game_factory != nullptr)
        delete game_factory;
    game_factory = p_factory;
    if (CGGame::game_instance != nullptr)
    {
        delete game_instance;
        game_instance = nullptr;
    }
    CGGame::game_instance = game_factory->CreateGame();
    CGGame::game_instance->window_properties = WindowProperties(p_width, p_height, p_title, p_fullscreen, 
        p_resizable, p_boarderless, p_transparent, p_topmost);
    CG_PRINT(CGSTR("Creating window..."));
    
    CGWindowSubProperty window_sub_property = CGConstructDefaultWindowSubProperty();
    window_sub_property.use_full_screen = p_fullscreen;
    window_sub_property.resizable = p_resizable;
    window_sub_property.boarderless = p_boarderless;
    window_sub_property.transparent = p_transparent;
    window_sub_property.topmost = p_topmost;

    CGGame::game_instance->game_window = CGCreateWindow(
        p_width, 
        p_height, 
        p_title.c_str(),
        window_sub_property);
    
    CG_ERROR_COND_EXIT(CGGame::game_instance->game_window == nullptr, -1, CGSTR("Failed to create window"));
    CG_PRINT(CGSTR("Window created."));
    CG_PRINT(CGSTR("Game initialized."));
    game_instance->game_initialized = true;
    game_instance->main_camera = new CGCamera();
}

void CGGame::InitGame(CGGameFactory*&& p_factory, unsigned int p_width, unsigned int p_height, const CGString& p_title, 
    CG_BOOL p_fullscreen, CG_BOOL p_resizable)
{
    InitGame(std::move(p_factory), p_width, p_height, p_title, p_fullscreen, p_resizable, CG_FALSE, CG_FALSE, CG_FALSE);
}

void CGGame::InitGame(CGGameFactory*&& p_factory, unsigned int p_width, unsigned int p_height, const CGString& p_title)
{
    InitGame(std::move(p_factory), p_width, p_height, p_title, CG_FALSE, CG_TRUE, CG_FALSE, CG_FALSE, CG_FALSE);
}

void CGGame::StartGame()
{
    CG_ERROR_CONDITION(game_instance == nullptr || !game_instance->game_initialized, CGSTR("Game is not initialized. Please initialize the game before starting the game."));
    game_instance->Ready();
    game_instance->root_component = game_factory->CreateRootComponent();
    game_instance->root_component->OnEnter();
    game_instance->GameLoop();
}

void CGGame::ExitGame()
{
    CG_ERROR_CONDITION(game_instance == nullptr || !game_instance->game_initialized, CGSTR("Game is not initialized. Please initialize the game before exiting the game."));
    delete game_instance->root_component;
    delete game_instance;
    game_instance = nullptr;
    CGTerminateGraphics();
    CG_PRINT(CGSTR("Game exited."));
    delete game_factory;
}

void CGGame::AddComponent(CGComponent* p_component)
{
    component_list.insert(component_list.end(), p_component);
}

void CGGame::RemoveComponent(CGComponent* p_component)
{
    for (auto iter = component_list.begin(); iter < component_list.end(); ++iter)
    {
        if (*iter != p_component)
            continue;
        else
        {
            component_list.erase(iter);
            break;
        }
    }
}

void CGGame::SetWindowClearColor(const CGColor& p_color)
{
    CG_ERROR_CONDITION(game_instance == nullptr || !game_instance->game_initialized, CGSTR("Game is not initialized. Please initialize the game before setting the window clear color."));
    CGSetClearScreenColor(p_color);
}

const CGCamera* CGGame::GetMainCamera() const noexcept
{
    return main_camera;
}

CGCamera* CGGame::GetMainCamera() noexcept
{
    return main_camera;
}

void CGGame::SetMainCamera(CGCamera* p_camera) noexcept
{
    main_camera = p_camera;
}

const CGWindow* CGGame::GetGameWindow() const noexcept
{
    return game_window;
}

CGWindow* CGGame::GetGameWindow() noexcept
{
    return game_window;
}

void CGGame::Tick(float p_delta)
{
    Update(p_delta);
    for (auto& i : component_list)
    {
        i->Tick(p_delta);
        if (i->IsQueueFreed())
            component_free_list.insert(component_free_list.end(), i);
    }
}

void CGGame::GameLoop()
{
    double tick_end_time = CGGetCurrentTime();
    while (!CGShouldWindowClose(game_window))
    {
        static double tick_start_time = 0;
        static double delta = 0.01;
        tick_start_time = CGGetCurrentTime();
        
        CGTickRenderStart(game_window);
        Tick(delta);
        CGWindowDraw(game_window);
        for (auto i = component_free_list.begin(); i < component_free_list.end(); ++i)
        {
            delete *i;
            component_free_list.erase(i);
        }
        CGTickRenderEnd();

        tick_end_time = CGGetCurrentTime();
        delta = tick_end_time - tick_start_time;
    }
}