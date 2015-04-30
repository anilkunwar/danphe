[Mesh]
   type = GeneratedMesh
  dim = 2
  nx = 30
  ny = 30
  nz = 0
  xmin = 0
  xmax = 250
  ymin = 0
  ymax = 250
  zmin = 0
  zmax = 0
  elem_type = QUAD4
[]

[Variables]
  [./potential]
    [./InitialCondition]
      type = SmoothCircleIC
      x1 = 125.0
      y1 = 125.0
      radius = 60.0
      invalue = 1.0
      outvalue = 0
      int_width = 30.0
    [../]
  [../]
[]

[AuxVariables]
  [./currentdensity_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./currentdensity_y]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./currentdensity_z]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./electric_potential]
    type = ElectricPotential
    variable = potential
    conductivity = 73 # (W/m K) From NIST leadfree solder database
  [../]
[]

[AuxKernels]
  [./currentdensity_x]
    type = CurrentDensity
    variable = currentdensity_x
    component = x
    execute_on = timestep_end
    electric_potential = potential
  [../]
  [./currentdensity_y]
    type = CurrentDensity
    variable = currentdensity_y
    component = y
    execute_on = timestep_end
    electric_potential = potential
  [../]
  [./currentdensity_z]
    type = CurrentDensity
    variable = currentdensity_z
    component = z
    execute_on = timestep_end
    electric_potential = potential
  [../]
[]

[BCs]
  [./inlet]
    type = DirichletBC
    variable = potential
    boundary = left
    value = 0.7 # (V) 
  [../]
  [./outlet]
    type = DirichletBC
    variable = potential
    boundary = right
    value = 0 # (V) 
  [../]
[]

[Materials]
  [./tin]
    type = TinSheet
    block = 0
  [../]
[]

[Problem]
  type = FEProblem
  coord_type = RZ
  rz_coord_axis = X
[]

[Executioner]
  type = Steady
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  output_initial = true
  exodus = true
  print_perf_log = true
  print_linear_residuals = true
[]
