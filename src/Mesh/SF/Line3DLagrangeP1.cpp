// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "Common/ObjectProvider.hpp"

#include "SFLib.hpp"
#include "Line3DLagrangeP1.hpp"

namespace CF {
namespace Mesh {
namespace SF {

////////////////////////////////////////////////////////////////////////////////

Common::ObjectProvider < Line3DLagrangeP1,
                         ElementType,
                         SFLib >
aLine3DLagrangeP1_Provider ( "Line3DLagrangeP1" );

////////////////////////////////////////////////////////////////////////////////

Line3DLagrangeP1::Line3DLagrangeP1() : Line3D()
{
  m_nb_nodes = nb_nodes;
  m_order = order;
}

std::string Line3DLagrangeP1::getElementTypeName() const
{
  return "Line3DLagrangeP1";
}

Real Line3DLagrangeP1::computeVolume(const NodesT& coord) const
{
  return 0;
}
	
bool Line3DLagrangeP1::is_coord_in_element(const RealVector& coord, const NodesT& nodes) const
{
	return false;
}

const CF::Mesh::ElementType::FaceConnectivity& Line3DLagrangeP1::face_connectivity() const
{
  static FaceConnectivity connectivity;
  return connectivity;
}

const CF::Mesh::ElementType& Line3DLagrangeP1::face_type(const CF::Uint face) const
{
  static const Line3DLagrangeP1 facetype;
  return facetype;
}



} // namespace SF
} // namespace Mesh
} // namespace CF
