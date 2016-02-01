[Mesh]
 file = Mesh_2.unv
 block_id = '1 2 3'
 block_name = 'Face_1left  Face_2middle Face_3right'
 boundary_id = '1 2 3 4 5 6 7 8 9 10'
 #boundary_name = 4 5 6 7 8 9 10
 boundary_name = 'lc_left lc_bottom lc_top sn_bottom lcsn_interface rcsn_interface sn_top rc_bottom rc_top rc_right'
[]


[Variables]
  [./T]
  order = FIRST
  family = LAGRANGE
  initial_condition = 623.15
  #scaling = 1.0e-5
  block = '1 2 3'
  [../]
[]

[Kernels]
  [./HcTimeDerivative]
    type = HeatConductionTimeDerivative
    variable = T
    #specific_heat_name=248.08 #J/kg K (at T=520-530) calculated using OC software
    #density_name=7290.0 #kg/m^3
    block = '1 2 3'
    #block = 'Face_1left  Face_3right'
  [../]

  [./HeatConduction]
    type = HeatConduction
    variable = T
    diffusion_coefficient_name=k_th
    # by default "thermal_conductivity" set in Material Properties is read in the 
    # MATERIALs block is read by this kernel without the mentioning of the  coefficient.
    block = '1 2 3'
  [../]

  [./JouleHeating]
    type = HeatSource
    variable = T
    #value=1.0
    function = volumetric_joule
    block = '2' 
  [../]

[]

[Functions]
[./volumetric_joule]
    type = ParsedFunction
    value = 'j*j/(elcond)'
    vars = 'j elcond'
    vals = '6.0e5 1.82e6'  #r_gb*k_d*s/v per second 1.82222e-2
[../]
[]

[BCs]

  [./cu_substrate_boundaries]
    type = RobinBCS
    variable = T
    boundary = '4 5 6 11 12 13'
    alpha = 0.0375 #ratio of convection heat transfer coefficient in W/m^2 K to conduction W/m K -->> h/k
    beta = 523.15  # T_coolant in K
  [../]

  [./mid_tin_boundaries]
    type = RobinBCS
    variable = T
    boundary = ' 7 10'
    alpha = 0.2049 #convection heat transfer coefficient in W/m^2 K
    beta = 523.15  # T_coolant in K
  [../]
[]

[Materials]
  [./thermal_conductivity_Cu]
   type = GenericFunctionMaterial
   prop_names = 'k_th' #'thermal_conductivity' #
   prop_values = '400'  #in W/m K
   block = 'Face_1left'
   #block = 'Face_1left  Face_3right'
  [../]

  [./specific_heat_cu]
   type = GenericFunctionMaterial
   prop_names = 'specific_heat' #'thermal_conductivity' #
   prop_values = '385'  #in W/m K
   block = '1'
  [../]

  [./density_cu]
   type = GenericFunctionMaterial
   prop_names = 'density' #'thermal_conductivity' #
   prop_values = '8900.0'  #in W/m K
   block = '1'
  [../]
  
  [./thermal_conductivity_tin]
   type = GenericFunctionMaterial
   prop_names = 'k_th' #'thermal_conductivity' #
   prop_values = '73.2'  #in W/m K
   block = '2'
  [../]

  [./specific_heat_tin]
   type = GenericFunctionMaterial
   prop_names = 'specific_heat' #'thermal_conductivity' #
   prop_values = '248.08'  #in W/m K
   block = '2'
  [../]

  [./density_tin]
   type = GenericFunctionMaterial
   prop_names = 'density' #'thermal_conductivity' #
   prop_values = '7290.0'  #in W/m K
   block = '2'
  [../]


[]

[Executioner]
  type = Transient
  num_steps = 360
  dt = 10.0
  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'
  #petsc_options_iname = '-pc_type -pc_hypre_type'
  #petsc_options_value = 'hypre boomeramg'
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
 petsc_options_value = 'asm      31                  preonly       lu           2'
[]

[Outputs]
  exodus = true
[]
