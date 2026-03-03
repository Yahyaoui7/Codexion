/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyahyaou <nyahyaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 15:21:53 by nyahyaou          #+#    #+#             */
/*   Updated: 2026/03/03 15:22:06 by nyahyaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	heap_sift_down(t_heap *q, int i)
{
	int	l;
	int	s;
	int	r;

	while (1)
	{
		l = 2 * i + 1;
		r = 2 * i + 2;
		s = i;
		if (l < q->size && less_req(q->arr[l], q->arr[s]))
			s = l;
		if (r < q->size && less_req(q->arr[r], q->arr[s]))
			s = r;
		if (s == i)
			break ;
		swap_req(&q->arr[i], &q->arr[s]);
		i = s;
	}
}

int	heap_pop(t_heap *q, t_request *out)
{
	if (q->size == 0)
		return (0);
	*out = q->arr[0];
	q->size--;
	if (q->size == 0)
		return (1);
	q->arr[0] = q->arr[q->size];
	heap_sift_down(q, 0);
	return (1);
}

int	heap_peek(t_heap *q, t_request *out)
{
	if (q->size == 0)
		return (0);
	*out = q->arr[0];
	return (1);
}
