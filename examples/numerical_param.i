[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 400
  ny = 400 
  xmax = 4.15e5
  ymax = 9.00e5
  elem_type = QUAD
[]

[GlobalParams]
  polynomial_order = 8
[]

[Variables]
  [./c]
  [../]
  [./w]
    scaling = 1.0e2
  [../]
  [./T]
    initial_condition = 622.0
    scaling = 1.0e5
  [../]
[]

[ICs]
  [./c_IC]
    type = SmoothCircleIC
    x1 = 2.25e5
    y1 = 4.0e4
    radius = 1.5e4
    invalue = 1.0
    outvalue = 0.1
    int_width = 1.5e4
    variable = c
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
  [./Top_T]
    type = DirichletBC
    variable = T
    boundary = top
    value = 622.4
  [../]

  [./Bottom_T]
    type = DirichletBC
    variable = T
    boundary = bottom
    value = 623.1
  [../]
[]

[Materials]
  [./Copper]
    type = TempPFParamsPolyFreeEnergy
    block = 0
    c = c
    T = T # K
    int_width = 1.5e4
    length_scale = 1.0e-9
    time_scale = 1.0
    D0 = 9.9e-9 # m^2/s, from Brown1980
    Em = 0.68082 # in eV, from Balluffi1978 Table 2
    Ef = 0.511 # in eV, from Balluffi1978 Table 2
    surface_energy = 0.55 # Total guess
    outputs = exodus
  [../]
  [./thcond]
    type = ParsedMaterial
    block = 0
    args = 'c'
    function = 'if(c>0.7,0.1e-9,5.7e-8)'
    f_name = thermal_conductivity
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

  num_steps = 36
  dt = 100
[]

[Outputs]
  output_initial = true
  interval = 1
  exodus = true
  print_perf_log = true
[]
