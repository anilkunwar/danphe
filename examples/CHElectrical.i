#This is the modified form of the test given in 
#moose/modules/phase_field/tests/Parsed/SplitCHParsed_test.i
# This testcase accounts for the effect of Electromigration
# Effect of External Voltage

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 30
  ny = 30
  nz = 0
  xmin = 0
  xmax = 250
  ymin = 0
  ymax = 250
  zmin = 0
  zmax = 0
  elem_type = QUAD4
[]

[Variables]
  [./potential]
  [../]
  [./c]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = SmoothCircleIC
      x1 = 125.0
      y1 = 125.0
      radius = 60.0
      invalue = 1.0
      outvalue = 0.1
      int_width = 30.0
    [../]
  [../]
  [./w]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  #Cahn-Hilliard
  [./c_res]
    type = SplitCHParsed
    variable = c
    f_name = F
    kappa_name = kappa_c
    w = w
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = w
    mob_name = M
  [../]
  [./time]
    type = CoupledImplicitEuler
    variable = w
    v = c
  [../]
  #Potential
  [./electric_potential]
    type = ElectricPotential
    variable = potential
    conductivity = 73 # (W/m K) From NIST leadfree solder database
  [../]
  #CoupledPotentialInCH
  #Make the related Changes after writing a Kernel KernelName
  [./couple]
    type = CoupledPotential
    variable = c
    electric_potential = potential
  [../]
[]

[Materials]
  [./pfmobility]
    type = PFMobility
    block = 0
    kappa = 0.1
    mob = 1e-3
  [../]

  [./free_energy]
    type = DerivativeParsedMaterial
    block = 0
    f_name = F
    args = 'c'
    constant_names       = 'barr_height  cv_eq'
    constant_expressions = '0.1          1.0e-2'
    function = 16*barr_height*(c-cv_eq)^2*(1-cv_eq-c)^2
    derivative_order = 2
  [../]
[]

[Preconditioning]
  # active = ' '
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2

  solve_type = 'NEWTON'
  petsc_options_iname = -pc_type
  petsc_options_value = lu

  l_max_its = 30
  l_tol = 1.0e-4
  nl_rel_tol = 1.0e-10
  start_time = 0.0
  num_steps = 6

  dt = 10
[]

[Outputs]
  output_initial = true
  interval = 1
  exodus = true
  print_perf_log = true
[]
