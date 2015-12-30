[GlobalParams]
# Dummy parameters
gravity = '0 -10 0'
rho = 6950
mu = 2.3309e-7
displacements = 'disp_x disp_y'
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  xmin = 0
  xmax = 1.243e-3
  ymin = 0
  ymax = 1.243e-3
  nx = 16
  ny = 16
  elem_type = QUAD9
[]

[Variables]
  # x-velocity
  [./u]
    order = SECOND
    family = LAGRANGE

    [./InitialCondition]
      type = ConstantIC
      value = 0.0
    [../]
  [../]

  # y-velocity
  [./v]
    order = SECOND
    family = LAGRANGE

    [./InitialCondition]
      type = ConstantIC
      value = 0.0
    [../]
  [../]

  # Pressure
[./p]
 order = FIRST
 family = LAGRANGE

 [./InitialCondition]
  type = ConstantIC
   value = 0 #1.0e+5 # This number is arbitrary for NS...
 [../]
[../]
[./disp_x]
initial_condition = 0
[../]
[./disp_y]
initial_condition = 0
[../]
[]


[Kernels]
# mass
# x-momentum, time
# x-momentum, space
# y-momentum, time
# y-momentum, space
active = 'y_momentum_space x_momentum_space  mass tdx tdy'
[./mass]
type = INSMass
variable = p
u = u
v = v
p = p
block = 0
#use_displaced_mesh = true
[../]
[./x_momentum_time]
type = INSMomentumTimeDerivative
variable = u
block = 0
#use_displaced_mesh = true
[../]
[./x_momentum_space]
type = INSMomentumGravity
variable = u
u = u
v = v
p = p
component = 0
block = 0
#use_displaced_mesh = true
[../]
[./y_momentum_time]
type = INSMomentumTimeDerivative
variable = v
block = 0
#use_displaced_mesh = true
[../]
[./y_momentum_space]
type = INSMomentumGravity
variable = v
u = u
v = v
p = p
component = 1
block = 0
#use_displaced_mesh = true
[../]
[./tdx]
type = TimeDerivative
variable = disp_x
block = 0
use_displaced_mesh = true
[../]
[./tdy]
type = TimeDerivative
variable = disp_y
block = 0
#use_displaced_mesh = true
[../]
[]

[BCs]
active = 'x_dis_right y_slip_right x_no_slip_left y_no_slip_left '
[./x_dis_right]
type = NSImposedVelocityBC
variable = u
boundary = left
desired_velocity = 5.0e-11
use_displaced_mesh = true
[../]
[./y_slip_right]
type = DirichletBC
variable = v
boundary = 'right'
value = 0.0
#use_displaced_mesh = true
[../]
[./x_no_slip_left]
type = DirichletBC
variable = u
boundary = 'left'
value = 0.0
#use_displaced_mesh = true
[../]
[./y_no_slip_left]
type = DirichletBC
variable = v
boundary = 'left'
value = 0.0
#use_displaced_mesh = true
[../]
[./pressure_pin]
    type = DirichletBC
    variable = p
    boundary = '99'
    value = 0
  [../]
[]

[Postprocessors]
[./v_right]
type = SideAverageValue
variable = v
boundary = right
#use_displaced_mesh = true
[../]
[]

[Preconditioning]
[./SMP_PJFNK]
# Preconditioned JFNK (default)
type = SMP
full = true
solve_type = PJFNK
[../]
[]

[Executioner]
# type = Steady
type = Transient
petsc_options_iname = '-ksp_gmres_restart '
petsc_options_value = '300 '
line_search = none
nl_rel_tol = 1e-8
nl_max_its = 6
l_tol = 1e-8
l_max_its = 300
#l_max_its = 30
start_time = 0.0
  num_steps = 2
#num_steps = 1 # 200
#end_time = 10
[]

[Outputs]
#file_base = sub1
csv = true
exodus = true
[]

