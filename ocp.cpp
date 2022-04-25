#include "include/ocp.h"
#include <array>

namespace ocp
{
    OCP::OCP()
    {
        acados_ocp_capsule = pendulum_ode_acados_create_capsule();
        // there is an opportunity to change the number of shooting intervals in C without new code generation
        int N = PENDULUM_ODE_N;
        // allocate the array and fill it accordingly
        double *new_time_steps = NULL;
        status = pendulum_ode_acados_create_with_discretization(acados_ocp_capsule, N, new_time_steps);

        if (status)
        {
            printf("pendulum_ode_acados_create() returned status %d. Exiting.\n", status);
            exit(1);
        }

        nlp_config = pendulum_ode_acados_get_nlp_config(acados_ocp_capsule);
        nlp_dims = pendulum_ode_acados_get_nlp_dims(acados_ocp_capsule);
        nlp_in = pendulum_ode_acados_get_nlp_in(acados_ocp_capsule);
        nlp_out = pendulum_ode_acados_get_nlp_out(acados_ocp_capsule);
        nlp_solver = pendulum_ode_acados_get_nlp_solver(acados_ocp_capsule);
        nlp_opts = pendulum_ode_acados_get_nlp_opts(acados_ocp_capsule);

        // initial condition
        int idxbx0[NBX0];
        idxbx0[0] = 0;
        idxbx0[1] = 1;
        idxbx0[2] = 2;
        idxbx0[3] = 3;

        double lbx0[NBX0];
        double ubx0[NBX0];
        lbx0[0] = 0;
        ubx0[0] = 0;
        lbx0[1] = 0;
        ubx0[1] = 0;
        lbx0[2] = 0;
        ubx0[2] = 0;
        lbx0[3] = 0;
        ubx0[3] = 0;

        ocp_nlp_constraints_model_set(nlp_config, nlp_dims, nlp_in, 0, "idxbx", idxbx0);
        ocp_nlp_constraints_model_set(nlp_config, nlp_dims, nlp_in, 0, "lbx", lbx0);
        ocp_nlp_constraints_model_set(nlp_config, nlp_dims, nlp_in, 0, "ubx", ubx0);

        // initialization for state values
        double x_init[NX];
        x_init[0] = 0.0;
        x_init[1] = 0.0;
        x_init[2] = 0.0;
        x_init[3] = 0.0;

        // initial value for control input
        double u0[NU];
        u0[0] = 0.0;
    }

    OCP::~OCP()
    {
        // free solver
        printf("Solver destructed\n");
        int status = pendulum_ode_acados_free(acados_ocp_capsule);
        if (status)
        {
            printf("pendulum_ode_acados_free() returned status %d. \n", status);
        }
        // free solver capsule
        status = pendulum_ode_acados_free_capsule(acados_ocp_capsule);
        if (status)
        {
            printf("pendulum_ode_acados_free_capsule() returned status %d. \n", status);
        }
    }

    double OCP::step_ocp(std::array<double, PENDULUM_ODE_NX> &init_state)
    {
        set_initial_state(init_state);
        ocp_warm_start();
        int status = ocp_solver();
        printf("solver status=%d\n", status);
        double u_out = utraj[0];
        return u_out;
    }

    void OCP::set_initial_state(std::array<double, PENDULUM_ODE_NX> &x_state)
    {
        ocp_nlp_constraints_model_set(nlp_config, nlp_dims, nlp_in, 0, "lbx", &*x_state.begin());
        ocp_nlp_constraints_model_set(nlp_config, nlp_dims, nlp_in, 0, "ubx", &*x_state.begin());
    }

    void OCP::set_ith_stage_state(int i, std::array<double, PENDULUM_ODE_NX> &x_state)
    {
        ocp_nlp_out_set(nlp_config, nlp_dims, nlp_out, i, "x", &*x_state.begin());
    }

    void OCP::set_ith_stage_input(int i, std::array<double, PENDULUM_ODE_NU> &u_input)
    {
        ocp_nlp_out_set(nlp_config, nlp_dims, nlp_out, i, "u", &*u_input.begin());
    }

    int OCP::ocp_solver()
    {
        // solve ocp in loop
        int rti_phase = 0;
        // prepare evaluation
        int NTIMINGS = 1;
        double min_time = 1e12;
        double elapsed_time;
        int sqp_iter;

        for (int ii = 0; ii < NTIMINGS; ii++)
        {
            ocp_nlp_solver_opts_set(nlp_config, nlp_opts, "rti_phase", &rti_phase);
            status = pendulum_ode_acados_solve(acados_ocp_capsule);
            ocp_nlp_get(nlp_config, nlp_solver, "time_tot", &elapsed_time);
            min_time = MIN(elapsed_time, min_time);
        }

        /* print solution and statistics */
        for (int ii = 0; ii <= nlp_dims->N; ii++)
            ocp_nlp_out_get(nlp_config, nlp_dims, nlp_out, ii, "x", &xtraj[ii * NX]);
        for (int ii = 0; ii < nlp_dims->N; ii++)
            ocp_nlp_out_get(nlp_config, nlp_dims, nlp_out, ii, "u", &utraj[ii * NU]);

        // printf("\n--- xtraj ---\n");
        // d_print_exp_tran_mat(NX, PENDULUM_ODE_N + 1, xtraj, NX);
        // printf("\n--- utraj ---\n");
        // d_print_exp_tran_mat(NU, PENDULUM_ODE_N, utraj, NU);
        // ocp_nlp_out_print(nlp_solver->dims, nlp_out);
        return status;
    }

    void OCP::ocp_warm_start()
    {
        for (int i = 1; i < PENDULUM_ODE_N; i++)
        {
            auto state_ith_iterator = std::next(std::begin(xtraj), PENDULUM_ODE_NX * i);
            auto input_ith_iterator = std::next(std::begin(xtraj), PENDULUM_ODE_NU * i);
            ocp_nlp_out_set(nlp_config, nlp_dims, nlp_out, i, "x", &*state_ith_iterator);
            ocp_nlp_out_set(nlp_config, nlp_dims, nlp_out, i, "u", &*input_ith_iterator);
        }
        auto state_Nth_iterator = std::next(std::begin(xtraj), PENDULUM_ODE_NX * PENDULUM_ODE_N);
        ocp_nlp_out_set(nlp_config, nlp_dims, nlp_out, PENDULUM_ODE_N, "x", &*state_Nth_iterator);
    }

} // ocp