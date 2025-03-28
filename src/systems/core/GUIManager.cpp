#include "systems/core/GUIManager.hpp"

#define TEXT_HEIGHT 10
#define CONFIGURATION_FILE "data/configurations/GUIManagerConfiguration.json"

#include <array>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Texture.hpp"
#include "systems/core/TextureManager.hpp"
#include "systems/core/MouseManager.hpp"
#include "systems/game_objects/StructureFactory.hpp"

static int text_width(mu_Font font, const char *text, int len)
{
    if (len == -1)
    {
        len = strlen(text);
    }
    return len * 5;
}

static int text_height(mu_Font font) { return TEXT_HEIGHT; }

GUIManager::GUIManager(SDL_Window *window, SDL_Renderer *renderer, TextureManager *textureManager, TickManager *tickManager, StructureFactory *structureFactory, MouseManager *mouseManager) : window(window), renderer(renderer), textureManager(textureManager), tickManager(tickManager), structureFactory(structureFactory), structureNamesList(structureFactory->getRegistredClasses()), mouseManager(mouseManager)
{
    button_map[SDL_BUTTON_LEFT & 0xff] = MU_MOUSE_LEFT;
    button_map[SDL_BUTTON_RIGHT & 0xff] = MU_MOUSE_RIGHT;
    button_map[SDL_BUTTON_MIDDLE & 0xff] = MU_MOUSE_MIDDLE;
    key_map[SDLK_LSHIFT & 0xff] = MU_KEY_SHIFT;
    key_map[SDLK_RSHIFT & 0xff] = MU_KEY_SHIFT;
    key_map[SDLK_LCTRL & 0xff] = MU_KEY_CTRL;
    key_map[SDLK_RCTRL & 0xff] = MU_KEY_CTRL;
    key_map[SDLK_LALT & 0xff] = MU_KEY_ALT;
    key_map[SDLK_RALT & 0xff] = MU_KEY_ALT;
    key_map[SDLK_RETURN & 0xff] = MU_KEY_RETURN;
    key_map[SDLK_BACKSPACE & 0xff] = MU_KEY_BACKSPACE;

    r_init(window, renderer);
    mu_init(&ctx);
    ctx.text_width = text_width;
    ctx.text_height = text_height;
    loadIcons();
    loadConfiguration();
}

GUIManager::~GUIManager() { structureTextureIdToTexture.clear(); }

void GUIManager::loadConfiguration() {}

void GUIManager::loadIcons()
{
    // for mu_draw_icon id parameter
    structureTextureIdToTexture.push_back(textureManager->getTexture("DEFAULT"));        // 0
    structureTextureIdToTexture.push_back(textureManager->getTexture("DEFAULT"));        // MU_ICON_CLOSE
    structureTextureIdToTexture.push_back(textureManager->getTexture("DEFAULT"));        // MU_ICON_CHECK
    structureTextureIdToTexture.push_back(textureManager->getTexture("ICON_COLLAPSED")); // MU_ICON_COLLAPSED
    structureTextureIdToTexture.push_back(textureManager->getTexture("ICON_EXPANDED"));  // MU_ICON_EXPANDED
    structureTextureIdToTexture.push_back(textureManager->getTexture("DEFAULT"));        // MU_ICON_MAX
    int size = structureNamesList.size();
    for (int i = 0; i < size; i++)
    {
        int index = i + 6;
        std::string name = structureNamesList[i];
        structureTextureNameToId[name] = index;
        structureTextureIdToTexture.push_back(textureManager->getTexture(name));
    }
}

void GUIManager::test_window(mu_Context *ctx, Player *player)
{
    if (mu_begin_window(ctx, "Structures", mu_rect(40, 40, 300, 450)))
    {
        mu_Container *win = mu_get_current_container(ctx);
        win->rect.w = mu_max(win->rect.w, 240);
        win->rect.h = mu_max(win->rect.h, 300);

        std::vector<int> iconIds = {1, 2, 3};

        if (mu_header_ex(ctx, "Structure names", MU_OPT_EXPANDED))
        {
            int row_sizes[] = {30, -1};
            mu_layout_row(ctx, 2, row_sizes, 25);

            size_t size = structureNamesList.size();
            for (size_t i = 0; i < size; ++i)
            {
                mu_Rect r = mu_layout_next(ctx);
                std::string structureName = structureNamesList[i];
                int id = structureTextureNameToId[structureName];
                mu_draw_icon(ctx, id, mu_rect(r.x, r.y, r.h, r.h), ctx->style->colors[MU_COLOR_TEXT]);
                if (mu_button_ex(ctx, structureNamesList[i].c_str(), 0, MU_OPT_ALIGNCENTER))
                {
                    std::cout << "Button pressed : " << structureNamesList[i] << std::endl;
                    changeMouseManagerClickOnEmptyTileStrategy(structureName, player);
                }
            }
        }
        mu_end_window(ctx);
    }
}

void GUIManager::changeMouseManagerClickOnEmptyTileStrategy(std::string structureName, Player *placedBy)
{
    std::function<Structure *(std::string, float, float, Player *, TickManager *)> constructor = structureFactory->getConstructor(structureName);
    TickManager *tempoTickManager = tickManager;
    std::function<Structure *(float, float)> newFunction = [constructor, structureName, placedBy, tempoTickManager](float i, float j) -> Structure *
    {
        Structure *structure = constructor(structureName, i, j, placedBy, tempoTickManager);
        return structure;
    };
    mouseManager->setClickOnEmptyTileStrategy(newFunction);
}

bool GUIManager::isMouseOverGUI(int x, int y)
{
    mu_Container *hoveredContainer = ctx.hover_root;
    if (hoveredContainer)
    {
        mu_Rect rect = hoveredContainer->rect;
        return (x >= rect.x && x <= rect.x + rect.w &&
                y >= rect.y && y <= rect.y + rect.h);
    }
    return false;
}

bool GUIManager::handleEvents(SDL_Event *event)
{
    bool eventConsumed = false;
    switch (event->type)
    {
    case SDL_QUIT:
        exit(EXIT_SUCCESS);
        break;
    case SDL_MOUSEMOTION:
        mu_input_mousemove(&ctx, event->motion.x, event->motion.y);
        eventConsumed = true;
        break;
    case SDL_MOUSEWHEEL:
        mu_input_scroll(&ctx, 0, event->wheel.y * -30);
        eventConsumed = true;
        break;
    case SDL_TEXTINPUT:
        mu_input_text(&ctx, event->text.text);
        eventConsumed = true;
        break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    {
        if (isMouseOverGUI(event->button.x, event->button.y))
        {
            int b = button_map[event->button.button & 0xff];
            if (b && event->type == SDL_MOUSEBUTTONDOWN)
            {
                mu_input_mousedown(&ctx, event->button.x, event->button.y, b);
                eventConsumed = true;
            }
            if (b && event->type == SDL_MOUSEBUTTONUP)
            {
                mu_input_mouseup(&ctx, event->button.x, event->button.y, b);
                eventConsumed = true;
            }
        }
        break;
    }
    case SDL_KEYDOWN:
    case SDL_KEYUP:
    {
        int c = key_map[event->key.keysym.sym & 0xff];
        if (c && event->type == SDL_KEYDOWN)
        {
            mu_input_keydown(&ctx, c);
            eventConsumed = true;
        }
        if (c && event->type == SDL_KEYUP)
        {
            mu_input_keyup(&ctx, c);
            eventConsumed = true;
        }
        break;
    }
    }
    return eventConsumed;
}

void GUIManager::process_frame(mu_Context *ctx, Player *player)
{
    mu_begin(ctx);
    test_window(ctx, player);
    mu_end(ctx);
}

void GUIManager::render(Player *player)
{
    process_frame(&ctx, player);
    mu_Command *cmd = NULL;
    while (mu_next_command(&ctx, &cmd))
    {
        switch (cmd->type)
        {
        case MU_COMMAND_TEXT:
            r_draw_text(cmd->text.str, cmd->text.pos, cmd->text.color);
            break;
        case MU_COMMAND_RECT:
            r_draw_rect(cmd->rect.rect, cmd->rect.color);
            break;
        case MU_COMMAND_ICON:
            r_draw_icon(cmd->icon.id, cmd->icon.rect, cmd->icon.color);
            break;
        case MU_COMMAND_CLIP:
            r_set_clip_rect(cmd->clip.rect);
            break;
        }
    }
    // r_present();
}

void GUIManager::r_init(SDL_Window *window1, SDL_Renderer *renderer1)
{
    window = window1;
    renderer = renderer1;
    font = TTF_OpenFont("assets/fonts/Raleway-Medium.ttf", TEXT_HEIGHT);
    if (!font)
    {
        return;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void GUIManager::r_draw_rect(mu_Rect rect, mu_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect sdl_rect = {rect.x, rect.y, rect.w, rect.h};
    SDL_RenderFillRect(renderer, &sdl_rect);
}

void GUIManager::r_init_font(const char *font_path, int font_size)
{
    if (TTF_Init() == -1)
    {
        exit(1);
    }

    font = TTF_OpenFont(font_path, font_size);
    if (!font)
    {
        exit(1);
    }
}

void GUIManager::r_draw_text(const char *text, mu_Vec2 pos, mu_Color color)
{
    SDL_Color sdl_color = {color.r, color.g, color.b, color.a};
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, sdl_color);
    if (!surface)
    {
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dst_rect = {pos.x, pos.y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dst_rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void GUIManager::r_draw_icon(int id, mu_Rect rect, mu_Color color)
{
    SDL_Texture *iconTexture = structureTextureIdToTexture[id]->getTexture();
    SDL_SetTextureColorMod(iconTexture, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(iconTexture, color.a);
    SDL_Rect dst = {rect.x, rect.y, rect.w, rect.h};
    SDL_RenderCopy(renderer, iconTexture, nullptr, &dst);
}

void GUIManager::r_set_clip_rect(mu_Rect rect)
{
    SDL_Rect sdl_rect = {rect.x, rect.y, rect.w, rect.h};
    SDL_RenderSetClipRect(renderer, &sdl_rect);
}

void GUIManager::r_clear(mu_Color clr)
{
    SDL_SetRenderDrawColor(renderer, clr.r, clr.g, clr.b, clr.a);
    SDL_RenderClear(renderer);
}

void GUIManager::r_present(void)
{
    SDL_RenderPresent(renderer);
}

int GUIManager::uint8_slider(mu_Context *ctx, unsigned char *value, int low, int high)
{
    static float tmp;
    mu_push_id(ctx, &value, sizeof(value));
    tmp = *value;
    int res = mu_slider_ex(ctx, &tmp, low, high, 0, "%.0f", MU_OPT_ALIGNCENTER);
    *value = tmp;
    mu_pop_id(ctx);
    return res;
}
