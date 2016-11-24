[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 200 #400
  ny = 200 #400
  xmax = 1.5e5
  ymax = 1.5e5
  elem_type = QUAD
[]

[GlobalParams]
  polynomial_order = 6
[]

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
  [./c1_IC]
    type = SpecifiedSmoothCircleIC
    x_positions =  '55.0e3 75.0e3 100.0e3' #'25.0e3 45.0e3 75.0e3'
    y_positions =   '67.0e3 72.0e3 70.0e3' #'65.0e3 75.0e3 68.0e3'
    z_positions = '0.0 0.0 0.0'     #'0.0 0.0 0.0'
    radii = '7.0e3 12.0e3 9.0e3'  #'5.0e3 15.0e3 8.0e3'
    invalue = 1.0
    outvalue = 0.0
    int_width = '3.3e3 3.3e3 3.3e3' #'1.0e3 10.0e3 10.0e3'
    variable = c
  [../]

  #[./c2_IC]
  
  #[../]
 
[]

[Functions]
  active = 'bc_func'

  # A ParsedFunction allows us to supply analytic expressions
  # directly in the input file
  #[./bc_func1]
   # type = ParsedFunction
   # value = '2.3e-4*t + 503'
    #value = '4e-3*t + 523'
    #vars = 'alpha'
    #vals = '16'
  #[../]
  # A ParsedFunction allows us to supply analytic expressions
  # directly in the input file
  [./bc_func]
    type = ParsedFunction
    value = '2.3e-3*t + 503'
    #value = '4e-3*t + 523'
    #vars = 'alpha'
    #vals = '16'
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

 [./source]
    type = MaskedBodyForce
    variable = w
    value = 5.0e-7
    mask = mask
  [../]
[]

[BCs]
  #[./Left_T]
  #  type = DirichletBC
  #  variable = T
  #  boundary = left
  #  value = 1000.0
  #[../]
  active = 'all'
  
  #[./top]
   # type = FunctionDirichletBC
   # variable = T
   # boundary = 'top'
   # function = bc_func1
  #[../]

  

  # The BC can take a function name to use
  [./all]
    type = FunctionDirichletBC
    variable = T
    boundary = 'bottom top left right'
    function = bc_func
  [../]
[]

[Materials]
  [./Copper]
    #type = PFParamsPolyFreeEnergy
    type = TempPFParamsPolyFreeEnergy
    block = 0
    c = c
    T = T # K
    int_width = 3.3e3
    length_scale = 1.0e-9
    time_scale = 1.0e-3
    D0 = 3.275e-4 #8.26e-5   #8.26e-5 # m^2/s, from Brown1980
    Em = 0.68082 # in eV, from Balluffi1978 Table 2
    Ef = 0.511 # in eV, from Balluffi1978 Table 2
    surface_energy = 0.56 # Total guess
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
    #type = ScaledPolynomialFreeEnergy
    block = 0
    c = c
    derivative_order = 2
  [../]

  [./mask]
    type = ParsedMaterial
    block = 0
    function = if(c>0.5,0,1)
    f_name = mask
    args = c
    outputs = exodus
  [../]
[]

[Preconditioning]
  [./SMP]
   type = SMP
   full = true
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'bdf2'

  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value =  'asm         31   preonly   lu      1'

  l_max_its = 30
  l_tol = 1.0e-4
  nl_max_its = 25
  nl_rel_tol = 1.0e-9

  num_steps = 240
  dt = 2.5e2
[]

[Outputs]
  output_initial = true
  interval = 1
  exodus = true
  print_perf_log = true
[]
