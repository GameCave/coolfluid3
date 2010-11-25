// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_Common_CEnv_hpp
#define CF_Common_CEnv_hpp

////////////////////////////////////////////////////////////////////////////////

#include "Common/Component.hpp"

namespace CF {
namespace Common {

////////////////////////////////////////////////////////////////////////////////

  /// Component that defines global environment
  /// @author Quentin Gasper
  class Common_API CEnv : public Component {

  public: //typedefs

    typedef boost::shared_ptr<CEnv> Ptr;
    typedef boost::shared_ptr<CEnv const> ConstPtr;

  public: // functions

    /// Contructor
    /// @param name of the component
    CEnv ( const std::string& name );

    /// Virtual destructor
    virtual ~CEnv();

    /// Get the class name
    static std::string type_name () { return "CEnv"; }

    /// Configuration Options
    virtual void define_config_properties ();

  private: // helper functions

    /// regists all the signals declared in this class
    virtual void define_signals () {}

  private: // data

    /// this is a Env to the component
    /// using weak_ptr means it might become invalid so we should test for expire()
    boost::weak_ptr<Component> m_Env_component;

  }; // CEnv

////////////////////////////////////////////////////////////////////////////////

} // Common
} // CF

////////////////////////////////////////////////////////////////////////////////

#endif // CF_Common_CEnv_hpp
