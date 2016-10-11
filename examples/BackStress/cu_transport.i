#if we need to model concentration c, make e(-kd*t) =1 i.e. Neumann BC 
# and for this we need to decrease the value of a_1 from 10^-8 to 10^-10

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10 #25
  ny = 10 #25
  xmax = 2.8e-4 #Length of the solder material
  ymax = 2.0e-3 #height of the solder material
[]

[Variables]
  [./S]
  initial_condition = 1.0e-5 # Start at 1.0e-5 Pa
  scaling = 1.0e-10
  #scaling = 1.0e+10
  [../]
  [./c]
  initial_condition = 1.0 #4.85 mol/m^3 at cathode source: M.L. Huang et al Acta Materialia 100(2015) 98-106. 4.0e-4  #mol/cc
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
   [./j_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./drift_velocity]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./v_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./grads_cx]
    order = CONSTANT
    family = MONOMIAL
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
  [./backstress_transport]
    type = BackstressConvection
    variable = c
    hydrostatic = S
    D_name = diff_coefficient
    #Q_asterik = 1.112e+4
    #kb = 8.31
    T_c = 423.0
  [../]
  [./electrical_transport]
    type = ConstantTensorElectricConvection
    variable = c
    D_name = diff_coefficient
    z = 2 #20.0 
    kb = 1.38e-23
    e = 1.6e-19
    rho = 5.5e-7
    T_c = 423.0
  [../]
# [./imc_formation]
   # type = ReactionTerm
   # block = 0
   #  variable = c
     #Temperature = T
    # c_sat = conc_sat
     #k_chem = 1.9e-6 #k_chem *s/v in 1/s
  #[../]
[]

[AuxKernels]
  [./velocity_x]
    type = BackstressComponent
    variable = velocity_x
    component = x
    execute_on = timestep_end
    hydrostatic = S
    D_name = diff_coefficient
    Q_asterik = 1.112e+4
    # kb = 8.31
    omega = 2.71e-23
    T_c = 423.0 
  [../]
  [./velocity_y]
    type = BackstressComponent
    variable = velocity_y
    component = y
    execute_on = timestep_end
    hydrostatic = S
    D_name = diff_coefficient
    #kb = 8.31
    omega = 2.71e-23
    T_c = 423.0 
  [../]
  [./velocity_z]
    type = ThermalComponent
    variable = velocity_z
    component = z
    execute_on = timestep_end
    hydrostatic = S
    D_name = diff_coefficient
    #kb = 8.31
    omega = 2.71e-23
    T_c = 423.0 
    #outputs = exodus
  [../]
  [./current_density_scalar]
    type = MaterialRealVectorValueAux
    property = current_density
    variable = j_x
    #value = 6.5e+6
    component = 0
 [../]
 [./Drift_velocity_mag]
    type = DriftVelocity
    variable = drift_velocity
    #property = current_density_x
    #component = 0
    #current_density = j_x
    D_name = diff_coefficient
    j_x = 5.6e+6 #6.5e+6
    z = 22.27 #20 #2.0 
    kb = 1.38e-23
    e = 1.6e-19
    rho = 5.5e-7
    T_c = 523.0
  [../]
  [./D_vel_comp]
    type = ElectricComponent
    variable = v_x
    component = x
    D_name = diff_coefficient
    z =  22.27  #20 
    kb = 1.38e-23
    e = 1.6e-19
    rho = 5.5e-7 #put the values for different orientations
    T_c = 523.0
  [../]
  [./conc_gradx]
    type = VariableGradientComponent
    variable = grads_cx
    gradient_variable = c
    component = x
  [../]
  [./variable_time_integration]
    type = VariableTimeIntegrationAux
    variable = imc_width
    variable_to_integrate = c
    #Temperature = T
    #D_name = diff_coefficient
    #Q_asterik = 1.112e+4
    #kb = 8.31 #use universal gas constant
    coefficient = 5.89155e-7 #times omega 1.92765e-7
  [../]
[]

#[Functions]
  #active = 'bc_func'
  # A ParsedFunction allows us to supply analytic expressions
  # directly in the input file
  #[./bc_func]
   # type = ParsedFunction
    #value = 'cs*kd*exp(-kd*t)'
    #vars = 'cs kd'
    #value = 'a_1*exp(-kd*t)'
    #vars = 'a_1 kd'
    #vals = '1.5e-8 2.35e-3'  #k_d*s/v per second
  #[../]
#[]
[BCs]
  [./right]
    type = NeumannBC
    variable = S
    boundary = right
    value = 0.1 # (K) 
  [../]
  [./left]
    type = NeumannBC
    variable = S
    boundary = left
    value = 0.0 #0.0 Pa/m
  [../]
  #[./concentration_bottom]
    #type = RobinBCS
    #variable = c
    #boundary = bottom
    #alpha = 4.1e-2 # wt %/s (mol/cc^3/s)
    #beta = 1.81e-3 # c_s saturated solubility or concentration
   #value = 0.0  #wt %
  #[../]
  #[./concentration_bottom]
   # type = FunctionNeumannBC
    #type = NeumannBC
    #variable = c
    #boundary = bottom
    #value = 1.5e-9 #1.76e-10 #2.77e-6 #wt %/s (mol/cc^3/s) kd=8.2e-6 m/s del c =6.6e-3 mol/cc sgb =1.14e-2
    #value = 0.0  #wt %
    #function = bc_func
  #[../]
  [./concentration_right]
    type = DirichletBC
    variable = c
    boundary = right
    value = 4.85e-3 #mmol/cc # (K) 
  [../]
  #[./concentration_top]
    #type = NeumannBC
    #variable = c
    #boundary = top
    ##value = 1.09e+3 # (mol/m^3)
    #value = 0.0 #wt %
  #[../]
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
