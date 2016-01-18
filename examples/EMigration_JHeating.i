
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
  initial_condition = 523.15 # Start at room temperature
  [../]
[]

[Kernels]
  [./HcTimeDerivative]
    type = HeatConductionTimeDerivative
    variable = T
    specific_heat_name=248.08 #J/kg K (at T=520-530) calculated using OC software
    density_name=7290.0 #kg/m^3
  [../]
  [./HeatConduction]
    type = HeatConduction
    variable = T
    diffusion_coefficient_name=k_th
    # by default "thermal_conductivity" set in Material Properties is read in the 
    # MATERIALs block is read by this kernel without the mentioning of the  coefficient.
  [../]
  [./JouleHeating]
    type = HeatSource
    variable = T
    #value=1.0
    function = volumetric_joule
  [../]
[]

[Functions]
[./volumetric_joule]
    type = ParsedFunction
    value = 'j*j/(elcond)'
    vars = 'j elcond '
    vals = '6.0e5 1.82e6'  #r_gb*k_d*s/v per second 1.82222e-2
[../]
[]

[BCs]
  [./bottom]
    type = DirichletBC
    variable = T
    boundary = bottom
    value = 523.15
  [../]
  #[./all]
    #type = NeumannBC
    #variable = T
    #boundary = 'left right top bottom'
    #value = 0
  #[../]
[]

[Materials]
  [./thermal_conductivity]
   type = GenericFunctionMaterial
   prop_names = 'k_th' #'thermal_conductivity' #
   prop_values = '73.2 '  #in W/m K
   block = 0
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 690
  dt = 6.0
  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]
