#include "Application.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "RendererComponent.h"
#include "KeyboardController.h"
#include "Renderable.h"
#include "TextElement.h"
#include "MenuController.h"
#include "BallController.h"
#include "Sound.h"
#include <random>
//#define PARALLELEXEC - TODO check how to execute some functions in a separate thread

#ifdef PARALLELEXEC
#include <thread>
#endif // PARALLELEXEC

// DEBUG
#include <iostream>

SDL_Renderer* App::glb_renderer = nullptr;
App* App::glb_appPtr = nullptr;
SDL_Event App::glb_event;

SDL_Rect App::glb_camera = {0, 0, 800, 640};

bool App::glb_isRunning = false;

std::stack<std::unique_ptr<State>> App::glb_states;

AudioMgr App::glb_audioMgr;

GameObjectMgr mgr;

auto& player(mgr.addGObj());
auto& player2(mgr.addGObj());
auto& rTracker(mgr.addGObj());
auto& lTracker(mgr.addGObj());
auto& ball(mgr.addGObj());

// menu
auto& playButton(mgr.addGObj());
auto& exitButton(mgr.addGObj());
auto& gameTitle(mgr.addGObj());
auto& menuController(mgr.addGObj());

// player choice
auto& onePlayerButton(mgr.addGObj());
auto& twoPlayersButton(mgr.addGObj());
auto& plChMenController(mgr.addGObj());

App::App()
{
}

App::~App()
{
}

void App::init(const char* title, int width, int height, bool fullscreen)
{
    App::glb_appPtr = this;
    int flags = 0;
    m_RSCORE = m_LSCORE = 0;

    if(fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        glb_renderer = SDL_CreateRenderer(m_window, -1, 0);
        if(glb_renderer){
            SDL_SetRenderDrawColor(glb_renderer, 0, 0, 0, 255);
        }
        glb_isRunning = true;
    }

    if(TTF_Init() == -1)
    {
        std::cout << "ERROR: TTF NOT INITALIZED" << std::endl; // Change to some debugging technique
    }

    glb_audioMgr.init();

    player.addComponent<TransformComponent>(L_INIT_POS, 100.0f, 100, 20, 4, ObjectTag::PLAYER);
    player.addComponent<RendererComponent>()
                .addRenderable<RenderableRec>();
    player.addComponent<KeyboardController>(SDLK_w, SDLK_s);
    player.addGroup(groupPlayers);


    player2.addComponent<TransformComponent>(950.0f, 100.0f, 100, 20, 4, ObjectTag::PLAYER);
    player2.addComponent<RendererComponent>();
    player2.getComponent<RendererComponent>()
            .addRenderable<RenderableRec>();
    player2.addComponent<KeyboardController>(SDLK_UP, SDLK_DOWN);
    player2.addGroup(groupPlayers);

    for(int i = 0;i<9;i++)
    {
        auto& netObj(mgr.addGObj());
        netObj.addComponent<TransformComponent>(static_cast<float>((SCREEN_WIDTH/2 - NET_WIDTH/2)),static_cast<float>(i * (NET_HEIGHT + NET_SPACE)),
                                                NET_HEIGHT,
                                                NET_WIDTH,
                                                1, ObjectTag::DEFAULT);
        netObj.addComponent<RendererComponent>()
                    .addRenderable<RenderableRec>();
        netObj.addGroup(groupNet);
    }

    ball.addComponent<TransformComponent>(static_cast<float>((SCREEN_WIDTH/2)),
                                          static_cast<float>(SCREEN_HEIGHT/2),
                                          RADIUS, ObjectTag::BALL);
    // initially we will set ball speed to 0
    // also change to method, not direct call
    ball.getComponent<TransformComponent>().m_speed = 0;
    ball.addComponent<RendererComponent>();
    ball.getComponent<RendererComponent>()
        .addRenderable<RenderableCircle>();
    ball.addComponent<BallController>(&player, &player2); // FIX
    ball.addGroup(groupBall);

    SDL_Color white = {255, 255, 255, 255};

    lTracker.addComponent<TextElement>(250, 50, "0", "pixfont.ttf", 125, white);
    lTracker.addComponent<ScoreTracker>();
    lTracker.addGroup(groupUI);

    rTracker.addComponent<TextElement>(700, 50, "0", "pixfont.ttf", 125, white);
    rTracker.addComponent<ScoreTracker>();
    rTracker.addGroup(groupUI);
    MainGameState* mGameState(new MainGameState(this));

    std::unique_ptr<State> scene1 {mGameState};

    glb_states.push(std::move(scene1));

    SDL_Color gray = {100, 100, 100, 255};

    playButton.addComponent<MenuElement>(325,250, "PLAY", "pixfont.ttf", 200, white, gray, true);
    playButton.getComponent<MenuElement>().setAction(new PlayAction());
    playButton.addGroup(groupMenuUI);

    exitButton.addComponent<MenuElement>(325,400, "EXIT", "pixfont.ttf", 200, white, gray, false);
    exitButton.getComponent<MenuElement>().setAction(new ExitAction());
    exitButton.addGroup(groupMenuUI);

    gameTitle.addComponent<TextElement>(250,50, "PONG", "pixfont.ttf", 300, white);
    gameTitle.addGroup(groupMenuUI);
    MenuGameState* menuGmState(new MenuGameState(this));

    menuController.addComponent<MenuController>();
    menuController.getComponent<MenuController>()
                  .addUIElement(&playButton);
    menuController.getComponent<MenuController>()
                  .addUIElement(&exitButton);
    menuController.addGroup(groupMenuController);
    std::unique_ptr<State> scene2 {menuGmState};

    glb_states.push(std::move(scene2));
    menuController.getComponent<MenuController>().m_isMenuDisabled = false;

    glb_states.top()->init();
}

void PlayerChoiceState::init()
{
    SDL_Color gray = {100, 100, 100, 255};
    SDL_Color white = {255, 255, 255, 255};

    onePlayerButton.addComponent<MenuElement>(325,250, "I Player", "pixfont.ttf", 200, white, gray, true);
    onePlayerButton.getComponent<MenuElement>().setAction(new PlayAction());
    onePlayerButton.addGroup(App::groupPlayerChoiceUI);

    twoPlayersButton.addComponent<MenuElement>(325,400, "II Players", "pixfont.ttf", 200, white, gray, false);
    twoPlayersButton.getComponent<MenuElement>().setAction(new PlayAction());
    twoPlayersButton.addGroup(App::groupPlayerChoiceUI);

    plChMenController.addComponent<MenuController>();
    plChMenController.getComponent<MenuController>()
                  .addUIElement(&onePlayerButton);
    plChMenController.getComponent<MenuController>()
                  .addUIElement(&twoPlayersButton);
    plChMenController.addGroup(App::groupMenuController);

}

auto& players(mgr.getGroup(App::groupPlayers));
auto& net(mgr.getGroup(App::groupNet));
auto& balls(mgr.getGroup(App::groupBall));
auto& UI(mgr.getGroup(App::groupUI));

auto& MenuUI(mgr.getGroup(App::groupMenuUI));

auto& PlCh(mgr.getGroup(App::groupPlayerChoiceUI));

void App::handleEvents()
{
    SDL_PollEvent(&glb_event);

	switch (glb_event.type)
	{
	case SDL_QUIT :
		glb_isRunning = false;
		break;
	default:
		break;
	}
}

void App::update()
{
    /*
    mgr.update();


    if(ball.getComponent<TransformComponent>().m_position.x < 0)
    {
        m_RSCORE++;
        rTracker.getComponent<ScoreTracker>()
                .updateScore(m_RSCORE);
        std::cout << m_RSCORE << std::endl;
        // logic for winning
        // else reset:
        reset();
    }else if(ball.getComponent<TransformComponent>().m_position.x > SCREEN_WIDTH)
    {
        m_LSCORE++;
        lTracker.getComponent<ScoreTracker>()
                .updateScore(m_LSCORE);
        std::cout << m_LSCORE << std::endl;
        reset();
    }
    */
   glb_states.top()->update();
   glb_audioMgr.update();
}

static std::random_device rd;

static std::mt19937 gen(rd());
int generateRandomValue() {


    std::uniform_int_distribution<int> distribution(0, 1);

    return (distribution(gen) == 0) ? -1 : 1;
}
// SPECIFICALLY FOR Y COORDINATE TEMP SOL
int generateRandomValueY() {


    std::uniform_int_distribution<int> distribution(-1, 1);

    return (distribution(gen));
}


void App::reset()
{
    // inital central position set
    ball.getComponent<TransformComponent>().m_position.x = static_cast<float>(SCREEN_WIDTH/2);
    ball.getComponent<TransformComponent>().m_position.y = static_cast<float>(SCREEN_HEIGHT/2);

    ball.getComponent<TransformComponent>().m_speed = DEF_BALL_SPEED;

    // Random direction toss
    ball.getComponent<TransformComponent>().m_velocity.x =    ball.getComponent<TransformComponent>().m_velocity.x * generateRandomValue();
    ball.getComponent<TransformComponent>().m_velocity.y =    ball.getComponent<TransformComponent>().m_velocity.y * generateRandomValueY();
}

void App::render()
{
    SDL_RenderClear(glb_renderer);

    /*
    // Rendering here
    for (auto& p : players)
	{
		p->render();
	}

	for(auto& n : net)
    {
        n->render();
    }

    for(auto& b : balls)
    {
        b->render();
    }

    for(auto& ui: UI)
    {
        ui->render();
    }
    */
    glb_states.top()->render();
    SDL_RenderPresent(glb_renderer);
}

void App::clean()
{
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(glb_renderer);
    SDL_Quit();
}

bool App::isAppRunning(){
    return glb_isRunning;
}


// Simplified state logic

MainGameState::MainGameState(App* app){
        m_appPtr = app;
}


void MainGameState::update() {
    mgr.update();

    if(ball.getComponent<TransformComponent>().m_position.x < 0)
    {
        m_appPtr->m_RSCORE++;
        rTracker.getComponent<ScoreTracker>()
                .updateScore(m_appPtr->m_RSCORE);
        //std::cout << m_appPtr->m_RSCORE << std::endl;
        // logic for winning
        // else reset:
        m_appPtr->reset();
    }else if(ball.getComponent<TransformComponent>().m_position.x > SCREEN_WIDTH)
    {
        m_appPtr->m_LSCORE++;
        lTracker.getComponent<ScoreTracker>()
                .updateScore(m_appPtr->m_LSCORE);
       // std::cout << m_appPtr->m_LSCORE << std::endl;
        m_appPtr->reset();
    }
}

void MainGameState::render() {
    for (auto& p : players)
	{
		p->render();
	}

	for(auto& n : net)
    {
        n->render();
    }

    for(auto& b : balls)
    {
        b->render();
    }

    for(auto& ui: UI)
    {
        ui->render();
    }
}

void MenuGameState::update()
{
    mgr.update();
}

void MenuGameState::render()
{
    for(auto& mUI: MenuUI)
    {
        mUI->render();
    }
}

MenuGameState::MenuGameState(App* app){
        m_appPtr = app;
}

PlayerChoiceState::PlayerChoiceState(App* app){
        m_appPtr = app;
}

void MenuGameState::init()
{
    // for now empty, do nothing here
}

void App::resetScore()
{
    m_RSCORE = m_LSCORE = 0;
    rTracker.getComponent<ScoreTracker>()
                .updateScore(m_RSCORE);
    lTracker.getComponent<ScoreTracker>()
                .updateScore(m_LSCORE);
}

void MainGameState::init()
{
    m_appPtr->resetScore();
    ball.getComponent<TransformComponent>().m_speed = DEF_BALL_SPEED;
}

void MainGameState::pause() // the method called pause but generally it will be just a reset
// I decided not to implement the pause system and etc
// this method will be used as exiting to menu
{
    ball.getComponent<TransformComponent>().m_speed = 0;
    ball.getComponent<TransformComponent>().m_position.x = static_cast<float>((SCREEN_WIDTH/2));
    ball.getComponent<TransformComponent>().m_position.y = static_cast<float>((SCREEN_HEIGHT/2));
}

void MenuGameState::pause()
{
}

void PlayerChoiceState::update()
{
    mgr.update();
}

void PlayerChoiceState::pause()
{

}

void PlayerChoiceState::render()
{
    for(auto& pch : PlCh)
    {
        pch->render();
    }
}
