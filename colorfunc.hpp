/*
 * =====================================================================================
 *
 *       Filename: colorfunc.hpp
 *        Created: 03/01/2018 11:46:11
 *    Description:  use uint32_t as color type
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

    constexpr uint32_t RGBA_F(double fR, double fG, double fB, double fA)
    {
        return RGBA(fR * 255, fG * 255, fB * 255, fA * 255);
    }

    enum ColorConst: uint32_t
    {
        RED     = RGBA(0XFF, 0X00, 0X00, 0X00),
        GREEN   = RGBA(0X00, 0XFF, 0X00, 0X00),
        BLUE    = RGBA(0X00, 0X00, 0XFF, 0X00),

        BLACK   = RGBA(0X00, 0X00, 0X00, 0X00),
        WHITE   = RGBA(0XFF, 0XFF, 0XFF, 0X00),
        YELLOW  = RGBA(0XFF, 0XFF, 0X00, 0X00),
        PURPLE  = RGBA(0XAB, 0X27, 0X4F, 0X00),
    };

    inline uint32_t RenderRGBA(uint32_t nDstColor, uint32_t nSrcColor)
    {
        auto nDstR = R(nDstColor);
        auto nDstG = G(nDstColor);
        auto nDstB = B(nDstColor);
        auto nDstA = A(nDstColor);

        auto nSrcR = R(nSrcColor);
        auto nSrcG = G(nSrcColor);
        auto nSrcB = B(nSrcColor);
        auto nSrcA = A(nSrcColor);

        nDstR = std::min<uint8_t>(255, std::lround((nSrcA / 255.0) * nSrcR + (1.0 - nSrcA / 255.0) * nDstR));
        nDstG = std::min<uint8_t>(255, std::lround((nSrcA / 255.0) * nSrcG + (1.0 - nSrcA / 255.0) * nDstG));
        nDstB = std::min<uint8_t>(255, std::lround((nSrcA / 255.0) * nSrcB + (1.0 - nSrcA / 255.0) * nDstB));
        nDstA = std::min<uint8_t>(255, std::lround((nSrcA *   1.0)         + (1.0 - nSrcA / 255.0) * nDstA));

        return RGBA(nDstR, nDstG, nDstB, nDstA);
    }

    inline bool String2Color(uint32_t *pColor, const char *szText)
    {
        if(szText == nullptr || std::strlen(szText) == 0){
            return false;
        }

        bool bFind = true;
        uint32_t nColor = 0;

        if(false
                || !std::strcmp(szText, "RED")
                || !std::strcmp(szText, "Red")
                || !std::strcmp(szText, "red")){
            bFind  = true;
            nColor = RED;
        }else if(false
                || !std::strcmp(szText, "GREEN")
                || !std::strcmp(szText, "Green")
                || !std::strcmp(szText, "green")){
            bFind  = true;
            nColor = GREEN;
        }else if(false
                || !std::strcmp(szText, "BLUE")
                || !std::strcmp(szText, "Blue")
                || !std::strcmp(szText, "blue")){
            bFind  = true;
            nColor = BLUE;
        }else if(false
                || !std::strcmp(szText, "YELLOW")
                || !std::strcmp(szText, "Yellow")
                || !std::strcmp(szText, "yellow")){
            bFind  = true;
            nColor = YELLOW;
        }else if(false
                || !std::strcmp(szText, "PURPLE")
                || !std::strcmp(szText, "Purple")
                || !std::strcmp(szText, "purple")){
            bFind  = true;
            nColor = PURPLE;
        }else{
            if(false
                    || (std::sscanf(szText, "0X%08X", &nColor)) == 1
                    || (std::sscanf(szText, "0X%08x", &nColor)) == 1){
                bFind = true;
            }
        }

        if(bFind && pColor){
            *pColor = nColor;
        }

        return bFind;
    }

#ifdef ENABLE_SDL_COLOR
    inline SDL_Color RGBA2Color(uint8_t nR, uint8_t nG, uint8_t nB, uint8_t nA)
    {
        SDL_Color stColor;
        stColor.r = nR;
        stColor.g = nG;
        stColor.b = nB;
        stColor.a = nA;
        return stColor;
    }

    inline SDL_Color RGBA2Color(uint32_t nColor)
    {
        return RGBA2Color(ColorFunc::R(nColor), ColorFunc::G(nColor), ColorFunc::B(nColor), ColorFunc::A(Color));
    }

    inline uint32_t Color2RGBA(const SDL_Color &rstColor)
    {
        return RGBA(rstColor.r, rstColor.g, rstColor.b, rstColor.a);
    }

    inline SDL_Color RenderColor(const SDL_Color &rstDstColor, const SDL_Color &rstSrcColor)
    {
        return RGBA2Color(Color2RGBA(rstDstColor), Color2RGBA(rstSrcColor));
    }

    inline bool String2Color(SDL_Color *pstColor, const char *szText)
    {
        uint32_t nColor;
        if(String2Color(&nColor, szText)){
            if(pstColor){
                *pstColor = RGBA2Color(nColor);
            }
            return true;
        }
        return false;
    }
#endif
}

#endif
