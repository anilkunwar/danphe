#if we need to model concentration c, make e(-kd*t) =1 i.e. Neumann BC 
# and for this we need to decrease the value of a_1 from 10^-8 to 10^-10

[Mesh]
  #dim = 2
  type = FileMesh
  file = EyeMesh_1.unv
  #block_id = '1'
  block_name = 'VitreousVolume'
  #boundary_id = '1 2 3'
  boundary_name = 'Hyaloid Lens Retina'
 []

[Variables]
  [./pressure]
  [../]
  [./c]
    initial_condition = 300 # Start at room temperature
  [../]
[]

[AuxVariables]
  [./velocity_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./velocity_y]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./velocity_z]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./ocular_pressure]
    type = EyePressure
    variable = pressure
  [../]
[./drug_diffusion]
    type = MatDiffusion
    variable = c
    D_name = diff_coefficient
  [../]
  [./c_dot]
    type = TimeDerivative
    variable = c
  [../]
  [./drug_convection]
    type = SpeciesConvection
    variable = c
    darcy_pressure = pressure
  [../]
[]

[AuxKernels]
  [./velocity_x]
    type = DarcyVelocity
    variable = velocity_x
    component = x
    execute_on = timestep_end
    darcy_pressure = pressure
  [../]
  [./velocity_y]
    type = DarcyVelocity
    variable = velocity_y
    component = y
    execute_on = timestep_end
    darcy_pressure = pressure
  [../]
  [./velocity_z]
    type = DarcyVelocity
    variable = velocity_z
    component = z
    execute_on = timestep_end
    darcy_pressure = pressure
  [../]
[]

[Functions]
  [./inlet_function]
    type = ParsedFunction
    value = 2000*sin(0.466*pi*t) # Inlet signal from Fig. 3
  [../]
  [./outlet_function]
    type = ParsedFunction
    value = 2000*cos(0.466*pi*t) # Outlet signal from Fig. 3
  [../]
[]

[BCs]
  [./inlet]
    type = FunctionDirichletBC
    variable = pressure
    boundary = left
    function = inlet_function
  [../]
  [./outlet]
    type = FunctionDirichletBC
    variable = pressure
    boundary = right
    function = outlet_function
  [../]
  [./inlet_concentration]
    type = DirichletBC
    variable = c
    boundary = left
    value = 350 # (C)
  [../]
  [./outlet_concentration]
    type = HeatConductionOutflow
    variable = c
    boundary = right
  [../]
[]

[Materials]
  [./column]
    type = PackedColumn
    block = 0
    sphere_radius = 1
  [../]
[]

[Problem]
  type = FEProblem
  coord_type = RZ
  rz_coord_axis = X
[]

[Executioner]
  type = Transient
  num_steps = 300
  dt = 0.1
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]
