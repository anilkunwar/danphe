[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 300
  ny = 300
  xmax = 150.0 #1.5e5
  ymax = 150.0 #1.5e5
  zmax = 0.0
  #elem_type = QUAD
  elem_type = HEX8
[]

#[GlobalParams]
  #polynomial_order = 6
#[]

[Variables]
  [./c]
  [../]
  [./w]
    scaling = 1.0e2
  [../]
  [./T]
    initial_condition = 523.0
    scaling = 1.0e5
  [../]
[]

[ICs]
  #[./c1_IC]
    #type = SpecifiedSmoothCircleIC
    # variable = c
    #x_positions =  '25.0 75.0' #'25.0e3 45.0e3 75.0e3'
    #y_positions =   '30.00 80.0' #'65.0e3 75.0e3 68.0e3'
    #z_positions = '0.0 0.0 '     #'0.0 0.0 0.0'
    #radii = '20.0 45.0'  #'5.0e3 15.0e3 8.0e3'
    #invalue = 1.0
    #outvalue = -1.0
    #int_width = 0.1 0.1  
    ## '1.0e3 10.0e3 10.0e3'
    
  #[../]

  [./c]
    type = SpecifiedSmoothCircleIC
    variable = c
    x_positions = '25 75'
    y_positions = '30 80'
    z_positions = '0 0 '
    radii = '20 45'
    invalue = 1.0
    outvalue = -1.0
    int_width = '4 4'
[../]
 
[]

[Functions]
  active = 'bc_funcc1'

  # A ParsedFunction allows us to supply analytic expressions
  # directly in the input file
  #[./bc_funcT1]
   # type = ParsedFunction
   # value = '2.3e-3*t + 503'
    #value = '4e-3*t + 523'
    #vars = 'alpha'
    #vals = '16'
  #[../]
  # A ParsedFunction allows us to supply analytic expressions
  # directly in the input file
  #[./bc_funcT2]
    #type = ParsedFunction
    #value = '2.3e-3*t + 503'
    ##value = '4e-3*t + 523'
    ##vars = 'alpha'
    ##vals = '16'
  #[../]

  [./bc_funcc1]
    type = ParsedFunction
    value = sqrt(2*.1)/2*cos(pi/6)*(1-c^2)
    vars = c
  [../]
[]

[Kernels]
  [./c_res]
    type = SplitCHParsed
    variable = c
    kappa_name = kappa
    w = w
    f_name = F
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = w
    mob_name = M
  [../]
  [./w_res_soret]
    type = SoretDiffusion
    variable = w
    c = c
    T = T
    diff_name = D
    Q_name = Qstar
  [../]
  [./time]
    type = CoupledTimeDerivative
    variable = w
    v = c
  [../]
  [./HtCond]
    type = MatDiffusion
    variable = T
    D_name = thermal_conductivity
  [../]
[]

[BCs]
##########Temperature Variable T###################################################################3
  #[./Left_T]
  #  type = DirichletBC
  #  variable = T
  #  boundary = left
  #  value = 1000.0
  #[../]
  #active = 'all'
  
  #[./top]
   # type = FunctionDirichletBC
   # variable = T
   # boundary = 'top'
   # function = bc_func1
  #[../]

  

  # The BC can take a function name to use
  #[./all]
    #type = FunctionDirichletBC
    #variable = T
    #boundary = 'bottom top left right'
    #function = bc_func
  #[../]
#############Phase field variable c##################################################################4
  #active = 'left right top bottom'
  [./WetNeumann]
    type = FunctionNeumannBC
    variable = c
    boundary = 'bottom'
    function = bc_funcc1
  [../]
  [./NeumannW]
    type = NeumannBC
    variable = w
    boundary ='bottom'
    value = 0
  [../]
 [./DirichletC]
    type = DirichletBC
    variable = c
    boundary = 'left right top'
    value = 0
 [../]
[]

[Materials]
  #[./Copper]
    ##type = PFParamsPolyFreeEnergy
    #type = TempPFParamsPolyFreeEnergy
    #block = 0
    #c = c
    #T = T # K
    #int_width = 3.3e3
    #length_scale = 1.0e-9
    #time_scale = 1.0e-3
    #D0 = 3.275e-4 #8.26e-5   #8.26e-5 # m^2/s, from Brown1980
    #Em = 0.68082 # in eV, from Balluffi1978 Table 2
    #Ef = 0.511 # in eV, from Balluffi1978 Table 2
    #surface_energy = 0.56 # Total guess
  #[../]
   [./constant]
    type = GenericConstantMaterial
    prop_names  = 'M kappa_c'
    prop_values = '1.0 2.0'
    block = 0
[../]
  [./thcond]
    type = ParsedMaterial
    block = 0
    args = 'c'
    function = 'if(c>0.7,1e-10,3.163e-8)'
    f_name = thermal_conductivity #self
    outputs = exodus
  [../]
  [./free_energy]
    type = PolynomialFreeEnergy
    block = 0
    c = c
    derivative_order = 2
  [../]
[]

[Preconditioning]
  [./SMP]
   type = SMP
   full = true
  [../]
[]

#[Executioner]
  #type = Transient
  #scheme = 'bdf2'

  #solve_type = 'PJFNK'
 # petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  #petsc_options_value =  'asm         31   preonly   lu      1'

 # l_max_its = 30
  #l_tol = 1.0e-4
  #nl_max_its = 25
  #nl_rel_tol = 1.0e-9

  #num_steps = 50
  #dt = 1000.0
#[]

[Executioner]
  # Preconditioned JFNK (default)
  # petsc options = '-snes -ksp_monitor'
  type = Transient
  scheme = BDF2
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  l_max_its = 30
  l_tol = 1.0e-3
  nl_max_its = 10
  nl_rel_tol = 1.0e-9
  end_time = 2000.0
  [./TimeStepper]
    type = DT2
    dt = .4
    e_max = 6.0e2
    e_tol = 1.0e-1
    max_increase = 1.5
  [../]
[]

[Adaptivity]
  initial_steps = 3
  marker = errorfrac
  max_h_level = 3
  [./Indicators]
    [./error]
      stop_time = 2000.0
      type = GradientJumpIndicator
      variable = c
    [../]
  [../]
  [./Markers]
    [./errorfrac]
      stop_time = 2000.0
      type = ErrorFractionMarker
      refine = 0.5
      coarsen = 0.2
      indicator = error
    [../]
  [../]
[]

[Outputs]
  output_initial = true
  interval = 1
  exodus = true
  print_perf_log = true
[]
