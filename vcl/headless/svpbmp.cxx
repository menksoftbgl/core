/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */

#ifndef IOS

#include <sal/config.h>

#include <cstring>

#include "headless/svpbmp.hxx"
#include "headless/svpgdi.hxx"
#include "headless/svpinst.hxx"

#include <basegfx/vector/b2ivector.hxx>
#include <basegfx/range/b2ibox.hxx>

#include <vcl/salbtype.hxx>
#include <vcl/bitmap.hxx>

using namespace basegfx;

SvpSalBitmap::~SvpSalBitmap()
{
    Destroy();
}

BitmapBuffer* ImplCreateDIB(
    const Size& rSize,
    sal_uInt16 nBitCount,
    const BitmapPalette& rPal)
{
    assert(
          (nBitCount ==  0
        || nBitCount ==  1
        || nBitCount ==  4
        || nBitCount ==  8
        || nBitCount == 16
        || nBitCount == 24
        || nBitCount == 32)
        && "Unsupported BitCount!");

    BitmapBuffer* pDIB = nullptr;

    if( rSize.Width() && rSize.Height() )
    {
        try
        {
            pDIB = new BitmapBuffer;
        }
        catch (const std::bad_alloc&)
        {
            pDIB = nullptr;
        }

        if( pDIB )
        {
            const sal_uInt16 nColors = ( nBitCount <= 8 ) ? ( 1 << nBitCount ) : 0;

            switch (nBitCount)
            {
                case 1:
                    pDIB->mnFormat = ScanlineFormat::N1BitLsbPal;
                    break;
                case 4:
                    pDIB->mnFormat = ScanlineFormat::N4BitMsnPal;
                    break;
                case 8:
                    pDIB->mnFormat = ScanlineFormat::N8BitPal;
                    break;
                case 16:
                {
#ifdef OSL_BIGENDIAN
                    pDIB->mnFormat= ScanlineFormat::N16BitTcMsbMask;
#else
                    pDIB->mnFormat= ScanlineFormat::N16BitTcLsbMask;
#endif
                    ColorMaskElement aRedMask(0xf800);
                    aRedMask.CalcMaskShift();
                    ColorMaskElement aGreenMask(0x07e0);
                    aGreenMask.CalcMaskShift();
                    ColorMaskElement aBlueMask(0x001f);
                    aBlueMask.CalcMaskShift();
                    pDIB->maColorMask = ColorMask(aRedMask, aGreenMask, aBlueMask);
                    break;
                }
                default:
                    nBitCount = 32;
                    SAL_FALLTHROUGH;
                case 32:
                {
                    pDIB->mnFormat = SVP_CAIRO_FORMAT;
                    break;
                }
            }

            pDIB->mnFormat |= ScanlineFormat::TopDown;
            pDIB->mnWidth = rSize.Width();
            pDIB->mnHeight = rSize.Height();
            pDIB->mnScanlineSize = AlignedWidth4Bytes( pDIB->mnWidth * nBitCount );
            pDIB->mnBitCount = nBitCount;

            if( nColors )
            {
                pDIB->maPalette = rPal;
                pDIB->maPalette.SetEntryCount( nColors );
            }

            try
            {
                size_t size = pDIB->mnScanlineSize * pDIB->mnHeight;
                pDIB->mpBits = new sal_uInt8[size];
                std::memset(pDIB->mpBits, 0, size);
            }
            catch (const std::bad_alloc&)
            {
                delete pDIB;
                pDIB = nullptr;
            }
        }
    }
    else
        pDIB = nullptr;

    return pDIB;
}

bool SvpSalBitmap::Create(BitmapBuffer *pBuf)
{
    Destroy();
    mpDIB = pBuf;
    return mpDIB != nullptr;
}

bool SvpSalBitmap::Create(const Size& rSize, sal_uInt16 nBitCount, const BitmapPalette& rPal)
{
    Destroy();
    mpDIB = ImplCreateDIB( rSize, nBitCount, rPal );
    return mpDIB != nullptr;
}

bool SvpSalBitmap::Create(const SalBitmap& rBmp)
{
    Destroy();

    const SvpSalBitmap& rSalBmp = static_cast<const SvpSalBitmap&>(rBmp);

    if (rSalBmp.mpDIB)
    {
        // TODO: reference counting...
        mpDIB = new BitmapBuffer( *rSalBmp.mpDIB );
        // TODO: get rid of this when BitmapBuffer gets copy constructor
        try
        {
            size_t size = mpDIB->mnScanlineSize * mpDIB->mnHeight;
            mpDIB->mpBits = new sal_uInt8[size];
            std::memcpy(mpDIB->mpBits, rSalBmp.mpDIB->mpBits, size);
        }
        catch (const std::bad_alloc&)
        {
            delete mpDIB;
            mpDIB = nullptr;
        }
    }

    return !rSalBmp.mpDIB || (mpDIB != nullptr);
}

bool SvpSalBitmap::Create( const SalBitmap& /*rSalBmp*/,
                           SalGraphics* /*pGraphics*/ )
{
    return false;
}

bool SvpSalBitmap::Create( const SalBitmap& /*rSalBmp*/,
                           sal_uInt16 /*nNewBitCount*/ )
{
    return false;
}

bool SvpSalBitmap::Create( const css::uno::Reference< css::rendering::XBitmapCanvas >& /*xBitmapCanvas*/, Size& /*rSize*/, bool /*bMask*/ )
{
    return false;
}

void SvpSalBitmap::Destroy()
{
    if (mpDIB)
    {
        delete[] mpDIB->mpBits;
        delete mpDIB;
        mpDIB = nullptr;
    }
}

Size SvpSalBitmap::GetSize() const
{
    Size aSize;

    if (mpDIB)
    {
        aSize.Width() = mpDIB->mnWidth;
        aSize.Height() = mpDIB->mnHeight;
    }

    return aSize;
}

sal_uInt16 SvpSalBitmap::GetBitCount() const
{
    sal_uInt16 nBitCount;

    if (mpDIB)
        nBitCount = mpDIB->mnBitCount;
    else
        nBitCount = 0;

    return nBitCount;
}

BitmapBuffer* SvpSalBitmap::AcquireBuffer(BitmapAccessMode)
{
    return mpDIB;
}

void SvpSalBitmap::ReleaseBuffer(BitmapBuffer*, BitmapAccessMode)
{
}

bool SvpSalBitmap::GetSystemData( BitmapSystemData& )
{
    return false;
}

bool SvpSalBitmap::Scale( const double& /*rScaleX*/, const double& /*rScaleY*/, BmpScaleFlag /*nScaleFlag*/ )
{
    return false;
}

bool SvpSalBitmap::Replace( const ::Color& /*rSearchColor*/, const ::Color& /*rReplaceColor*/, sal_uLong /*nTol*/ )
{
    return false;
}

#endif

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
