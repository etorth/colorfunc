/*
 * =====================================================================================
 *
 *       Filename: colorfunc.hpp
 *        Created: 03/01/2018 11:46:11
 *    Description: 
 *                  
 *                  use uint32_t as color type
 *                  support color manipulation and conversion
 *
 *                  (R << 24) | (G << 16) | (B << 8) | A
 *
 *                  then we can use:
 *                  
 *                      ColorConst::RED +   0       // transparent
 *                      ColorConst::RED + 128       // red with half alpha
 *                      ColorConst::RED + 255       // solid red
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */

#ifndef __COLOR_FUNC_HPP__
#define __COLOR_FUNC_HPP__

#include <cmath>
#include <cstdio>
#include <cstring>
#include <algorithm>

#ifdef ENABLE_SDL_COLOR
#include <SDL2/SDL.h>
#endif

namespace ColorFunc
{
    enum ColorMask: uint32_t
    {
        MASK_R  = 0XFF000000,
        MASK_G  = 0X00FF0000,
        MASK_B  = 0X0000FF00,
        MASK_A  = 0X000000FF,
    };

    enum ColorShift: int
    {
        SHIFT_R = 24,
        SHIFT_G = 16,
        SHIFT_B =  8,
        SHIFT_A =  0,
    };

    constexpr uint8_t R(uint32_t nRGBA)
    {
        return (nRGBA & MASK_R) << SHIFT_R;
    }

    constexpr uint8_t G(uint32_t nRGBA)
    {
        return (nRGBA & MASK_G) << SHIFT_G;
    }

    constexpr uint8_t B(uint32_t nRGBA)
    {
        return (nRGBA & MASK_B) << SHIFT_B;
    }

    constexpr uint8_t A(uint32_t nRGBA)
    {
        return (nRGBA & MASK_A) << SHIFT_A;
    }

    constexpr uint32_t RGBA(uint8_t nR, uint8_t nG, uint8_t nB, uint8_t nA)
    {
        return ((uint32_t)(nR) << SHIFT_R) | ((uint32_t)(nG) << SHIFT_G) | ((uint32_t)(nB) << SHIFT_B)  | ((uint32_t)(nA) << SHIFT_A);
    }

    enum ColorConst: uint32_t
    {
        RED     = RGBA(0XFF, 0X00, 0X00, 0X00),
        GREEN   = RGBA(0X00, 0XFF, 0X00, 0X00),
        BLUE    = RGBA(0X00, 0X00, 0XFF, 0X00),

        BLACK   = RGBA(0X00, 0X00, 0X00, 0X00),
        WHITE   = RGBA(0XFF, 0XFF, 0XFF, 0X00),
        YELLOW  = RGBA(0XFF, 0XFF, 0X00, 0XFF),
    };


    SDL_Color RGBA2Color(uint8_t, uint8_t, uint8_t, uint8_t);
    SDL_Color ARGB2Color(uint8_t, uint8_t, uint8_t, uint8_t);

    SDL_Color RGBA2Color(uint32_t);
    SDL_Color ARGB2Color(uint32_t);

    uint32_t Color2RGBA(const SDL_Color &);
    uint32_t Color2ARGB(const SDL_Color &);

    SDL_Color RenderColor(const SDL_Color &, const SDL_Color &);

    uint32_t RenderARGB(uint32_t, uint32_t);
    uint32_t RenderRGBA(uint32_t, uint32_t);

    bool String2Color(SDL_Color *, const char *);
}

SDL_Color ColorFunc::RGBA2Color(uint8_t nR, uint8_t nG, uint8_t nB, uint8_t nA)
{
    SDL_Color stColor;
    stColor.r = nR;
    stColor.g = nG;
    stColor.b = nB;
    stColor.a = nA;
    return stColor;
}

SDL_Color ColorFunc::ARGB2Color(uint8_t nA, uint8_t nR, uint8_t nG, uint8_t nB)
{
    return RGBA2Color(nR, nG, nB, nA);
}

SDL_Color ColorFunc::RGBA2Color(uint32_t nRGBA)
{
    return RGBA2Color((nRGBA & 0XFF000000) >> 24, (nRGBA & 0X00FF0000) >> 16, (nRGBA & 0X0000FF00) >> 8, (nRGBA & 0X000000FF));
}

SDL_Color ColorFunc::ARGB2Color(uint32_t nARGB)
{
    return ARGB2Color((nARGB & 0XFF000000) >> 24, (nARGB & 0X00FF0000) >> 16, (nARGB & 0X0000FF00) >> 8, (nARGB & 0X000000FF));
}

uint32_t ColorFunc::Color2RGBA(const SDL_Color &rstColor)
{
    return 0
        | ((uint32_t)(rstColor.r) << 24)
        | ((uint32_t)(rstColor.g) << 16)
        | ((uint32_t)(rstColor.b) <<  8)
        | ((uint32_t)(rstColor.a) <<  0);
}

uint32_t ColorFunc::Color2ARGB(const SDL_Color &rstColor)
{
    uint32_t nRGBA = Color2RGBA(rstColor);
    return ((nRGBA & 0X000000FF) << 24) | ((nRGBA & 0XFFFFFF00) >> 8);
}

SDL_Color ColorFunc::RenderColor(const SDL_Color &rstDstColor, const SDL_Color &rstSrcColor)
{
    SDL_Color stRetColor;
    stRetColor.r = std::min<uint8_t>(255, std::lround((rstSrcColor.a / 255.0) * rstSrcColor.r + (1.0 - rstSrcColor.a / 255.0) * rstDstColor.r));
    stRetColor.g = std::min<uint8_t>(255, std::lround((rstSrcColor.a / 255.0) * rstSrcColor.g + (1.0 - rstSrcColor.a / 255.0) * rstDstColor.g));
    stRetColor.b = std::min<uint8_t>(255, std::lround((rstSrcColor.a / 255.0) * rstSrcColor.b + (1.0 - rstSrcColor.a / 255.0) * rstDstColor.b));
    stRetColor.a = std::min<uint8_t>(255, std::lround((rstSrcColor.a *   1.0)                 + (1.0 - rstSrcColor.a / 255.0) * rstDstColor.a));
    return stRetColor;
}

uint32_t ColorFunc::RenderRGBA(uint32_t nDstColor, uint32_t nSrcColor)
{
    return Color2RGBA(RenderColor(RGBA2Color(nDstColor), RGBA2Color(nSrcColor)));
}

uint32_t ColorFunc::RenderARGB(uint32_t nDstColor, uint32_t nSrcColor)
{
    return Color2ARGB(RenderColor(ARGB2Color(nDstColor), ARGB2Color(nSrcColor)));
}

bool ColorFunc::String2Color(SDL_Color *pstColor, const char *szText)
{
    if(!szText || std::strlen(szText) == 0){ return false; }

    bool bFind = true;
    SDL_Color stColor {0X00, 0X00, 0X00, 0XFF};
    if(false
            || !std::strcmp(szText, "RED")
            || !std::strcmp(szText, "Red")
            || !std::strcmp(szText, "red")){
        stColor.r = 0XFF;
    }else if(false
            || !std::strcmp(szText, "GREEN")
            || !std::strcmp(szText, "Green")
            || !std::strcmp(szText, "green")){
        stColor.g = 0XFF;
    }else if(false
            || !std::strcmp(szText, "BLUE")
            || !std::strcmp(szText, "Blue")
            || !std::strcmp(szText, "blue")){
        stColor.b = 0XFF;
    }else if(false
            || !std::strcmp(szText, "YELLOW")
            || !std::strcmp(szText, "Yellow")
            || !std::strcmp(szText, "yellow")){
        stColor.r = 0XFF;
        stColor.g = 0XFF;
    }else if(false
            || !std::strcmp(szText, "PURPLE")
            || !std::strcmp(szText, "Purple")
            || !std::strcmp(szText, "purple")){
        stColor.r = 0X80;
        stColor.b = 0X80;
    }else{
        // try "0XFF00FF00FF" mode
        int nRes = -1;
        uint32_t nARGB = 0XFFFFFFFF;

        if(false
                || ((nRes = std::sscanf(szText, "0X%08X", &nARGB)) == 1)
                || ((nRes = std::sscanf(szText, "0x%08X", &nARGB)) == 1)
                || ((nRes = std::sscanf(szText, "0x%08x", &nARGB)) == 1)
                || ((nRes = std::sscanf(szText, "0X%08x", &nARGB)) == 1)){
            stColor = ARGB2Color(nARGB);
        }else{
            bFind = false;
        }
    }

    if(bFind && pstColor){ *pstColor = stColor; }
    return bFind;
}

#endif
