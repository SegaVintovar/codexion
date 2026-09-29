/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   coder.c											:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: vs <vs@student.42.fr>					  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/09/05 13:00:49 by vsudak			#+#	#+#			 */
/*   Updated: 2026/09/29 13:53:15 by vs			   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "codexion.h"

t_coder	*new_coder(int id, t_quantum_compiler *state)
{
	t_coder	*new;

	new = malloc(sizeof(t_coder));
	if (!new)
		return (NULL);
	new->id = id;
	new->compiles_left = state->comp_c_r;
	new->state = state;
	new->last_comp_t = 0;
	return (new);
}

void	assign_dongles(t_coder *coder, t_quantum_compiler *state)
{
	int	c_id;
	int	ld_id;
	int	rd_id;

	c_id = coder->id;
	if (c_id == 0)
	{
		ld_id = state->coders_c - 1;
		rd_id = c_id;
	}
	else if (c_id == state->coders_c - 1)
	{
		ld_id = c_id - 1;
		rd_id = c_id;
	}
	else
	{
		ld_id = c_id - 1;
		rd_id = c_id;
	}
	coder->left = state->dongles[ld_id];
	coder->right = state->dongles[rd_id];
}
