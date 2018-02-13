# Author: Anil Kunwar
# 2016 -10 - 23 Sunday
# Modelling the air cooling of liquid solders upto the melting point from an initial temperature of 275 degree C
# http://mooseframework.org/wiki/MooseSystems/Functions/
#length scale = 1.0e-3
#time scale = 1.0e-3
[Mesh]
  type = GeneratedMesh
  dim = 3
  elem_type = HEX8
  nx = 5 #10 #20 #25
  ny = 5 #10 #20 #25
  nz = 5 #10 #20 #25
  xmax = 1.000 #4.50e2 #Length of the solder material
  ymax = 0.100 #4.50e2 #height of the solder material
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

[AuxVariables]
  [./T_grad_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./T_grad_y]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./T_grad_z]
    order = CONSTANT
    family = MONOMIAL
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
  #[./HeatSink]
   # type = HeatSource
    #variable = T
    #value= '-2.0e-5'  #2*kcu*Acu*gradT in J/ms
   # function = heat_sink
    #block = '0' 
  #[../]
[]

[AuxKernels]
  [./T_grad_x]
    type = ThermalGradient
    variable = T_grad_x
    component = x
    execute_on = timestep_end
    #darcy_pressure = pressure
    # Kernel/Auxkernel variable = input file var name (syntax)
    T = T
  [../]
  [./T_grad_y]
    type = ThermalGradient
    variable = T_grad_y
    component = y
    execute_on = timestep_end
    #darcy_pressure = pressure
    # Kernel/Auxkernel variable = input file var name (syntax)
    T = T
  [../]
  [./T_grad_z]
    type = ThermalGradient
    variable = T_grad_z
    component = z
    execute_on = timestep_end
    #darcy_pressure = pressure
    # Kernel/Auxkernel variable = input file var name (syntax)
    T = T
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
    vals = '-1.8e-4'  # C/(ms ) 6.3e-5
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
[./amb_T]
    type = ParsedFunction
    value = 'a -b*t' # a in K and b in K/s, t is ms
    vars = 'a b'
    #vals = '583 0.1'  #these are in SI units
    #vals = '548 6.3e-5'  # K and K/ms
    #vals = '549.317 9.17e-5'  # K and K/ms
    vals = '549.317 4.18e-5'  # K and K/ms
[../]
#[./h_conv]
    #type = ParsedFunction
    #value = 'a*t^(-m)' # a in J/(ms mm^2 K) and m with base t is dimensionless
    #vars = 'a m'
    #vals = '11500 0.03'  #these are in SI units
    #vals = '11.5e-6 0.03'  # 11.5e-6 J/(ms mm^2 K) and 1
#[../]
[]

[BCs]
  #[./cu_boundary]
    #type = FunctionRobinBCS
    #variable = T
    #boundary = 'bottom'
    #function = 'convection_coefficient' #15 11e-6 #convection heat transfer coefficient in J/(ms mm^2 K)
     #beta = 298 #440.5
  #[../]
    # The BC can take a function name to use
  #[./all]
   # type = FunctionDirichletBC
   # variable = T
   # boundary = 'bottom'
    #function = bcs_function
  #[../]
    [./robin_boundaries]
    type = OnlyBetaFunctionRobinBCS
    variable = T
    boundary = ' front back top'
    betafunction = 'amb_T'
    alpha = 15e-9 #15 #convection heat transfer coefficient in J/(ms mm^2 K)
    #beta = 298 #440.5  # T_coolant in K
  [../]
  [./robin_boundaries1]
    type = OnlyBetaFunctionRobinBCS
    variable = T
    boundary = ' bottom' #'left right ' natural bc
    alpha = 11.56e-6 #15 #convection heat transfer coefficient in J/(ms mm^2 K)
    betafunction = 'amb_T'
    #beta = 523.15  # T_coolant in K
  [../]
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
    # The independent values are of Temperature(K) whereas dependent values are of tin thermal conductivity in W/ (m K)
    # Reference: kth = A + B*T , where A = 98.72 W/(m K) and B = -0.067  W/( m K^2 )  F. Meydaneri et al., Met. Mater. Int. (2012) 18: 77-85
    independent_vals = ' 506 510 515 520 525 530 535 540 545 550 555 560 565 570 575 580 583' 	
    dependent_vals = '64.82e-6 64.55e-6 64.21e-6 63.88e-6 63.54e-6  63.21e-6 62.87e-6 62.54e-6 62.20e-6 61.87e-6 61.53e-6 61.20e-6 60.86e-6 60.53e-6 60.19e-6 59.86e-6 59.65e-6'
    [../]
  [./cp]
    type = HeatCapacityMaterial
    T_grad = 'T'
    # The independent values are of Temperature(K) whereas dependent values are of specific heat capacity in J/(kg K)
    # The calculation is done in OpenCalphad cp = h.T via the method of compound energy formalism 
    independent_vals = '506 510 515 520 525 530 535 540 545 550 555 560 565 570 575 580 583' 	
    dependent_vals = ' 250.12 249.60 248.98 248.40 247.95 247.42 246.83 246.36 245.91 245.49 245.09 244.72 244.37 244.04 243.72 243.43 243.26'
  [../]
  [./rho]
    type = DensityMaterial
    T_grad = 'T'
    # The independent values are of Temperature(K) whereas dependent values are of tin density in kg/ m^3)
    # Reference: T. Gancarz et al., Int. J. Thermophysics (2013) 34: 250-266, rho = A + BT, where A = 7276.6, B = -0.613 
    independent_vals = ' 506 510 515 520 525 530 535 540 545 550 555 560 565 570 575 580 583' 	
    dependent_vals = '6965.80e-9 6963.97e-9 6960.90e-9 6957.84e-9 6954.77e-9 6951.70e-9 6948.64e-9 6945.58e-9 6942.51e-9 6939.45e-9 6936.38e-9 6933.32e-9 6930.25e-9 6927.18e-9 6924.12e-9 6921.06e-9 6919.22e-9'
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

