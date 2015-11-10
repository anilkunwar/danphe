[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 50
  ny = 50
  xmax = 0.2 # Length of the solder material
  ymax = 0.2 # height of the solder material
[]

[Variables]
  [./T]
  initial_condition = 300 # Start at room temperature
  [../]
  [./c]
  #scaling = 1.0e+4
  [../]
[]

[Kernels]
  [./HtCond]
    type = MatDiffusion
    variable = T
    D_name = thermal_conductivity
  [../]
  [./Cu_diffusion]
    type = MatDiffusion
    variable = c
    D_name = diff_coefficient
  [../]
  [./c_dot]
    type = TimeDerivative
    variable = c
  [../]
  [./thermal_transport]
    type = ThermalConvection
    variable = c
    Temperature = T
    Q_asterik = 1.112e+4
    #kb = 1.38e-23
  [../]
[]

[BCs]
  [./bottom]
    type = DirichletBC
    variable = T
    boundary = bottom
    value = 523.0 # (K) 
  [../]
  [./top]
    type = DirichletBC
    variable = T
    boundary = top
    value = 520.0 # (K) 
  [../]
  [./concentration]
    type = DirichletBC
    variable = c
    boundary = bottom
    #value = 1.09e+3 # (mol/m^3)
    value = 1.81  #wt %
  [../]
[]

[Materials]
  [./th_cond]
   type = GenericFunctionMaterial
   prop_names = 'thermal_conductivity'
   prop_values = '4.0e-2'  #in W/mm K
   block = 0
  [../]
  [./diff_coeff]
   type = GenericFunctionMaterial
   prop_names = 'diff_coefficient'
   prop_values = '3.2'  #in mm^2/s
   block = 0
  [../]
[]


[Executioner]
  type = Transient
  num_steps = 69
  dt = 60
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Debug]
  show_var_residual_norms = true
[]

[Outputs]
  exodus = true
[]
