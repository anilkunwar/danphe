# Simulation of Cu transport across solder under the influence of voltage difference
# Variables are Cu concentration (c) and potential distribution (phi)
#if we need to model concentration c, make e(-kd*t) =1 i.e. Neumann BC 
# and for this we need to decrease the value of a_1 from 10^-8 to 10^-10

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10 #25
  ny = 10 #25
  xmax = 4.5e-4 #Length of the solder material
  ymax = 4.5e-4 #height of the solder material
[]

#[GlobalParams]
#variable = c
#current_density = '6.5e6 0 0'
#[]

[Variables]
  [./c]
  initial_condition = 4.2e-4 #4.0e-4  #mol/cc
  scaling = 1.0e+00
  [../]
  [./phi]
  #initial_condition = 4.2e-4 #4.0e-4  #mol/cc
  scaling = 1.0e-02
  [../]
[]


[Kernels]
  [./Cu_diffusion]
    type = MatDiffusion
    variable = c
    D_name = diff_coefficient
  [../]
  [./c_dot]
    type = TimeDerivative
    variable = c
  [../]
  [./electrical_transport]
    type = NernstPlanckConvection
    variable = c
    M_name = mob_coefficient
    Voltage = phi
  [../]
  #[./heatsource]
  #  type = SinkTerm
   # block = 0
   # function = volumetric_c
   # variable = c
  #[../]
 #[./imc_formation]
  #  type = ReactionTerm
   # block = 0
   #  variable = c
     #Temperature = T
    # c_sat = conc_sat
     #k_chem = 1.9e-6 #k_chem *s/v in 1/s
  #[../]
  [./Voltage_distribution]
    type = MatDiffusion
    variable = phi
    D_name = electric_cond
  [../]
[]


[Functions]
  active = 'bc_func'
  # A ParsedFunction allows us to supply analytic expressions
  # directly in the input file
  [./bc_func]
    type = ParsedFunction
    value = 'cs*(1.0 -exp(-kd*t))'
    vars = 'cs kd'
    #value = 'a_1*exp(-kd*t)'
    #vars = 'a_1 kd'
    vals = '1.485e-3 1.8222e-3'  #r_gb*k_d*s/v per second 1.82222e-2
    #r_gb is the ratio of grain boundary channel to the whole area considered 0.1
  [../]
[]

[BCs]
  #[./concentration_bottom]
    #type = RobinBCS
    #variable = c
    #boundary = bottom
    #alpha = 4.1e-2 # wt %/s (mol/cc^3/s)
    #beta = 1.485e-3 # c_s saturated solubility or concentration
   #value = 0.0  #wt %
  #[../]
  [./concentration_bottom]
    type = FunctionDirichletBC
    #type = NeumannBC
    variable = c
    boundary = right
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
    boundary = left
    #value = 1.09e+3 # (mol/m^3)
    value = 0.0 #wt %
  [../]
  [./voltage_right]
   type = DirichletBC
    variable = phi
    boundary = right
    value = 0.0 #520.0 # (K) 
  [../]
  [./voltage_left]
   type = DirichletBC
    variable = phi
    boundary = left
    value = 0.7e-04 #520.0 # (K) 
  [../]
[]

[Materials]
  [./diff_coeff]
   type = GenericFunctionMaterial
   prop_names = 'diff_coefficient conc_sat mob_coefficient electric_cond '
   prop_values = '6.44e-9 6.8e-3 2.85e-07 1.8e+06 '  #in m^2/s mol/cc 3.2e-9 6.8e-3 6.44e-9 mobility = mob_coefficient= ( Dze/(k_b*T))
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
  csv = true
[]
