# This output simulates 6 bubbles coalescing together on position dependant BCs

[Mesh]
  #type = GeneratedMesh
  #dim = 3
  #elem_type = HEX8
  #nx = 30 #400
  #ny = 30 #400
  #nz = 10 #400
  #xmax = 1.0e3
  #ymax = 1.5e3
  #zmax = 1.0e3
  type = GeneratedMesh
  dim = 2
  elem_type = QUAD4
  nx = 300 #400 300
  ny = 300 #400 300
  #nz = 150 #400
  xmax = 2.0e+4 # mm 1.0e3
  ymax = 2.0e+4 #1.0e2
  #zmax = 1.0e3
  #elem_type = QUAD4 #for 2D (i)
[]

[GlobalParams]
  polynomial_order = 8
[]

[Variables]
  [./c]
  scaling = 1.0e-05 #changed scale from 1.0e+0 to 1.0e+2
  [../]
  [./w]
    scaling = 1.0e-03 #changed scale from 1.0e+2 to 1.0e+4
  [../]
  [./volt]
    initial_condition = 0.0 #293.150
    scaling = 1.0e00  #changed scale from 1.0e+5 to 1.0e+7
  [../]
[]

[ICs]
  [./c1_IC]
    # specify a circle or sphere here
    type = SpecifiedSmoothCircleIC
    x_positions =  '7.0e+3' #'25.0e3 45.0e3 75.0e3'
    y_positions =   '1.0e+4 ' #'65.0e3 75.0e3 68.0e3'
    z_positions = '0.0 '     #'0.0 0.0 0.0' 750 if zmax = 1.5e3
    radii = '2.5e+3 '  #'5.0e3 15.0e3 8.0e3'
    invalue = 1.0
    outvalue = 0.0
    int_width = '0.25e+3 ' #'1.0e3 10.0e3 10.0e3'
    variable = c
  [../]

  #[./c2_IC]
  
  #[../]
 
[]

[AuxVariables]
  [./velocity_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./velocity_y]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./velocity_z]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]


[Functions]
  #active = 'bc_func'

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
  #[./bc_func]
    #type = ParsedFunction
    #value = '2.3e-3*t + 503'
    #value = '4e-3*t + 523'
    #vars = 'alpha'
    #vals = '16'
  #[../]
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
    type = MobilityVoltageDiffusion
    variable = w
    c = c
    volt = volt
    porefactor = 100.0 #1.5 the real pore factor is 3.0 v = distance * time and so timestep will be 100 times total time
    mobility_name = M  #because of R_thermal = McQ*(grad_T)/T
    z=2.0
    rho=2.027e+02 #ohm nm
    T_c=423.15
    kb=8.617343e-5 #eV/K
    #Q_name = Qstar
  [../]
  
  [./time]
    type = CoupledTimeDerivative
    variable = w
    v = c
  [../]
  [./HtCond]
    type = MatDiffusion
    variable = volt
    D_name = thermal_conductivity
  [../]

 [./source]
    type = MaskedBodyForce
    variable = w
    value = 0 #2.0e-7
    mask = mask
  [../]
[]

[AuxKernels]
  [./velocity_x]
    type = ThermalComponent
    variable = velocity_x
    component = x
    execute_on = timestep_end
    Temperature = volt
    D_name = D
    Q_asterik = 1.11 ##Q_asterik in eV
    kb = 8.617343e-5 # Boltzmann constant in eV/K
  [../]
  [./velocity_y]
    type = ThermalComponent
    variable = velocity_y
    component = y
    execute_on = timestep_end
    Temperature = volt
    D_name = D
    Q_asterik = 1.11 #Q_asterik in eV
    kb = 8.617343e-5 # Boltzmann constant in eV/K
  [../]
  [./velocity_z]
    type = ThermalComponent
    variable = velocity_z
    component = z
    execute_on = timestep_end
    Temperature = volt
    D_name = D
    Q_asterik = 1.11 ##Q_asterik in eV
    kb = 8.617343e-5 # Boltzmann constant in eV/K
    #outputs = exodus
  [../]
[]


[BCs]
  #active = 'WetNeumann NeumannW DirichletC'
  #active = 'all'
  #[./WetNeumann]
    #type = WettingAngleNeumannBC
    #variable = c
    #boundary = 'bottom' #wetting
    #value = 0.0 #.0871
  #[../]
  #[./NeumannW]
    #type = NeumannBC
    #variable = w
    #boundary ='bottom' # circle is wetting bc and bottom (w/o circle) is non-wetting bc
    #value = 0
  #[../]
 #[./DirichletC]
    #type = DirichletBC
    #variable = c 
    #boundary = 'bottom' #non-wetting
    #value = 0
 #[../]
  [./left_volt]
    type = DirichletBC
    variable = volt
    boundary = left
    value = 7.054e-02 #453.15 grad_phi = rho_c*j_c c-acis = 20.27*10^-6 V/um 4.054e-04 (0.07V DC with 0.5 A current)
  [../]
  [./Right_volt]
    #type = FunctionDirichletBC
    type = DirichletBC
    variable = volt
    boundary = 'right'
    value = 1.00e-5 #293.15
   # function = bc_func1
  [../]
  # The BC can take a function name to use
  #[./all]
    #type = FunctionDirichletBC
    #variable = T
    #boundary = 'bottom top left right'
    #function = bc_func
  #[../]
[]

[Materials]
  [./Tin] #Copper
    #type = TempPFParamsPolyFreeEnergy
    type = VoltPFParamsPolyFreeEnergy
    block = '0'
    c = c
    volt = volt # K
    int_width = 5.0e+2 #0.5 micron
    length_scale = 1.0e-9 #1.0e-9
    time_scale = 1.0e-9 #1.0e-9
    D0 = 2.935e-4 #4.99455e-5 4.99455e-5 1.498e-4   (ACTUAL VALUE =4.99455e-5) = porefactor=> 3 ... 8.26e-5   For tin, reference Fensham1950 D_v = N/n*D_atom
    # N/n = 231.087 at 423 K
    Em = 0.68082 # in eV, from Balluffi1978 Table 2 For tin, sun1976 JAP, vol. 47, Feb. 1976
    Ef =  0.4823 # 1.21 in eV, from Balluffi1978 Table 2 For tin, sun1976, JAP, vol. 47, Feb. 1976
    surface_energy = 0.709 # Total guess For tin references, vitos1998 and Sellers2010a
    #Use reference for Heat of Transport for vacancy as Q_v = 1.11 ev from Tucker2014, JAP and Schelling2014, JAP
  [../]
  [./thcond]
    type = ParsedMaterial
    block = '0'
    args = 'c'
    function = 'if(c>0.7,3.0e-10,4.93e-03)'  #3.163e-8 in liquid state sigma_caxis = 0.4933399 x 10^7 mho m^-1 (S/m) 3.0e-24=electrical-conductivity very low
    f_name = thermal_conductivity #self it is electrical conductivity = reciprocal of resistivity
    outputs = exodus
  [../]
  [./free_energy]
    type = PolynomialFreeEnergy
    #type = ScaledPolynomialFreeEnergy
    block = '0'
    c = c
    derivative_order = 2
  [../]

  [./mask]
    type = ParsedMaterial
    block = '0'
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
  #[Preconditioning]
  #[./SMP]
    #type = FDP
    #full = true
  #[../]
#[]
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

  num_steps =3600 #250 10000
  dt = 1.0e+08 #2.5e2 1.0e2 converges well for dt=100
[]

[Outputs]
  #output_initial = true
  interval = 1
  exodus = true
  print_perf_log = true
  #[./dbg]
  #show_var_residual_norms = true
  #type = DebugOutput
  #[../]
[]

[Debug]
  show_var_residual_norms = true
[]
