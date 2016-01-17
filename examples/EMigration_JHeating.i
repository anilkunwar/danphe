
[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10 #25
  ny = 10 #25
  xmax = 4.5e-4 #Length of the solder material
  ymax = 4.5e-4 #height of the solder material
[]


[Variables]
  [./T]
  [../]
[]

[Kernels]
  [./HcTimeDerivative]
    type = HeatConductionTimeDerivative
    variable = T
    specific_heat_name=1.0
    density_name=1.0
  [../]
  [./HeatConduction]
    type = HeatConduction
    variable = T
    thermal_conductivity=1.0
  [../]
  [./JouleHeating]
    type = HeatSource
    #value=1.0
    function = volumetric_joule
  [../]
[]

[Functions]
[./volumetric_joule]
    type = ParsedFunction
    value = 'j*j/(elcond)'
    vars = 'j elcond '
    #value = 'a_1*exp(-kd*t)'
    #vars = 'a_1 kd'
    vals = '6.0e5 1.82e6'  #r_gb*k_d*s/v per second 1.82222e-2
    #r_gb is the ratio of grain boundary channel to the whole area considered 0.1
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = T
    boundary = bottom
    value = 523.15
  [../]
  #[./right]
    #type = DirichletBC
    #variable = T
    #boundary = right
    #value = 1
  #[../]
[]

[Executioner]
  type = Transient
  num_steps = 69
  dt = 60.0
  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]
