#
# Example 1
# Illustrating the coupling between chemical and electrical driving forces.
# Check the file below for comments and suggestions for parameter modifications.
#
[Mesh]
type = GeneratedMesh
dim = 2
nx = 40
ny = 40
nz = 0
xmin = 0
xmax = 50
ymin = 0
ymax = 50
zmin = 0
zmax = 0
elem_type = QUAD4
[]
[Variables]
[./c]
order = FIRST
family = LAGRANGE
[./InitialCondition]
type = SmoothCircleIC
x1 = 0
y1 = 0
radius = 25.0
invalue = 1.0
outvalue = 0.0
int_width = 50.0
[../]
[../]
[./w]
order = FIRST
family = LAGRANGE
[../]
[./v]
order = FIRST
family = LAGRANGE
[../]
[]
[Kernels]
[./ElectricPotential]
type = ElectricPotential
#type = CoupledPotential
variable = v
[../]
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
[]
#
# The AuxVariables and AuxKernels below are added to visualize the xx and yy stress tensor components
#
#[AuxVariables]
#[]
#[AuxKernels]
#[]
[Materials]
[./pfmobility]
type = PFMobility
block = 0
#kappa = 0.1
kappa = 5
#mob = 1e-3
mob = 1
[../]
# simple chemical free energy with a miscibility gap
[./chemical_free_energy]
type = DerivativeParsedMaterial
block = 0
f_name = Fc
args = 'c'
constant_names = 'barr_height cv_eq'
constant_expressions = '0.1 1.0e-2'
function = 16*barr_height*(c-cv_eq)^2*(1-cv_eq-c)^2
enable_jit = true
derivative_order = 2
[../]
# free energy contribution from electrical energy
[./electric_free_energy]
type = ElasticEnergyMaterial #Write the newly written material src C file name
f_name = Fe
block = 0
args = 'c'
derivative_order = 2
[../]
# Sum up chemical and elastic contributions
[./free_energy]
type = DerivativeSumMaterial
block = 0
f_name = F
sum_materials = 'Fc Fe'
args = 'c'
derivative_order = 2
[../]
[]
[BCs]
[./bottom_y]
type = PresetBC
variable = disp_y
boundary = 'bottom'
value = 0
[../]
[./top_y]
type = PresetBC
variable = disp_y
boundary = 'top'
# prescribed displacement
# -5 will result in a compressive stress
# 5 will result in a tensile stress
value = -5
[../]
[./left_x]
type = PresetBC
variable = disp_x
boundary = 'left'
value = 0
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
solve_type = 'PJFNK'
petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
petsc_options_value = 'asm 101 preonly lu 1'
l_max_its = 30
nl_max_its = 10
l_tol = 1.0e-4
nl_rel_tol = 1.0e-8
nl_abs_tol = 1.0e-10
start_time = 0.0
num_steps = 200
[./TimeStepper]
type = SolutionTimeAdaptiveDT
dt = 1
[../]
[]
[Outputs]
interval = 1
exodus = true
output_on = timestep_end
[./console]
type = Console
perf_log = true
output_on = 'timestep_end failed nonlinear'
[../]
[]