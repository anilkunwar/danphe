[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 60
  ny = 60
  xmax = 500
  ymax = 500
  elem_type = QUAD4
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
  [./volt]
    initial_condition = 1000
    scaling = 1.0e5
  [../]
[]

[ICs]
  [./c_IC]
    type = SmoothCircleIC
    x1 = 125.0
    y1 = 250.0
    radius = 60.0
    invalue = 1.0
    outvalue = 0.1
    int_width = 100.0
    variable = c
  [../]
[]

[Kernels]
  [./electrical_potential]
    type = MatDiffusion
    variable = volt
    D_name = electrical_conductivity
  [../]
  #[./electric_potential]
   # type = ElectricPotential
   # variable = volt
   # conductivity = 73 # (W/m K) From NIST leadfree solder database
 # [../]
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
  [./w_res_voltage]
    type = SplitCHVoltage
    variable = w
    c = c
    volt = volt
    diff_name = D
    z_name = zeff
    T = T
  [../]
  [./time]
    type = CoupledTimeDerivative
    variable = w
    v = c
  [../]
[]

[BCs]
  [./bottom_volt]
    type = DirichletBC
    variable = volt
    boundary = bottom
    value = 1024
  [../]

  [./top_volt]
    type = DirichletBC
    variable = volt
    boundary = top
    value = 1000
  [../]
[]

[Materials]
  [./Copper]
    type = VoltPFParamsPolyFreeEnergy
    block = 0
    c = c
    volt = volt
    T = 423 # K
    int_width = 60.0
    length_scale = 1.0e-9
    time_scale = 1.0e-9
    D0 = 3.1e-5 # m^2/s, from Brown1980
    Em = 0.71 # in eV, from Balluffi1978 Table 2
    Ef = 1.28 # in eV, from Balluffi1978 Table 2
    surface_energy = 0.708 # Total guess
  [../]
  [./elcond]
    type = ParsedMaterial
    block = 0
    args = 'c'
    function = 'if(c>0.7,1.0e-8,5.5e-8)'
    f_name = electrical_conductivity
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

  num_steps = 60
  dt = 20.0
[]

[Outputs]
  output_initial = true
  interval = 1
  exodus = true
  print_perf_log = true
[]
