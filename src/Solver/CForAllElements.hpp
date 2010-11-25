// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_Solver_CForAllElements_hpp
#define CF_Solver_CForAllElements_hpp

#include <boost/mpl/for_each.hpp>

#include "Common/OptionArray.hpp"

#include "Common/URI.hpp"

#include "Mesh/COperation.hpp"
#include "Mesh/SF/Types.hpp"

using namespace CF::Mesh;


/////////////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace Solver {

/////////////////////////////////////////////////////////////////////////////////////

/// Predicate class to test if the region contains a specific element type
template < typename TYPE >
struct IsComponentElementType
{
  bool operator()(const CElements& component)
  {
    return IsElementType<TYPE>()( component.element_type() );
  }
}; // IsElementRegion

/////////////////////////////////////////////////////////////////////////////////////

template<typename COp>
class CForAllElementsT : public COperation
{
public: // typedefs

  typedef boost::shared_ptr< CForAllElementsT > Ptr;
  typedef boost::shared_ptr< CForAllElementsT const > ConstPtr;
  friend struct Looper;

public: // functions

  /// Contructor
  /// @param name of the component
  CForAllElementsT ( const std::string& name ) :
    COperation(name),
    m_operation(new COp("operation"), Deleter<COp>())
  {
    BuildComponent<full>().build(this);
    m_property_list["Regions"].as_option().attach_trigger ( boost::bind ( &CForAllElementsT::trigger_Regions,   this ) );
  }

  void trigger_Regions()
  {
    std::vector<URI> vec; property("Regions").put_value(vec);
    BOOST_FOREACH(const CPath region_path, vec)
    {
      m_loop_regions.push_back(look_component_type<CRegion>(region_path));
    }
  }


  /// Virtual destructor
  virtual ~CForAllElementsT() {}

  /// Get the class name
  static std::string type_name () { return "CForAllElements"; }

  /// Configuration Options
  virtual void define_config_properties ()
  {
    std::vector< URI > dummy;
    options.add_option< OptionArrayT < URI > > ("Regions", "Regions to loop over", dummy)->mark_basic();
  }

  // functions specific to the CForAllElements component

  const COp& operation() const
  {
    return *m_operation;
  }

  COp& operation()
  {
    return *m_operation;
  }

  void execute(Uint index = 0)
  {
    // If the typename of the operation equals "COperation", then the virtual version
    // must have been called. In this case the operation must have been created as a
    // child component of "this_class", and should be set accordingly.
    if (m_operation->type_name() == "COperation")
    {
      BOOST_FOREACH(CRegion::Ptr& region, m_loop_regions)
        BOOST_FOREACH(CElements& elements, recursive_range_typed<CElements>(*region))
      {
        // Setup all child operations
        BOOST_FOREACH(COperation& operation, range_typed<COperation>(*this))
        {
          operation.set_loophelper( elements );
          const Uint elem_count = elements.elements_count();
          for ( Uint elem = 0; elem != elem_count; ++elem )
            operation.execute( elem );
        }
      }
    }
    else
    // Use now the templated version defined below
    {
      BOOST_FOREACH(CRegion::Ptr& region, m_loop_regions)
      {
        Looper looper(*this,*region);
        boost::mpl::for_each< SF::Types >(looper);
      }
    }
  }

private:

  /// Looper defines a functor taking the type that boost::mpl::for_each
  /// passes. It is the core of the looping mechanism.
  struct Looper
  {
  public: // functions

    /// Constructor
    Looper(CForAllElementsT& this_class, CRegion& region_in ) : region(region_in) , op(*this_class.m_operation) { }

    /// Operator
    template < typename SFType >
    void operator() ( SFType& T )
    {
      BOOST_FOREACH(CElements& elements, recursive_filtered_range_typed<CElements>(region,IsComponentElementType<SFType>()))
      {
        op.set_loophelper( elements );

        // loop on elements. Nothing may be virtual starting from here!
        const Uint elem_count = elements.elements_count();
        for ( Uint elem = 0; elem != elem_count; ++elem )
        {
          op.template executeT<SFType>( elem );
        }
      }
    }

  private: // data

    /// Region to loop on
    CRegion& region;

    /// Operation to perform
    COp& op;

  }; // Looper

private: // helper functions

  /// regists all the signals declared in this class
  virtual void define_signals () {}

private:

  /// Operation to perform
  typename COp::Ptr m_operation;

  /// Regions to loop over
  std::vector<CRegion::Ptr> m_loop_regions;

};

typedef CForAllElementsT<COperation> CForAllElements;

/////////////////////////////////////////////////////////////////////////////////////

} // Solver
} // CF

/////////////////////////////////////////////////////////////////////////////////////

#endif // CF_Solver_COperation_hpp
