# Two phase materials simulation
# Simulation of thermodiffusion of Cu in molten Sn.
#

[Mesh]
  type = GeneratedMesh
  dim = 2
  distribution = DEFAULT
  elem_type = QUAD4
  nx = 50
  ny = 50
  nz = 0
  xmin = 0
  xmax = 25
  ymin = 0
  ymax = 25
  zmin = 0
  zmax = 0
  uniform_refine = 2
[]

[GlobalParams]
  block = 0           # The generated mesh is used for all materials and kernels
[]

[Variables]
  [./c]   # Mole fraction of Cr (unitless)
    order = FIRST
    family = LAGRANGE
  [../]
  [./w]   # Chemical potential (eV/mol)
    order = FIRST
    family = LAGRANGE
  [../]
[]

[ICs]
  [./concentrationIC]   # 46.774 mol% Cr with variations
    type = RandomIC
    min = 0.018
    max = 0.013
    seed = 210
    variable = c
  [../]
[]

[BCs]
  [./Periodic]
    [./c_bcs]
      auto_direction = 'x y'
    [../]
  [../]
[]

[Kernels]
  [./w_dot]
    variable = w
    v = c
    type = CoupledTimeDerivative
  [../]
  [./coupled_res]
    variable = w
    type = SplitCHWRes
    mob_name = Mc
  [../]
  [./coupled_parsed]
    variable = c
    type = SplitCHParsed
    f_name = f_loc
    kappa_name = kappa_c
    w = w
  [../]
[]

[Materials]
  # d is a scaling factor that makes it easier for the solution to converge
  # without changing the results. It is defined in each of the materials and
  # must have the same value in each one.
  [./kappa]                  
    # Gradient energy coefficient (eV nm^2/mol)
    #currently not done and is in J --will be done together with thermaldiffusion term
    # Define constant value kappa_c 
    type = GenericFunctionMaterial
    prop_names = 'kappa_c'
    prop_values = '8.125e-16'
    #prop_values = '8.125e-10*6.24150934e+18*1e+09^2*1.0e-30'
                  # kappa_c*eV_J*nm_m^2*d
  [../]
  [./mobility]               # Mobility (nm^2 mol/eV/s)
    # Mohanty et. al. (JAP-2009, JNM-2011)
    type = DerivativeParsedMaterial
    f_name = Mc
    args = c
    constant_names =       'rho    B1    B2  nm_m   eV_J  d'
    constant_expressions = '-32.770969 -25.8186669 -3.29612744 1e+09  6.24150934e+18 1.0e-30'
    function = 'nm_m^2/eV_J/d*rho*c*(1-c)*(c*B2 + (1-c)*B1)'
    derivative_order = 1
    outputs = exodus
  [../]
  [./local_energy]
    # Defines the function for the local free energy density of liquid Cu-Sn system as given in the
    # problem, then converts units and adds scaling factor for its role in numerical convergence.
    # Park M.S. and Arroyave R., J. Elect. Mater., 39(2010), 2574-2582.
    type = DerivativeParsedMaterial
    f_name = f_loc
    args = c
    constant_names = 'Gcu   Gsn   R_T   L0   L1  L2 eV_J  d'
    constant_expressions = '-1.1083e+04 -2.8963e+04 4.34822e+03 -1.0487e+04
                             -1.8198e+04 1.05284e+04 6.24150934e+18 1.0e-30'
    function = 'eV_J*d*(Gcu*(c)+Gsn*(1-c)+R_T*((1-c)*log(1-c)+c*log(c))+c*(1-c)*
                (L0+L1*(1-2*(1-c))+L2*(1-4*(1-c)-4*(1-c)^2)))'
    derivative_order = 2
  [../]
  [./precipitate_indicator]  # Returns 1/625 if precipitate
      type = ParsedMaterial
      f_name = prec_indic
      args = c
      function = if(c>0.6,0.00016,0)
 [../]
[]

[Postprocessors]
  [./step_size]             # Size of the time step
    type = TimestepSize
  [../]
  [./iterations]            # Number of iterations needed to converge timestep
    type = NumNonlinearIterations
  [../]
  [./nodes]                 # Number of nodes in mesh
    type = NumNodes
  [../]
  [./evaluations]           # Cumulative residual calculations for simulation
    type = NumResidualEvaluations
  [../]
  [./precipitate_area]      # Fraction of surface devoted to precipitates
    type = ElementIntegralMaterialProperty
    mat_prop = prec_indic
  [../]
  [./active_time]           # Time computer spent on simulation
    type = RunTime
    time_type = active
  [../]
[]
[Preconditioning]
  [./coupled]
    type = SMP
    full = true
  [../]
[]

[Executioner]
   type = Transient
  solve_type = NEWTON
  l_max_its = 30
  l_tol = 1e-6
  nl_max_its = 50
  nl_abs_tol = 1e-9
  end_time = 604800   # 7 days
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type
                         -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm      31                  preonly
                         ilu          1'
  [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 10
    cutback_factor = 0.8
    growth_factor = 1.5
    optimal_iterations = 7
  [../]
  [./Adaptivity]
    coarsen_fraction = 0.1
    refine_fraction = 0.7
    max_h_level = 2
  [../]
[]

[Debug]
  show_var_residual_norms = true
[]

[Outputs]
  exodus = true
  console = true
  csv = true
  [./console]
    type = Console
    max_rows = 10
  [../]
[]
