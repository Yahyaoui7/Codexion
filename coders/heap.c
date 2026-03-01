#include "codexion.h"

static void	swap_req(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static int	less_req(t_request a, t_request b)
{
	if (a.priority != b.priority)
		return (a.priority < b.priority);
	return (a.coder_id < b.coder_id);
}

int	heap_peek(t_heap *q, t_request *out)
{
	if (q->size == 0)
		return (0);
	*out = q->arr[0];
	return (1);
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

int	heap_remove_by_id(t_heap *q, int coder_id)
{
	int	i;

	i = 0;
	while (i < q->size)
	{
		if (q->arr[i].coder_id == coder_id)
			break ;
		i++;
	}
	if (i == q->size)
		return (0);
	q->size--;
	if (i != q->size)
		q->arr[i] = q->arr[q->size];
	heap_sift_up(q, i);
	heap_sift_down(q, i);
	return (1);
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
