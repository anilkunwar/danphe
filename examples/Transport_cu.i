[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10 #25
  ny = 10 #25
  xmax = 2.0e-4 # Length of the solder material
  ymax = 2.0e-4 # height of the solder material
[]

[Variables]
  [./T]
  initial_condition = 300 # Start at room temperature
  scaling = 1.0e-6
  [../]
  [./c]
  initial_condition = 4.0e-4  #mol/cc
  scaling = 1.0e+4
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
    #kb = 1.38e-23
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
    value = 520.0 # (K) 
  [../]
  [./concentration_bottom]
    type = NeumannBC
    variable = c
    boundary = top
    value = 5.025e-6 # wt %/s (mol/cc^3/s)
    #value = 0.0  #wt %
  [../]
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
   prop_values = '35'  #in W/m K
   block = 0
  [../]
  [./diff_coeff]
   type = GenericFunctionMaterial
   prop_names = 'diff_coefficient'
   prop_values = '3.2e-9'  #in m^2/s
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
