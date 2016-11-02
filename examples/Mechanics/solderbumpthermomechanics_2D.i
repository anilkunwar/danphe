# Adapted from the following tutorial
#https://github.com/idaholab/moose/blob/devel/modules/combined/examples/thermomechanics/circle_thermal_expansion_stress.i
# This example problem demonstrates coupling heat conduction with mechanics.
# A circular domain (2D section of a spherical solder bump) has as uniform heat source via
# electric current induced joule heating  that increases with time  and a 
# fixed temperature on the outer boundary, resulting in a temperature gradient.
# This results in heterogeneous thermal expansion, where it is pinned in the center.
# Looking at the hoop stress demonstrates why solder bumps  have radial cracks
# near the weakpoint i.e. at the interface of solder-substrate (location near IMC).
# that extend from the outer boundary to about halfway through the radius.
# The problem is run with length units of microns.
# Adapted from the following tutorial
#https://github.com/idaholab/moose/blob/devel/modules/combined/examples/thermomechanics/circle_thermal_expansion_stress.i
# Mesh in unv format is used as external mesh

[Mesh]
  #Circle mesh has a radius of 1000 units
  type = FileMesh
  #file = circle.unv
  #file = Mesh_1.unv
  file = Mesh_2.unv
  block_id = '1'
  block_name = 'circleface'
  #block_name = 'soldercircle_face'

  boundary_id = '2 3' # need to know that putting 1 here does not recognize the boundary
  boundary_name = 'outboundf inbound'
  #boundary_name = 'Edge_2outer Edge_1inner'
  #boundary_name = 'boundary_circle'
  uniform_refine = 1
[]

[Variables]
  # We solve for the temperature and the displacements
  [./T]
    initial_condition = 800
    scaling = 1e7
  [../]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[AuxVariables]
  [./radial_stress]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./hoop_stress]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  active = 'TensorMechanics htcond Q_function'
  [./htcond] #Heat conduction equation
    type = HeatConduction
    variable = T
  [../]
  [./TensorMechanics] #Action that creates equations for disp_x and disp_y
    displacements = 'disp_x disp_y'
  [../]
  [./Q_function] #Heat generation term
    type = BodyForce
    variable = T
    value = 1
    function = 0.8e-9*t
  [../]
[]

[AuxKernels]
  [./radial_stress] #Calculates radial stress from cartesian
    type = CylindricalRankTwoAux
    variable = radial_stress
    rank_two_tensor = stress
    index_j = 0
    index_i = 0
    center_point = '0 0 0'
  [../]
  [./hoop_stress] #Calculates hoop stress from cartesian
    type = CylindricalRankTwoAux
    variable = hoop_stress
    rank_two_tensor = stress
    index_j = 1
    index_i = 1
    center_point = '0 0 0'
  [../]
[]

[BCs]
  [./outer_T] #Temperature on outer edge is fixed at 800K
    type = PresetBC
    variable = T
    boundary = 2
    value = 800
  [../]
  [./outer_x] #Displacements in the x-direction are fixed in the center
    type = PresetBC
    variable = disp_x
    boundary = 3
    value = 0
  [../]
  [./outer_y] #Displacements in the y-direction are fixed in the center
    type = PresetBC
    variable = disp_y
    boundary = 3
    value = 0
  [../]
[]

[Materials]
  [./thcond] #Thermal conductivity is set to 5 W/mK
    type = GenericConstantMaterial
    block = 1
    prop_names = 'thermal_conductivity'
    prop_values = '5e-6'
  [../]
  [./iso_C] #Sets isotropic elastic constants
    type = ComputeElasticityTensor
    fill_method = symmetric_isotropic
    C_ijkl = '2.15e5 0.74e5'
    block = 1
  [../]
  [./srain] #We use small deformation mechanics
    type = ComputeSmallStrain
    displacements = 'disp_x disp_y'
    thermal_expansion_coeff = 1e-6
    temperature = T
    block = 1
  [../]
  [./stress] #We use linear elasticity
    type = ComputeLinearElasticStress
    block = 1
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2
  num_steps = 10
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'
  l_max_its = 30
  nl_max_its = 10
  nl_abs_tol = 1e-9
  l_tol = 1e-04
[]

[Outputs]
  exodus = true
  print_perf_log = true
[]
