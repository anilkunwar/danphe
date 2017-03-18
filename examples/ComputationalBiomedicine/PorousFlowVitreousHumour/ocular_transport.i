#if we need to model concentration c, make e(-kd*t) =1 i.e. Neumann BC 
# and for this we need to decrease the value of a_1 from 10^-8 to 10^-10

[Mesh]
  #dim = 2
  type = FileMesh
  file = EyeMesh_1.unv
  #block_id = '1'
  block_name = 'VitreousVolume'
  #boundary_id = '1 2 3'
  boundary_name = 'Hyaloid Lens Retina'
 []

[Variables]
  [./T]
  initial_condition = 300 # Start at room temperature
  scaling = 1.0e-10
  #scaling = 1.0e+10
  [../]
  [./c]
  initial_condition = 5.0e-4 #4.0e-4  #mol/cc
  scaling = 1.0e+15
  [../]
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
  [./imc_width]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./HtCond]
    type = MatDiffusion
    variable = T
    D_name = thermal_conductivity
  [../]
  [./Cu_diffusion]
    type = MatDiffusion
    variable = c
    D_name = diff_coefficient
  [../]
  [./c_dot]
    type = TimeDerivative
    variable = c
  [../]
  [./thermal_transport]
    type = ThermalConvection
    variable = c
    Temperature = T
    D_name = diff_coefficient
    Q_asterik = 1.112e+4
    kb = 8.31
  [../]
  #[./heatsource]
  #  type = SinkTerm
   # block = 0
   # function = volumetric_c
   # variable = c
  #[../]
 [./imc_formation]
    type = ReactionTerm
   # block = 0
     variable = c
     Temperature = T
     c_sat = conc_sat
     k_chem = 1.9e-6 #k_chem *s/v in 1/s
  #[../]
[]

[AuxKernels]
  [./velocity_x]
    type = ThermalComponent
    variable = velocity_x
    component = x
    execute_on = timestep_end
    Temperature = T
    D_name = diff_coefficient
    Q_asterik = 1.112e+4
    kb = 8.31
  [../]
  [./velocity_y]
    type = ThermalComponent
    variable = velocity_y
    component = y
    execute_on = timestep_end
    Temperature = T
    D_name = diff_coefficient
    Q_asterik = 1.112e+4
    kb = 8.31
  [../]
  [./velocity_z]
    type = ThermalComponent
    variable = velocity_z
    component = z
    execute_on = timestep_end
    Temperature = T
    D_name = diff_coefficient
    Q_asterik = 1.112e+4
    kb = 8.31
    #outputs = exodus
  [../]
  [./variable_time_integration]
    type = VariableTimeIntegrationAux
    variable = imc_width
    variable_to_integrate = c
    #Temperature = T
    #D_name = diff_coefficient
    #Q_asterik = 1.112e+4
    #kb = 8.31 #use universal gas constant
    coefficient = 1.29777e-6 #times omega
  [../]
[]

[Functions]
  active = 'bc_func'
  # A ParsedFunction allows us to supply analytic expressions
  # directly in the input file
  [./bc_func]
    type = ParsedFunction
    #value = 'cs*kd*exp(-kd*t)'
    #vars = 'cs kd'
    value = 'a_1*exp(-kd*t)'
    vars = 'a_1 kd'
    vals = '1.5e-8 9.1e-3'  #k_d*s/v per second
  [../]
[]
[BCs]
  [./bottom]
    type = DirichletBC
    variable = T
    boundary = bottom
    value = 523.0 # (K) 
  [../]
  [./top]
    type = DirichletBC
    variable = T
    boundary = top
    value = 520.0 #520.0 # (K) 
  [../]
  #[./concentration_bottom]
    #type = RobinBCS
    #variable = c
    #boundary = bottom
    #alpha = 4.1e-2 # wt %/s (mol/cc^3/s)
    #beta = 1.81e-3 # c_s saturated solubility or concentration
   #value = 0.0  #wt %
  #[../]
  [./concentration_bottom]
    type = FunctionNeumannBC
    #type = NeumannBC
    variable = c
    boundary = bottom
    #value = 1.5e-9 #1.76e-10 #2.77e-6 #wt %/s (mol/cc^3/s) kd=8.2e-6 m/s del c =6.6e-3 mol/cc sgb =1.14e-2
    #value = 0.0  #wt %
    function = bc_func
  [../]
  #[./concentration_bottom]
   # type = DirichletBC
    #variable = c
    #boundary = bottom
    #value = 1.7e-3 #520.0 # (K) 
  #[../]
  [./concentration_top]
    type = NeumannBC
    variable = c
    boundary = top
    #value = 1.09e+3 # (mol/m^3)
    value = 0.0 #wt %
  [../]
[]

[Materials]
  [./th_cond]
   type = GenericFunctionMaterial
   prop_names = 'thermal_conductivity'
   prop_values = '7.3e+1'  #in W/m K
   block = 0
  [../]
  [./diff_coeff]
   type = GenericFunctionMaterial
   prop_names = 'diff_coefficient conc_sat'
   prop_values = '6.44e-9 6.8e-3'  #in m^2/s mol/cc 3.2e-9 6.8e-3
   block = 0
  [../]

[]

[Preconditioning]
 [./coupled]
    type = SMP
    full = true
  [../]
  #[./SMP]
   # type = FDP
   # full = true
  #[../]
[]

[Executioner]
  type = Transient
  num_steps = 69
  dt = 60.0
  solve_type = PJFNK
  #petsc_options = '-snes_monitor -ksp_monitor_true_residual -snes_converged_reason -ksp_converged_reason'
    petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre    boomeramg      101'
  #petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type
                         #-sub_pc_type -pc_asm_overlap'
  #petsc_options_value = 'asm      31                  preonly
                        # ilu          1'
[]

[Debug]
  show_var_residual_norms = true
[]

[Outputs]
  exodus = true
[]
