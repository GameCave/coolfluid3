// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_Common_ConfigObject_hpp
#define CF_Common_ConfigObject_hpp

/////////////////////////////////////////////////////////////////////////////////////

#include "Common/PropertyList.hpp"

namespace CF {
namespace Common {

/////////////////////////////////////////////////////////////////////////////////////

  /// Class defines a object that has options that can be dynamically
  /// configured by the end-user at run-time.
  /// @author Tiago Quintino
  class Common_API ConfigObject  {

  public:

    /// Defines the properties which are configurable (options)
    virtual void define_config_properties() {};

    /// configures all the options on this class
    void configure ( XmlNode& node );

    /// sets a link to the option
    template < typename TYPE >
        void link_to_parameter ( const std::string& pname, TYPE* par )
    {
      cf_assert(m_property_list.check(pname));

      m_property_list[pname].as_option().link_to(par);
    }

    /// get the pointer to the option
    const Property & property(const std::string& optname ) const;

    /// Configure one option, and trigger its actions
    /// @param [in] optname  The option name
    /// @param [in] val      The new value assigned to the option
    void configure_property(const std::string& optname, const boost::any& val)
    {
      m_property_list.configure_property(optname,val);
    }

    bool check_property(const std::string & prop_name)
    {
      return m_property_list.check(prop_name);
    }

  protected:

    /// storage of the option list
    PropertyList m_property_list;

  }; // ConfigObject

/////////////////////////////////////////////////////////////////////////////////////

} // Common
} // CF

////////////////////////////////////////////////////////////////////////////////

#endif // CF_Common_ConfigObject_hpp
