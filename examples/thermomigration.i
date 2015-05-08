# Steady-state test for the ThermoDiffusion kernel.
#
# This test applies a constant temperature gradient to drive thermo-diffusion
# in the variable u. At steady state, the thermo-diffusion is balanced by
# diffusion due to Fick's Law, so the total flux is
#
#   J = -D ( grad(u) - ( Qstar u / R ) grad(1/T) )
#
# If there are no fluxes at the boundaries, then there is no background flux and
# these two terms must balance each other everywhere:
#
#   grad(u) = ( Qstar u / R ) grad(1/T)
#
# The dx can be eliminated to give
#
#   d(ln u) / d(1/T) = Qstar / R
#
# This can be solved to give the profile for u as a function of temperature:
#
#   u = A exp( Qstar / R T )
#
# Here, we are using simple heat conduction with Dirichlet boundaries on 0 <= x <= 1
# to give a linear profile for temperature: T = x + 1. We also need to apply one
# boundary condition on u, which is u(x=0) = 1. These conditions give:
#
#   u = exp( -(Qstar/R) (x/(x+1)) )
#
# This analytical result is tracked by the aux variable "correct_u".

[Mesh]
  type = GeneratedMesh
  dim = 2
  xmin = 0
  xmax = 8.72e-4
  ymin = 0
  ymax = 3.1e-4
  nx = 50
  ny = 40
[]

[Variables]
  [./conc]
    initial_condition = 0.007 #Concentration in the tin-solder
  [../]
  [./T]
    initial_condition = 616.5  #Average Temperature in Tin Medium
  [../]
[]

[Kernels]
  [./soret]
    type = ThermoDiffusion
    variable = conc
    temp = T
    gas_constant = 8.31
  [../]
  [./diffC]
    type = CoefDiffusion
    variable = conc
    coef = 3.2e-9
  [../]

  # Heat diffusion gives a linear temperature profile to drive the Soret diffusion.
  [./diffT]
    type = CoefDiffusion
    variable = T
    coef = 3.2e-9
  [../]
  [./diffusion_time_derivative]
    type = TimeDerivative
    variable = conc
    coef = 1
  [../]
[]

[BCs]
  [./bottom]
    type = NeumannBC
    variable = conc
    boundary = bottom
    value = 1.3e-2
  [../]
  #[./top]
  #  type = DirichletBC
  #  variable = conc
  #  boundary = top
  #  value = 1.3e-3
  #[../]


  [./bottomt]
    type = DirichletBC
    variable = T
    boundary = bottom
    value = 623
  [../]
  [./topt]
    type = DirichletBC
    variable = T
    boundary = top
    value = 610
  [../]
[]

[Materials]
  [./fake_material]
     type = GenericConstantMaterial
     block = 0
     prop_names = 'mass_diffusivity heat_of_transport'
     prop_values = '3.2e-9 20e3'
  [../]
[]

[Functions]
  # The correct (analytical) result for the solution of u.
  [./concentration_profile]
    type = ParsedFunction
    value = 'exp(-x/(x+1))'
  [../]
[]

[AuxVariables]
  [./correct_u]
  [../]
[]

[AuxKernels]
  # This kernel is just being used to plot the correct result for u.
  [./copy_from_function]
    type = FunctionAux
    variable = correct_u
    function = concentration_profile
  [../]
[]

[Executioner]
  type = Transient
  start_time = 0
  end_time = 1000
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
  [./TimeStepper]
    type = TransientHalf
    ratio = 0.5
    min_dt = 0.01
    dt = 1
  [../]
[]

[Outputs]
  output_initial = true
  exodus = true
  print_linear_residuals = true
[]
