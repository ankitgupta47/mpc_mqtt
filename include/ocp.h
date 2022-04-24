#ifndef OCP_H_
#define OCP_H_
// standard
#include <stdio.h>
#include <stdlib.h>
#include <array>
// acados
#include "./acados/utils/print.h"
#include "./acados/utils/math.h"
#include "./acados_c/ocp_nlp_interface.h"
#include "./acados_c/external_function_interface.h"
#include "./acados_solver_pendulum_ode.h"

#define NX     PENDULUM_ODE_NX
#define NZ     PENDULUM_ODE_NZ
#define NU     PENDULUM_ODE_NU
#define NP     PENDULUM_ODE_NP
#define NBX    PENDULUM_ODE_NBX
#define NBX0   PENDULUM_ODE_NBX0
#define NBU    PENDULUM_ODE_NBU
#define NSBX   PENDULUM_ODE_NSBX
#define NSBU   PENDULUM_ODE_NSBU
#define NSH    PENDULUM_ODE_NSH
#define NSG    PENDULUM_ODE_NSG
#define NSPHI  PENDULUM_ODE_NSPHI
#define NSHN   PENDULUM_ODE_NSHN
#define NSGN   PENDULUM_ODE_NSGN
#define NSPHIN PENDULUM_ODE_NSPHIN
#define NSBXN  PENDULUM_ODE_NSBXN
#define NS     PENDULUM_ODE_NS
#define NSN    PENDULUM_ODE_NSN
#define NG     PENDULUM_ODE_NG
#define NBXN   PENDULUM_ODE_NBXN
#define NGN    PENDULUM_ODE_NGN
#define NY0    PENDULUM_ODE_NY0
#define NY     PENDULUM_ODE_NY
#define NYN    PENDULUM_ODE_NYN
#define NH     PENDULUM_ODE_NH
#define NPHI   PENDULUM_ODE_NPHI
#define NHN    PENDULUM_ODE_NHN
#define NPHIN  PENDULUM_ODE_NPHIN
#define NR     PENDULUM_ODE_NR

namespace ocp
{
class OCP
{
    public:
    OCP();
    ~OCP();
    void set_initial_state( std::array<double,PENDULUM_ODE_NX>& x_state);
    void set_ith_stage_state(int i, std::array<double,PENDULUM_ODE_NX>& x_state);
    void set_ith_stage_input(int i, std::array<double,PENDULUM_ODE_NU>& u_input);
    double step_ocp(std::array<double,PENDULUM_ODE_NX>& init_state);

    private:
    int ocp_solver();
    void ocp_warm_start();
    pendulum_ode_solver_capsule *acados_ocp_capsule;
    ocp_nlp_config *nlp_config;
    ocp_nlp_dims *nlp_dims;
    ocp_nlp_in *nlp_in;
    ocp_nlp_out *nlp_out;
    ocp_nlp_solver *nlp_solver;
    double xtraj[NX * (PENDULUM_ODE_N+1)];
    double utraj[NU * PENDULUM_ODE_N];
    void *nlp_opts;
    int status;

};
}//ocp
#endif
