# Anil Kunwar
# 2016 -10 - 23 Sunday
# Modelling the air cooling of liquid solders upto the melting point
# http://mooseframework.org/wiki/MooseSystems/Functions/
#length scale = 1.0e-3
#time scale = 1.0e-3
[Mesh]
  type = GeneratedMesh
  dim = 3
  elem_type = HEX8
  nx = 20 #25
  ny = 20 #25
  nz = 20 #25
  xmax = 1.000 #4.50e2 #Length of the solder material
  ymax = 0.070 #4.50e2 #height of the solder material
  zmax = 0.100  #9.00e5 #thickness of the solder material
[]

[Variables]
  [./T]
   order = FIRST
  family = LAGRANGE
  initial_condition = 548.15
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
  #[./JouleHeating]
    #type = HeatSource
    #variable = T
    #value=1.0
    #function = volumetric_joule
    #block = '2' 
  #[../]
  [./HeatSink]
    type = HeatSource
    variable = T
    #value= '-7.0e-6'  #2*kcu*Acu*gradT in J/ms
    function = heat_sink
    #block = '0' 
  [../]
[]

[Functions]
#[./volumetric_joule]
    #type = ParsedFunction
    #value = 'j*j/(elcond)'
    #vars = 'j elcond'
    #vals = '6.0e5 1.82e6'  #these are in SI units
    #vals = '3.0e-1 1.82e3'  # C/(ms mm^2) and 1/{mho mm)
#[../]
[./heat_sink]
    type = ParsedFunction
    value = 'k*t'
    vars = 'k'
    #vals = '6.0e5 1.82e6'  #these are in SI units
    vals = '-8.0e-5'  # C/(ms mm^2) and 1/{mho mm)
[../]
[./convection_coefficient]
    type = ParsedFunction
    value = 'a*t^(-m)' # a in J/(ms mm^2 K) and m with base t is dimensionless
    vars = 'a m'
    #vals = '11500 0.03'  #these are in SI units
    vals = '11.5e-6 0.03'  # J/(ms mm^2 K) and 1
[../]
  #[./bcs_function]
    #type = ParsedFunction
    #value = 'a -b*t' # a in K and b in K/s, t is s
    #vars = 'a b'
   #vals = '583 1.0e-4'  # K and K/ms
  #[../]
#[./amb_T]
    #type = ParsedFunction
    #value = 'a -b*t' # a in K and b in K/s, t is ms
    #vars = 'a b'
    #vals = '583 0.1'  #these are in SI units
    #vals = '583 1.0e-4'  # K and K/ms
#[../]
#[./h_conv]
    #type = ParsedFunction
    #value = 'a*t^(-m)' # a in J/(ms mm^2 K) and m with base t is dimensionless
    #vars = 'a m'
    #vals = '11500 0.03'  #these are in SI units
    #vals = '11.5e-6 0.03'  # 11.5e-6 J/(ms mm^2 K) and 1
#[../]
[]

[BCs]
  [./cu_boundary]
    type = FunctionRobinBCS
    variable = T
    boundary = 'bottom'
    function = 'convection_coefficient' #15 11e-6 #convection heat transfer coefficient in J/(ms mm^2 K)
     beta = 298 #440.5
  [../]
    # The BC can take a function name to use
  #[./all]
   # type = FunctionDirichletBC
   # variable = T
   # boundary = 'bottom'
    #function = bcs_function
  #[../]
   [./robin_boundaries]
    type = RobinBCS
    variable = T
    boundary = ' front back top'
    alpha = 15e-9 #15 #convection heat transfer coefficient in J/(ms mm^2 K)
    beta = 298 #440.5  # T_coolant in K
  [../]
  #[./robin_boundaries1]
    #type = OnlyBetaFunctionRobinBCS
    #variable = T
    #boundary = ' front back left top'
    #alpha = 15e-9 #15 #convection heat transfer coefficient in J/(ms mm^2 K)
    #betafunction = 'amb_T'
    #beta = 523.15  # T_coolant in K
  #[../]
   # [./robin_boundaries2]
    #type = BetaFunctionRobinBCS
    #variable = T
    #boundary = 'bottom'
    #alpha = 7.66e-7 #15 #convection heat transfer coefficient in J/(ms mm^2 K)
    #betafunction = 'amb_T'
    #hfunction = 'h_conv'
    #beta = 523.15  # T_coolant in K
  #[../]
[]

[Materials]
    [./k]
    type = ThermalConductivityMaterial
    T_grad = 'T'
    independent_vals = '495 500 505 510 515 520 525 530 535 540 545 550 555' 	
    dependent_vals = '66.72 66.66 66.61 66.55 66.50 66.45 66.39 66.34 66.29 66.23 66.18 66.12'
    # The independent values are of Temperature(K) whereas dependent values are of tin thermal conductivity in W/ (m K)
    # Reference: kth = A + B*T , where kth = A + B*T , where A = 72.065 W/(m K) and B = -0.0108  W/( m K^2 )
    # N. Aksoz et al. J. Electron. Mater. (2013) 42:3573-3581
    # F. Meydaneri et al., Met. Mater. Int. (2012) 18: 77-85
    [../]
    [./cp]
     type = HeatCapacityMaterial
    T_grad = 'T'
    # The independent values are of Temperature(K) whereas dependent values are of specific heat capacity in J/(kg K)
    # The calculation is done in OpenCalphad cp = h.T via the method of compound energy formalism 
    independent_vals = '495 500 505 510 515 520 525 530 535 540 545 550 555' 	
    dependent_vals = ' 250.9 250.5 249.97 249.37 248.78 248.23 247.70 247.21 246.74 246.29 245.87 245.48' 
    #independent_vals = '506 510 515 520 525 530 535 540 545 550 555 560 565 570 575 580 583' 	
    #dependent_vals = ' 250.12 249.60 248.98 248.40 247.95 247.42 246.83 246.36 245.91 245.49 245.09 244.72 244.37 244.04 243.72 243.43 243.26' 
    #  249.51 249.76 
   [../]
   [./rho]
    type = DensityMaterial
    T_grad = 'T'
    # The independent values are of Temperature(K) whereas dependent values are of tin density in kg/ m^3)
    # Reference: T. Gancarz et al., Int. J. Thermophysics (2011) 32: 1210-1233 , rho = A + BT, where A = 7379.3, B = -0.591 
    independent_vals = ' 495 500 505 510 515 520 525 530 535 540 545 550' 	
    dependent_vals = '7086.75 7083.79 7080.84 7080.84 7077.89 7074.93 7071.98 7069.02 7066.07 7063.11 7060.15 7057.20'   
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
  dt = 1000 #000.0
  #end_time = 3600.0
  num_steps = 2000 #3600.0 #5000
[]

[Outputs]
  exodus = true
  print_perf_log = true
[]

