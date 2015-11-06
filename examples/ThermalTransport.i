# Two phase materials simulation
# Simulation of thermodiffusion of Cu in molten Sn.
#

[Mesh]
  type = GeneratedMesh
  dim = 2
  distribution = DEFAULT
  elem_type = QUAD4
  nx = 25
  ny = 25
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
  [./T] # Temperature of the medium (in K)
    order = FIRST
    family = LAGRANGE
    initial_condition = 500.0
    #scaling = 1.0e5
  [../] 
[]

[ICs]
  [./concentrationIC]   # 46.774 mol% Cr with variations
    type = RandomIC
    min = 0.018
    max = 0.015
    seed = 210
    variable = c
  [../]
[]

[BCs]
  #[./Periodic]
   # [./c_bcs]
   #   auto_direction = 'x y'
    #[../]
  #[../]

  [./Top_T]
    type = DirichletBC
    variable = T
    boundary = top
    value = 520.0
  [../]

  [./Bottom_T]
    type = DirichletBC
    variable = T
    boundary = bottom
    value = 623.0
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
  [./w_res_soret]
    type = MultiSoretDiffusion
    variable = w
    c = c
    T = T
    net_thermotransport = Mq
    #name in kernel file = symbol in src/materials/filename.C and include/materials/filename.h
  [../]
  [./HtCond]
    type = MatDiffusion
    variable = T
    D_name = thermal_conductivity
    #Either the effective thermal conductivity/ the composite thermal conductivity or something else
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
    #block = 0
    prop_names = 'kappa_c'
    #prop_values = '8.215e-16'
    prop_values = '8.125e-16*6.24150934e+18*1e+09^2*1.0e-10'
                  # kappa_c*eV_J*nm_m^2
  [../]
  [./mobility]               # Mobility (nm^2 mol/eV/s)
    # Mohanty et. al. (JAP-2009, JNM-2011)
    type = DerivativeParsedMaterial
    f_name = Mc
    args = c
    #block = 0
    #constant_names =       'Acr    Bcr    Ccr    Dcr
     #                       Ecr    Fcr    Gcr
     #                       Afe    Bfe    Cfe    Dfe
     #                       Efe    Ffe    Gfe
     #                       nm_m   eV_J   d'
    #constant_expressions = '-32.770969 -25.8186669 -3.29612744 17.669757
    #                        37.6197853 20.6941796  10.8095813
    #                        -31.687117 -26.0291774 0.2286581   24.3633544
    #                        44.3334237 8.72990497  20.956768
    #                        1e+09      6.24150934e+18          1e-27'
    constant_names =       'rho    B1    B2  nm_m   eV_J  d'
    constant_expressions = '6.13873e+4 12.0936e-9 4.9362e-14 1e+09 6.24150934e+18 1.0e-10'
    function = '1/nm_m/eV_J/d*rho*c*(1-c)*(c*B2 + (1-c)*B1)'
    #function = 'nm_m^2/eV_J/d*((1-c)^2*c*10^
    #            (Acr*c+Bcr*(1-c)+Ccr*c*log(c)+Dcr*(1-c)*log(1-c)+
    #            Ecr*c*(1-c)+Fcr*c*(1-c)*(2*c-1)+Gcr*c*(1-c)*(2*c-1)^2)
    #            +c^2*(1-c)*10^
    #            (Afe*c+Bfe*(1-c)+Cfe*c*log(c)+Dfe*(1-c)*log(1-c)+
    #            Efe*c*(1-c)+Ffe*c*(1-c)*(2*c-1)+Gfe*c*(1-c)*(2*c-1)^2))'
    #function = 'nm_m^2/eV_J*rho*c*(1-c)*(c*B1 + (1-c)*B2)'
    derivative_order = 1
    outputs = exodus
  [../]
  [./local_energy]
    # Defines the function for the local free energy density of liquid Cu-Sn system as given in the
    # problem, then converts units and adds scaling factor for its role in numerical convergence.
    # Park M.S. and Arroyave R., J. Elect. Mater., 39(2010), 2574-2582.
    type = DerivativeParsedMaterial
    #block = 0
    f_name = f_loc
    args = c
    #constant_names = 'A   B   C   D   E   F   G  eV_J d'
    #constant_expressions = '-2.446831e+04 -2.827533e+04 4.167994e+03 7.052907e+03
                            #1.208993e+04 2.568625e+03 -2.354293e+03
                            #6.24150934e+18 1e-27'
    #function = 'eV_J*d*(A*c+B*(1-c)+C*c*log(c)+D*(1-c)*log(1-c)+
                #E*c*(1-c)+F*c*(1-c)*(2*c-1)+G*c*(1-c)*(2*c-1)^2)'
    constant_names = 'Gcu   Gsn   R_T   L0   L1  L2 eV_J d'
    constant_expressions = '-1.1083e+04 -2.8963e+04 4.34822e+03 -1.0487e+04
                             -1.8198e+04 1.05284e+04 6.24150934e+18 1.0e-10'
    function = 'eV_J*d*(Gcu*(1-c)+Gsn*c+R_T*((1-c)*log(1-c)+c*log(c))+c*(1-c)*
                (L0+L1*(1-2*c)+L2*(1-4*c-4*c*c)))'
    derivative_order = 2
  [../]
  [./net_heatoftransport]
    # species 1 is tin and species 2 is copper
    type = ThermotransportParameter
    #block = 0
    c = c
    T = T # K
    #int_width = 60.0
    #length_scale = 1.0e-9
    #time_scale = 1.0e-9
    B0_1 = 1.2e-9 # m^2/s, from M. Abdulhamid Thesis(2008) and Ref 42 therein Z.Mei et al. (1992).
    B0_2 = 2.4e-5 # m^2/s, from M. Abdulhamid Thesis(2008) and Ref 42 therein Z.Mei et al. (1992).
    Qh1 = 1.36e3 # J/mol, H. _Y.Hsiao and C. Chen, Applied Physics Letters(APL), 94(2009)092107.
    Qh2 = 1.12e4 # J/mol, N. Zhao et. al., Scientific Reports (2015).
    E1 = 4.389e4 # in J/mol, from M. Abdulhamid Thesis(2008) and Ref 42 therein Z.Mei et al. (1992).
    E2 = 3.302e4 # in J/mol, from M. Abdulhamid Thesis(2008) and Ref 42 therein Z.Mei et al. (1992).
    #rho = 6.13873e+4 # in mol/m^3 from  Park M.S. and Arroyave R., J. Elect. Mater., 39(2010), 2574-2582.
    #surface_energy = 0.708 # Total guess
    #...
  [../]
   [./thcond]
    type = ParsedMaterial
    #block = 0
    args = 'c'
    function = 'if(c>0.7,4.0e-8,4.0e-8)'
    f_name = thermal_conductivity
    outputs = exodus
  [../]
  [./precipitate_indicator]  # Returns 1/625 if precipitate
      type = ParsedMaterial
      f_name = prec_indic
      args = c
      function = if(c>0.6,0.0016,0)
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
  #[./SMP]
   # type = FDP
   # full = true
  #[../]
[]

[Executioner]
  type = Transient
  solve_type = PJFNK
  l_max_its = 30
  l_tol = 1e-6
  nl_max_its = 50
  nl_abs_tol = 1e-9
  end_time = 86400   # 1 day. We only need to run this long enough to verify
                     # the model is working properly.
  petsc_options = '-snes_monitor -ksp_monitor_true_residual -snes_converged_reason -ksp_converged_reason'
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type
                         -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm      31                  preonly
                         ilu          1'
  dt = 3.6e3
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
