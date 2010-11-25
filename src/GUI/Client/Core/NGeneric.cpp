// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include <QFileIconProvider>

#include "GUI/Client/Core/NGeneric.hpp"

using namespace CF::GUI::ClientCore;

NGeneric::NGeneric(const QString & name, const QString & type) :
    CNode(name, type, GENERIC_NODE)
{
  BuildComponent<full>().build(this);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

QString NGeneric::toolTip() const
{
  return this->getComponentType();
}

