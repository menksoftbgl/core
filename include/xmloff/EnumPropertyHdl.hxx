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

#ifndef INCLUDED_XMLOFF_ENUMPROPERTYHDL_HXX
#define INCLUDED_XMLOFF_ENUMPROPERTYHDL_HXX

#include <xmloff/xmlprhdl.hxx>
#include <com/sun/star/uno/Type.h>
#include <xmloff/xmlement.hxx>

/**
    PropertyHandler for a generic xml enumeration type:
*/
class XMLOFF_DLLPUBLIC XMLEnumPropertyHdl : public XMLPropertyHandler
{
private:
    const SvXMLEnumMapEntry* mpEnumMap;
    const css::uno::Type & mrType;

public:
    XMLEnumPropertyHdl( const SvXMLEnumMapEntry* pEnumMap, const css::uno::Type & rType ) : mpEnumMap( pEnumMap ), mrType( rType ) {}
    virtual ~XMLEnumPropertyHdl();

    virtual bool importXML( const OUString& rStrImpValue, css::uno::Any& rValue, const SvXMLUnitConverter& rUnitConverter ) const override;
    virtual bool exportXML( OUString& rStrExpValue, const css::uno::Any& rValue, const SvXMLUnitConverter& rUnitConverter ) const override;
};

#endif // INCLUDED_XMLOFF_ENUMPROPERTYHDL_HXX

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
