[Mesh]
 type = GeneratedMesh
 dim = 2
 xmax = 1000
 ymax = 1000
 nx = 50
 ny = 50
[]
[GlobalParams]
 polynomial_order = 8
[]
[Variables]
 [./T]
 [../]
 [./c]
 [../]
 [./w]
     scaling = 1.0e3
 [../]
[]
[ICs]
 [./c_IC]
    type = SmoothCircleIC
    x1 = 175.0
    y1 = 0.0
    radius = 100
    invalue = 1.0
    outvalue = 0.01
    int_width = 100.0
    variable = c
 [../]
[]
#[AuxVariables]
#[./T]
#[../]
#[]
[Kernels]
 [./heat_conduction]
    type = HeatConduction
    variable = T
 [../]
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
[]
#[AuxKernels]
# [./Temp]
#    type = FunctionAux
#    variable = T
#    function = 1000.0+0.025*x
# [../]
#[]
[BCs]
  [./bottom_temperature]
    type = DirichletBC
    variable = T
    boundary = bottom
    value = 350 # (C)
  [../]
  [./top_temperature]
    type = DirichletBC
    variable = T
    boundary = top
    value = 348 # (C)
  [../]
[Materials]
 [./Copper]
    type = PFParamsPolyFreeEnergy
    block = 0
    c = c
    T = T # K
    int_width = 80.0
    length_scale = 1.0e-9
    time_scale = 1.0e-6
    D0 = 3.1e-5 # m^2/s, from Brown1980
    Em = 0.71 # in eV, from Balluffi1978 Table 2
    Ef = 1.28 # in eV, from Balluffi1978 Table 2
    surface_energy = 0.708 # Total guess
 [../]
 [./Cu_cond]
    type = GenericConstantMaterial
    block = 0
    prop_names = thermal_conductivity
    prop_values = 73 # K: (W/m*K) from wikipedia @296K
  [../]
 [./free_energy]
    type = PolynomialFreeEnergy
    block = 0
    c = c
    outputs = exodus
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
    solve_type = 'NEWTON'
    petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
    petsc_options_value = 'asm 31 preonly lu 1'
    l_max_its = 10
    l_tol = 1.0e-4
    nl_max_its = 25
    nl_rel_tol = 1.0e-9
    start_time = 0.0
    num_steps = 20
    dt = 3
[]
[Outputs]
    output_initial = true
    exodus = true
    print_perf_log = true
[]
