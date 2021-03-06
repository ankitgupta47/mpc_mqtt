/**************************************************************************************************
*                                                                                                 *
* This file is part of HPIPM.                                                                     *
*                                                                                                 *
* HPIPM -- High-Performance Interior Point Method.                                                *
* Copyright (C) 2019 by Gianluca Frison.                                                          *
* Developed at IMTEK (University of Freiburg) under the supervision of Moritz Diehl.              *
* All rights reserved.                                                                            *
*                                                                                                 *
* The 2-Clause BSD License                                                                        *
*                                                                                                 *
* Redistribution and use in source and binary forms, with or without                              *
* modification, are permitted provided that the following conditions are met:                     *
*                                                                                                 *
* 1. Redistributions of source code must retain the above copyright notice, this                  *
*    list of conditions and the following disclaimer.                                             *
* 2. Redistributions in binary form must reproduce the above copyright notice,                    *
*    this list of conditions and the following disclaimer in the documentation                    *
*    and/or other materials provided with the distribution.                                       *
*                                                                                                 *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND                 *
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED                   *
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE                          *
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR                 *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES                  *
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;                    *
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND                     *
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT                      *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS                   *
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                    *
*                                                                                                 *
* Author: Gianluca Frison, gianluca.frison (at) imtek.uni-freiburg.de                             *
*                                                                                                 *
**************************************************************************************************/

#ifndef HPIPM_S_TREE_OCP_QP_DIM_H_
#define HPIPM_S_TREE_OCP_QP_DIM_H_

#include "hpipm_common.h"

#ifdef __cplusplus
extern "C" {
#endif



struct s_tree_ocp_qp_dim
	{
	struct tree *ttree; // tree describing node conndection
	int *nx; // number of states // Nn
	int *nu; // number of inputs // Nn
	int *nb; // number of box constraints // Nn
	int *nbx; // number of state box constraints // Nn
	int *nbu; // number of input box constraints // Nn
	int *ng; // number of general constraints // Nn
	int *ns; // number of soft constraints // Nn
	int *nsbx; // number of soft state box constraints
	int *nsbu; // number of soft input box constraints
	int *nsg; // number of soft general constraints
	int Nn; // number of nodes
	hpipm_size_t memsize;
	};



//
hpipm_size_t s_tree_ocp_qp_dim_strsize();
//
hpipm_size_t s_tree_ocp_qp_dim_memsize(int Nn);
//
void s_tree_ocp_qp_dim_create(int Nn, struct s_tree_ocp_qp_dim *qp_dim, void *memory);
//
void s_tree_ocp_qp_dim_set_all(struct tree *ttree, int *nx, int *nu, int *nbx, int *nbu, int *ng, int *nsbx, int *nsbu, int *nsg, struct s_tree_ocp_qp_dim *dim);
//
void s_tree_ocp_qp_dim_set_tree(struct tree *ttree, struct s_tree_ocp_qp_dim *dim);
//
void s_tree_ocp_qp_dim_set(char *field, int stage, int value, struct s_tree_ocp_qp_dim *dim);
//
void s_tree_ocp_qp_dim_set_nx(int stage, int value, struct s_tree_ocp_qp_dim *dim);
//
void s_tree_ocp_qp_dim_set_nu(int stage, int value, struct s_tree_ocp_qp_dim *dim);
//
void s_tree_ocp_qp_dim_set_nbx(int stage, int value, struct s_tree_ocp_qp_dim *dim);
//
void s_tree_ocp_qp_dim_set_nbu(int stage, int value, struct s_tree_ocp_qp_dim *dim);
//
void s_tree_ocp_qp_dim_set_ng(int stage, int value, struct s_tree_ocp_qp_dim *dim);
//
void s_tree_ocp_qp_dim_set_ns(int stage, int value, struct s_tree_ocp_qp_dim *dim);
//
void s_tree_ocp_qp_dim_set_nsbx(int stage, int value, struct s_tree_ocp_qp_dim *dim);
//
void s_tree_ocp_qp_dim_set_nsbu(int stage, int value, struct s_tree_ocp_qp_dim *dim);
//
void s_tree_ocp_qp_dim_set_nsg(int stage, int value, struct s_tree_ocp_qp_dim *dim);
//
void s_tree_ocp_qp_dim_set_nbxe(int stage, int value, struct s_tree_ocp_qp_dim *dim);
//
void s_tree_ocp_qp_dim_set_nbue(int stage, int value, struct s_tree_ocp_qp_dim *dim);
//
void s_tree_ocp_qp_dim_set_nge(int stage, int value, struct s_tree_ocp_qp_dim *dim);



#ifdef __cplusplus
}	// #extern "C"
#endif



#endif // HPIPM_S_TREE_OCP_QP_DIM_H_
