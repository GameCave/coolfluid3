// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "common/Core.hpp"
#include "common/FindComponents.hpp"
#include "common/Foreach.hpp"
#include "common/Log.hpp"
#include "common/OptionList.hpp"
#include "common/Signal.hpp"
#include "common/Builder.hpp"
#include "common/OptionT.hpp"
#include <common/EventHandler.hpp>

#include "math/LSS/System.hpp"

#include "mesh/Region.hpp"
#include "mesh/LagrangeP0/LibLagrangeP0.hpp"
#include "mesh/LagrangeP0/Quad.hpp"
#include "mesh/LagrangeP0/Line.hpp"

#include "HeatCouplingFlux.hpp"
#include "AdjacentCellToFace.hpp"
#include "Tags.hpp"

#include "solver/actions/Proto/ProtoAction.hpp"
#include "solver/actions/Proto/Expression.hpp"

namespace cf3
{

namespace UFEM
{

using namespace solver::actions::Proto;

common::ComponentBuilder < HeatCouplingFlux, common::ActionDirector, LibUFEM > HeatCouplingFlux_Builder;

HeatCouplingFlux::HeatCouplingFlux(const std::string& name): ActionDirector(name)
{
  options().add_option("lss", m_lss)
    .pretty_name("LSS")
    .description("The linear system for which the boundary condition is applied")
    .attach_trigger(boost::bind(&HeatCouplingFlux::trigger_setup, this))
    .link_to(&m_lss);

  options().add_option("gradient_region", m_gradient_region)
    .pretty_name("Gradient Region")
    .description("The (volume) region in which to calculate the temperature gradient")
    .attach_trigger(boost::bind(&HeatCouplingFlux::trigger_gradient_region, this))
    .link_to(&m_gradient_region);

  options().add_option("temperature_field_tag", UFEM::Tags::solution())
    .pretty_name("Temperature Field Tag")
    .description("Tag for the temperature field in the region where the gradient needs to be calculated")
    .attach_trigger(boost::bind(&HeatCouplingFlux::trigger_setup, this));
    
  // First compute the gradient
  create_static_component<ProtoAction>("ComputeGradient");
    // Then set the gradient on the boundary elements, and configure its tag
  Handle<AdjacentCellToFace> set_boundary_gradient = create_static_component<AdjacentCellToFace>("SetBoundaryGradient");
  set_boundary_gradient->options().configure_option("field_tag", std::string("gradient_field"));
  // Finally set the boundary condition
  create_static_component<ProtoAction>("NeumannHeatFlux");
}

HeatCouplingFlux::~HeatCouplingFlux()
{
}


void HeatCouplingFlux::on_regions_set()
{
  Handle<AdjacentCellToFace> set_boundary_gradient(get_child("SetBoundaryGradient"));
  if(is_not_null(set_boundary_gradient))
  {
    // Set the boundary regions of the component that copies the gradient from the volume to the boundary
    set_boundary_gradient->options().configure_option("regions", options()["regions"].value());
    // Set the regions on which to apply the Neumann BC
    get_child("NeumannHeatFlux")->options().configure_option("regions", options()["regions"].value());
  }
}

void HeatCouplingFlux::trigger_gradient_region()
{
  Handle<Component> compute_gradient = get_child("ComputeGradient");
  if(is_not_null(compute_gradient) && is_not_null(m_gradient_region))
  {
    compute_gradient->options().configure_option("regions", std::vector<common::URI>(1, m_gradient_region->uri()));
  }
}

void HeatCouplingFlux::trigger_setup()
{
  if(is_null(m_lss))
    return;

  // Get the tags for the used fields
  const std::string temperature_field_tag = options().option("temperature_field_tag").value<std::string>();

  Handle<ProtoAction> compute_gradient(get_child("ComputeGradient"));
  Handle<AdjacentCellToFace> set_boundary_gradient(get_child("SetBoundaryGradient"));
  Handle<ProtoAction> neumann_heat_flux(get_child("NeumannHeatFlux"));

  // Represents the temperature field, as calculated
  MeshTerm<0, ScalarField> T("Temperature", temperature_field_tag);
  // Represents the gradient of the temperature, to be stored in an (element based) field
  MeshTerm<1, VectorField> GradT("TemperatureGradient", "gradient_field", common::Core::instance().libraries().library<mesh::LagrangeP0::LibLagrangeP0>());

  // Expression to calculate the gradient, at the cell centroid:
  // nabla(T, center) is the shape function gradient matrix evaluated at the element center
  // T are the nodal values for the temperature
  compute_gradient->set_expression(elements_expression
  (
    boost::mpl::vector2<mesh::LagrangeP0::Quad, mesh::LagrangeP1::Quad2D>(),
    GradT = nabla(T, gauss_points_1)*nodal_values(T) // Calculate the gradient at the first gauss point, i.e. the cell center
  ));

  // Expression for the Neumann BC itself
  // Placeholder for the Linear System right-hand-side:
  SystemRHS rhs(options().option("lss"));
  neumann_heat_flux->set_expression(elements_expression
  (
    boost::mpl::vector2<mesh::LagrangeP0::Line, mesh::LagrangeP1::Line2D>(), // Valid for surface element types
    rhs(T) += integral<1>(transpose(N(T))*GradT*normal) // Classical Neumann condition formulation for finite elements
  ));

  // Raise an event to indicate that we added a variable (GradT)
  common::XML::SignalOptions options;
  common::SignalArgs f = options.create_frame();
  common::Core::instance().event_handler().raise_event("ufem_variables_added", f);
}

} // namespace UFEM

} // namespace cf3
