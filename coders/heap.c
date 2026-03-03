/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyahyaou <nyahyaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 14:08:50 by nyahyaou          #+#    #+#             */
/*   Updated: 2026/03/03 14:08:55 by nyahyaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap_req(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int	less_req(t_request a, t_request b)
{
	if (a.priority != b.priority)
		return (a.priority < b.priority);
	return (a.coder_id < b.coder_id);
}

static void	heap_sift_up(t_heap *q, int i)
{
	int	p;

	while (i > 0)
	{
		p = (i - 1) / 2;
		if (less_req(q->arr[p], q->arr[i]))
			break ;
		swap_req(&q->arr[p], &q->arr[i]);
		i = p;
	}
}

int	heap_push(t_heap *q, t_request r)
{
	int	i;

	if (q->size >= q->capacity)
		return (0);
	i = q->size++;
	q->arr[i] = r;
	heap_sift_up(q, i);
	return (1);
}
