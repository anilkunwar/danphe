# Applicable for 2d axisymmetric model
# Illustrates on how a drop spreads on impacting a rigid bottom wall
# The real constants and materials constants are described as Protected type in SurfaceTension.h header file
# Kernels in danphe app : CHConvection and SurfaceTension
# Other source codes are in the core MOOSE Framework over which danphe resides.
# Authors: Vitaliy Yurkiv and Anil Kunwar
[GlobalParams]
  gravity = '0 0.0 0'
  supg = true
  pspg = true
  convective_term = true
  integrate_p_by_parts = true
  transient_term = true
  laplace = true
  u = vel_x
  v = vel_y
  p = p
  alpha = 1
  enable_jit=false
[]


[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 35
  ny = 35
  nz = 0
  xmin = 0
  ymin = 0
  zmin = 0
  xmax = 5.0
  ymax = 5.0
  zmax = 0
  elem_type = QUAD4
[]


[Variables]
# Velocity in radial (r) direction
  [./vel_x]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
	      type = FunctionIC
        function = vel_x_IC
    [../]
  [../]

# Velocity in axial (z) direction
  [./vel_y]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
	      type = FunctionIC
        function = vel_y_IC
    [../]
 [../]

# pressure for the NS module
  [./p]
    order = FIRST
    family = LAGRANGE
  [../]

#########
# phase-field module variables: concentration (c) and chemical potential (w)
#########
  [./c]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
	      type = FunctionIC
        function = c_IC
    [../]
  [../]

  [./w]
    order = FIRST
    family = LAGRANGE
  [../]
[]

# setting up 2D axisymmetric calculations
[Problem]
  type          = FEProblem     # This is the "normal" type of Finite Element Problem in MOOSE
  coord_type    = RZ            # Axisymmetric RZ
  rz_coord_axis = Y             # Which axis the symmetry is around
[]


[Functions]
# concentration initial conditions
  [./c_IC]
    type = ParsedFunction
    value = 'r:=(((x-0.0)/1.0)^2+((y-1.0)/1.0)^2-0);if(r<=1,1.0,0)'
  [../]
# velocity in X direction initial condition, in this problem it is zero
  [./vel_x_IC]
    type = ParsedFunction
    value = 'r:=(((x-0.0)/1.0)^2+((y-1.0)/1.0)^2-0);if(r<=1,0.0,0)'
  [../]
# velocity in Y direction initial conditions, vertical velocity is applied
  [./vel_y_IC]
    type = ParsedFunction
    value = 'r:=(((x-0.0)/1.0)^2+((y-1.0)/1.0)^2-0);if(r<=1,-0.1,0)'
  [../]

[]



[AuxVariables]
[./time]
[../]
  [./x_coor]
  [../]
  [./y_coor]
  [../]
[]

  [AuxKernels]
    [./time]
      type = FunctionAux
      variable = time
      function = t
    [../]
    [./x_coor]
    type = FunctionAux
    variable = x_coor
    function = x
  [../]
        [./y_coor]
      type = FunctionAux
      variable = y_coor
      function = y
    [../]

[]


[Kernels]

#########
# 2D axisymmetric Navier-Stokes kernels: mass, momentum time and space
#########
# mass
 [./mass]
  type = INSMassRZ
   variable = p
   u = vel_x
   v = vel_y
   p = p
#   x_vel_forcing_func = vel_x_ff
#   y_vel_forcing_func = vel_y_ff
 [../]

# The updated INSMomentumTimeDerivative kernel in moose can now take :"density name" from Materials block
# So, we seek to obtain rho from DerivativeParsedMaterial material class
 [./x_momentum_time]
   type = INSMomentumTimeDerivative
   variable = vel_x 
   #rho_name = rho
 [../]
 [./y_momentum_time]
   type = INSMomentumTimeDerivative
   variable = vel_y
   #rho_name = rho
 [../]

 # x-momentum, space
 [./x_momentum_space]
  type = INSMomentumLaplaceFormRZ
   variable = vel_x
   component = 0
#   forcing_func = vel_x_ff
 [../]

 # y-momentum, space
 [./y_momentum_space]
   type = INSMomentumLaplaceFormRZ
   variable = vel_y
   component = 1
#   forcing_func = vel_y_ff
 [../]

 # coupled force, we need it to pass surface tension from the PFM to the NS module. It is needed for both x and y vel
  [./force_x]
    #type = CoupledForce
    type = SurfaceTension
    variable = vel_x
    v = c
    func_name = F_s
    coef = 1
  [../]

  [./force_y]
    #type = CoupledForce
    type = SurfaceTension
    variable = vel_y
    v = c
    func_name = F_s
    coef = 1
  [../]

  #########
  # the PFM kernels
  #########
  [./c_res]
    type = SplitCHParsed
    variable = c
    f_name = F
    kappa_name = kappa_c
    w = w
  [../]

  [./w_res]
    type = SplitCHWResAniso     # SplitCHWRes
    variable = w
    mob_name = M_c
  [../]
  [./time]
    type = CoupledTimeDerivative
    variable = w
    v = c
  [../]

# the term to add advection velocity to the Cahn-Hilliard eq. u*nabla(C):
# dC/dt+u*nabla(C)=(1/Pe)*(M nabla(w)), Pe is the Peclet number Pe = L*u/(M*w), L is the characteristic lenght
  [./conv]
    #type = MyConvection
    type = CHConvection
     variable = c
	velocity_x = vel_x
	velocity_y = vel_y
 [../]

[]


[Materials]

#########
# setting up density and viscosity for the NS module
#########
# rho_water = 1000 kg/m^3
# rho_air = 1.29 kg/m3
  [./rho]
     type = DerivativeParsedMaterial
      args = 'c'
      f_name = rho #'rho'
      constant_names        = 'rho_l    rho_s    lambda_rho'
      constant_expressions  = '1         1         1.29e-3'

      function = '1*((c)^2)^0.5+1*(1-((c)^2)^0.5)*lambda_rho'
      derivative_order = 0
      outputs = exodus
      output_properties = 'rho'
      #material_property_names = 'rho'
    [../]
# mu_water = 1.0*10^-3 kg/(m s)
# mu_air = 1.79*10^-5 kg/(m s)
    [./mu]
       type = DerivativeParsedMaterial
        args = 'c'
        f_name = 'mu'
        constant_names        = 'mu_l   mu_s    lambda_mu'
        constant_expressions  = '1      10       1.79e-2'
        function = '1*((c)^2)^0.5+1*(1-((c)^2)^0.5)*lambda_mu'
        derivative_order = 0
        outputs = exodus
        output_properties = 'mu'
      [../]
# the NS forcing function: surface tension, coef = 1/Re*Ca
      [./F_s]
        type = DerivativeParsedMaterial
        f_name = F_s
        args = 'c w'
        material_property_names = 'F  dF:=D[F(c),c,]'
        constant_names =       ' coef    '
        constant_expressions = '5.0e1   '
        function = 'coef*w*c^2*(1-c)^2'
        derivative_order = 0
        outputs = exodus
        output_properties = F_s
      [../]

# anisotropic mobility, active only at the interface meaning no bulk mobility
    [./c_aniso]
      type = ConstantAnisotropicMobility
      tensor = '1.0   0.0   0.0
                0.0   1.0   0.0
                0.0   0.0   0.0'
      M_name = c_aniso_tensor
      outputs = exodus
    [../]
    [./var_dependence_c_tensor_1]
      type = DerivativeParsedMaterial
      args = 'c time '
      f_name = c_var_dep_1
      constant_names =       ' M_0     '
      constant_expressions = '10.0   '
      function = 'M_0*c^2*(1-c)^2'
      derivative_order = 0
      outputs = exodus
    [../]
    [./c_mobility_tensor]
      type = CompositeMobilityTensor
      M_name =  M_c
      tensors = 'c_aniso_tensor'
      weights = 'c_var_dep_1   '
      args = 'c'
      outputs = exodus
      output_properties = M_c
    [../]
# gradient energy coefficient
    [./kappa_c]
    		type = DerivativeParsedMaterial
    		f_name = kappa_c
    		args = 'c time'
    		constant_names = 'kappa_c_0'
    		constant_expressions = '0.002'
    		function = 'kappa_c_0'
    		derivative_order = 0
    		outputs = exodus
    		output_properties = kappa_c
      [../]
# free energy of the system
      [./free_energy]
        type = DerivativeParsedMaterial
        f_name = F
        args = 'c'
        constant_names = 'barr_height  cv_eq'
        constant_expressions = '0.25      0'
        function = 16*barr_height*(c-cv_eq)^2*(1-cv_eq-c)^2
        derivative_order = 2
        outputs = exodus
        output_properties = F
      [../]
[]

[BCs]

#########
# BC for the NS eq
#########
[./u_axis]
  type = DirichletBC
  boundary = 'left bottom'
  variable = vel_x
  value = 0
[../]
[./v_no_slip]
  type = DirichletBC
  boundary = 'bottom'
  variable = vel_y
  value = 0
[../]

[./u_domain]
  type = NeumannBC
  boundary = 'top right'
  variable = vel_x
  value = 0
[../]
[./v_domain]
  type = NeumannBC
  boundary = 'left top right'
  variable = vel_y
  value = 0
[../]

#########
# BC for the PFM
#########


# no flux of c, in this case contact angle is 90 degree, n*M*nabla(C)=0
  [./c_bottom_CHF_BC]
    type = CahnHilliardFluxBC
    variable = c
    boundary = bottom
    flux = '0.0 0.0 0.0'
    mob_name = 1
    args = ''
  [../]


# no flux of chemical potential
  [./w_bottom_CHF_BC]
    type = CahnHilliardFluxBC
    variable = w
    boundary = 'top bottom left right'
    flux = '0.0 0.0 0.0'
    mob_name = 1
    args = ''
  [../]

[]

[Preconditioning]
  [./SMP_PJFNK]
    type = SMP
    full = true
    solve_type = Newton
  [../]
[]

[Executioner]
  type = Transient
  petsc_options_iname = '-pc_type -sub_pc_type -sub_pc_factor_levels'
  petsc_options_value = 'bjacobi  ilu          4'

  nl_rel_tol  = 1e-5
  nl_abs_tol  = 1e-5
  nl_max_its  = 50
  l_tol       = 1e-5
  l_max_its   = 50

  end_time    = 200

# adaptive time stepping
  [./TimeStepper]
	    type = IterationAdaptiveDT
	    dt = 0.01
  [../]

# adaptive mesh to resolve an interface
  [./Adaptivity]
    initial_adaptivity    = 2             # Number of times mesh is adapted to initial condition
    refine_fraction       = 0.7           # Fraction of high error that will be refined
    coarsen_fraction      = 0.1           # Fraction of low error that will coarsened
    max_h_level           = 3             # Max number of refinements used, starting from initial mesh (before uniform refinement)
    weight_names          = 'c	 '
    weight_values         = '1  '
  [../]

[]

[Outputs]
  exodus = true
[]
