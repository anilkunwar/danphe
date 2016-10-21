#length scale = 1.0e-3
#time scale = 1.0e-3
[Mesh]
  type = GeneratedMesh
  dim = 3
  elem_type = HEX8
  nx = 10 #25
  ny = 10 #25
  nz = 10 #25
  xmax = 0.45 #4.50e2 #Length of the solder material
  ymax = 9000 #4.50e2 #height of the solder material
  zmax = 0.1  #9.00e5 #thickness of the solder material
[]

[Variables]
  [./T]
   order = FIRST
  family = LAGRANGE
  initial_condition = 523.15
  #scaling = 1.0e20
  [../]
[]

#[ICs]
  #[./T_IC]
    #type = FunctionIC
    #variable = T
    #function = volumetric_joule
  #[../]
#[]

[Kernels]
#active = 'HeatDiff'
  [./HeatDiff]
    type = HeatConduction
    variable = T
  [../]
  [./HeatTdot]
    type = HeatConductionTimeDerivative
    variable = T
  [../]
  [./JouleHeating]
    type = HeatSource
    variable = T
    #value=1.0
    function = volumetric_joule
    #block = '2' 
  [../]
  #[./HeatSink]
   # type = HeatSource
    #variable = T
    #value= '-7.0e-6'  #2*kcu*Acu*gradT in J/s
    #function = heat_sink
    #block = '0' 
  #[../]
[]

[Functions]
[./volumetric_joule]
    type = ParsedFunction
    value = 'j*j/(elcond)'
    vars = 'j elcond'
    #vals = '6.0e5 1.82e6'  #these are in SI units
    vals = '5.6e-1 1.82e3'  # C/(ms mm^2) and 1/{mho mm)
[../]
[./convection_coefficient]
    type = ParsedFunction
    value = 'a*t^(-m)' # a in J/(ms mm^2 K) and m with base t is dimensionless
    vars = 'a m'
    #vals = '11500 0.03'  #these are in SI units
    vals = '11.5e-6 0.03'  # J/(ms mm^2 K) and 1
[../]
[]

[BCs]
  [./leftright]
    type = FunctionRobinBCS
    variable = T
    boundary = 'left right'
    function = convection_coefficient #15 11e-6 #convection heat transfer coefficient in J/(ms mm^2 K)
    beta = 523.15
  [../]
  [./robin_boundaries]
    type = RobinBCS
    variable = T
    boundary = ' front back bottom top'
    alpha = 15e-9 #15 #convection heat transfer coefficient in J/(ms mm^2 K)
    beta = 523.15  # T_coolant in K
  [../]
[]

[Materials]
  [./k]
    type = GenericConstantMaterial
    prop_names = 'thermal_conductivity'
    prop_values = '7.32e-5' #Sn in J/(mm msec K)
    block = 0
  [../]
  [./cp]
    type = GenericConstantMaterial
    prop_names = 'specific_heat'
    prop_values = '248.08' #Sn in J/(kg C)
    block = 0
  [../]
  [./rho]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '7.29e-6' #Sn in kg/(mm^3)
    block = 0
  [../]
[]

[Executioner]
  type = Transient


  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



  petsc_options_iname = '-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  petsc_options_value = '201                hypre    boomeramg      8'


  line_search = 'none'
  trans_ss_check = true
  ss_check_tol = 1.0e-08


  [./Predictor]
    type = SimplePredictor
    scale = 1.0
  [../]

# controls for linear iterations
  l_max_its = 100
  l_tol = 1e-2

# controls for nonlinear iterations
  nl_max_its = 15
  nl_abs_tol = 1e-10

# time control
  start_time = 0.0
  dt = 1000.0
  #end_time = 3600.0
  num_steps = 3600.0 #5000
[]

[Outputs]
  exodus = true
  print_perf_log = true
[]

