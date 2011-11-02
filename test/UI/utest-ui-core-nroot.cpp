// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "Test module for the UI NRoot class"

#include "UI/Core/NGeneric.hpp"
#include "UI/Core/NRoot.hpp"

#include "test/UI/CoreApplication.hpp"

using namespace cf3::common;
using namespace cf3::ui::core;

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( UICoreNBrowserSuite )

//////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( init )
{
  application();

  AssertionManager::instance().AssertionDumps = false;
  AssertionManager::instance().AssertionThrows = true;
  ExceptionManager::instance().ExceptionDumps = false;
  ExceptionManager::instance().ExceptionOutputs = false;
}

//////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( child_from_root )
{
  NRoot::Ptr root( new NRoot("Root") );
  NGeneric::Ptr node1(new NGeneric("Node1", "MyFirstType"));
  NGeneric::Ptr node2(new NGeneric("Node2", "MySecondType"));
  CNode::Ptr node;

  // 1. root has no child
  BOOST_CHECK_THROW( root->child_from_root(0), FailedAssertion );
  BOOST_CHECK_THROW( root->child_from_root(1), FailedAssertion );

  root->add_node(node1);
  root->add_node(node2);

  // 2. get the first child (node1)
  BOOST_REQUIRE_NO_THROW( node = root->child_from_root(0) );
  BOOST_CHECK_EQUAL( node->name(), std::string("Node1") );
  BOOST_CHECK_EQUAL( node->component_type().toStdString(), std::string("MyFirstType") );

  // 3. get the second child (node2)
  BOOST_REQUIRE_NO_THROW( node = root->child_from_root(1) );
  BOOST_CHECK_EQUAL( node->name(), std::string("Node2") );
  BOOST_CHECK_EQUAL( node->component_type().toStdString(), std::string("MySecondType") );
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()

