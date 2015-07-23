[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 100
  ny = 100
  xmax = 4.5e5
  ymax = 4.5e5
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
    initial_condition = 0.68e-3
    scaling = 1.0e5
  [../]
[]

[ICs]
  [./c_IC]
    type = SmoothCircleIC
    x1 = 4.5e5 #1.25e5
    y1 = 1.0e5 #7.0e4
    radius = 5.0e4 #6.0e4
    invalue = 1.0
    outvalue = 0.1
    int_width = 1.0e4 #3.0e4
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
    type = SplitCHVoltage
    variable = w
    c = c
    volt = volt
    diff_name = D
    z_name = zeff
    #T = T
  [../]
  [./time]
    type = CoupledTimeDerivative
    variable = w
    v = c
  [../]
  [./electrical_potential]
    type = MatDiffusion
    variable = volt
    D_name = electrical_conductivity
  [../]
[]

[BCs]
  #active = 'Top_T Bottom_T'
  [./Bottom_T]
    type = DirichletBC
    variable = volt
    boundary = bottom
    value = 0.7e-3
  [../]

  [./Top_T]
    type = DirichletBC
    variable = volt
    boundary = top
    value = 7e-3
    # i = 4.5 A , l*b*h = 3 mm * 0.15 mm * 2 mm, resist = 23.34e-8 ohm m and T = 150 degree centigrade
  [../]
[]

[Materials]
  [./Copper]
    type = VoltPFParamsPolyFreeEnergy
    block = 0
    c = c
    volt = volt
    int_width = 1.0e4 #needs careful allocation of this value
    length_scale =1.0e-9   #1.0e-8
    time_scale = 1.0e-3 #1.0e-9
    D0 = 8.26e-3  #C.H.Ma and R.A. swalin, J.A.P (1962)
    Em = 0.68082      #0.71 # in eV, from P.H. S and M.O., 1976, J.A.P.
    Ef = 0.511    #1.28 # in eV, from P.H. S and M.O., 1976, J.A.P.
    # surface energy of tin 
    surface_energy = 0.5 #0.35 #J/m^2 ref. N. Moelans et al. , TMS, 2010
  [../]
  [./elcond]
    type = ParsedMaterial
    block = 0
    args = 'c'
    #function = 'if(c>0.7,1.0e-8,4.93e-8)'
    function = 'if(c>0.7,1.0e-9,1.8e-3)'
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

  num_steps = 360 #150 #100 #60
  #dt = 20.0
  dt = 1000
[]

[Problem]
  type = FEProblem
  coord_type = RZ
[]

[Outputs]
  output_initial = true
  interval = 1
  exodus = true
  print_perf_log = true
[]
