/* Author : Nicolas Reszka */

#include "stdio.h"
#include "stdlib.h"

/* Oriented graph structure */
typedef struct graph_s
{
	/* The number of nodes in the graph */
	int             node_count;

	/* Chained list containing this graph's nodes */
	struct node_s*  nodes;
} 	graph;

/* Node structure */
typedef struct node_s
{
	/* The nodes identification number */
	int             id;

	/* 0 if isn't marked, 1 if is marked */
	int             is_marked;	

	/* ID of the node pointing to this node */
	int             origin;

	/* Chained list containing edges coming from this node */
	struct edge_s*  edges; 

	/* Pointer to next node */
	struct node_s*  next;
} 	node;

/* Edge structure */
typedef struct edge_s
{
	int 			capacity;
	int 			stream;
	
	/* Pointer to the target node */
	struct node_s* 	target_node;

	/* Pointer to next edge from the parent node*/
	struct edge_s* 	next;
} 	edge;

/* Creates a empty graph */
graph* 	graph_create()
{	
	graph* 	new_graph;

	new_graph = malloc(sizeof(node));

	new_graph->node_count = 0;
	new_graph->nodes = NULL;

	return 	new_graph;
}

/* Adds a node to a graph */
void 	graph_add_node(graph* g, node* new_node)
{
	int 	i;
	node*	last_node;

	/* Start of the chained list of nodes */
	if (g->node_count == 0)
	{
		g->nodes = new_node;
	}
	else
	{
		last_node = g->nodes;

		/* Find the end of the chained list of nodes */
		while (last_node->next != NULL)
		{	
			last_node = last_node->next;
		}

		/* Append the new node */
		last_node->next = new_node;
	}

	g->node_count++;
}

/* Gets the node identified by the "id" in the "g" graph */
node*	graph_get_node(graph* g, int id)
{
	node*	target_node;

	target_node = g->nodes;

	while (target_node->id != id && target_node->next != NULL)
	{
		target_node = target_node->next;
	}

	return target_node; 
}

/* Prints the graph's nodes to the console */
void 	graph_log_nodes(graph* g)
{
	int 	i;
	node*	current_node;

	printf("this graph's nodes : \n");

	current_node = g->nodes;

	/* Iterate the node chained list */
	while (current_node != NULL) 
	{
		/* Print every nodes of the list */
		printf(
			"#%d => is_marked : %d origin : %d\n", 
			current_node->id, 
			current_node->is_marked, 
			current_node->origin
		);
		
		current_node = current_node->next;
	}
	putchar('\n');
}

/* Prints the graph's edges to the console */
void 	graph_log_edges(graph* g)
{
	int 	i;
	node*	current_node;
	edge*	current_edge;

	printf("this graph's edges : \n");

	current_node = g->nodes;

	/* Iterate the node chained list */
	while (current_node != NULL) 
	{
		current_edge = current_node->edges;

		/* Iterate the edge chained list */
		while (current_edge != NULL) 
		{
			/* Print every edge of the list */
			printf("%d -> %d capacity : %d stream : %d\n", current_node->id, current_edge->target_node->id, current_edge->capacity, current_edge->stream);

			current_edge = current_edge->next;
		}
		
		current_node = current_node->next;
	}
	putchar('\n');
}

/* Creates a new node */
node* 	node_create(int id)
{
	node* 	new_node;

	new_node = malloc(sizeof(node));

	new_node->id = id;
	new_node->is_marked = 0;
	new_node->origin = 0;
	new_node->edges = NULL;
	new_node->next = NULL;

	return 	new_node;
}

/* */
void	node_add_edge(node* n, edge* new_edge)
{
	edge*	last_edge;

	/* Start of the chained list of edges */
	if (n->edges == NULL) 
	{
		n->edges = new_edge;
	}
	else
	{
		last_edge = n->edges;

		/* Find the end of the chained list of edges */
		while (last_edge->next != NULL)
		{
			last_edge = last_edge->next;
		}	

		/* Append the new edge */
		last_edge->next = new_edge;
	}
}

edge* 	edge_create(int capacity, node* target_node)
{
	edge* 	new_edge;

	new_edge = malloc(sizeof(edge));

	new_edge->stream = 0;
	new_edge->capacity = capacity;
	new_edge->target_node = target_node;
	new_edge->next = NULL;

	return 	new_edge;
}

edge* 	edge_set_stream(edge* edge_to_set, int stream)
{
	/* The capacity must be respected */
	if (stream <= edge_to_set->capacity)
	{
		edge_to_set->stream = stream;
	}
	else
	{
		printf("error : trying to set stream higher than capacity\n");
	}

	return 	edge_to_set;
}

int		edge_can_be_improved_positive(edge* edge_to_improve)
{
	return edge_to_improve->stream < edge_to_improve->capacity;
}

int 	edge_can_be_improved_negative(edge* edge_to_improve)
{
	return edge_to_improve->stream > 0;
}

node*	node_mark_source(node* source)
{
	source->is_marked = 1;
	source->origin = source->id;

	return source;
}

void	node_mark_neighbors(graph* g, node* origin_node)
{
	edge* 	current_edge;
	node*	current_node;
	int 	i;

	/* Mark all positive neighbors */

	current_edge = origin_node->edges;

	while (current_edge != NULL)
	{
		current_node = current_edge->target_node;

		if (current_node->is_marked == 0 && edge_can_be_improved_positive(current_edge))
		{
			current_node->is_marked = 1;
			current_node->origin = origin_node->id;

			node_mark_neighbors(g, current_node);
		}

		current_edge = current_edge->next;
	} 

	/* Mark all negative neighbors */

	current_node = g->nodes;

	while (current_node != NULL) 
	{
		if (current_node->is_marked == 0)
		{
			current_edge = current_node->edges;

			while (current_edge != NULL) 
			{
				if (current_edge->target_node->id == origin_node->id && edge_can_be_improved_negative(current_edge))
				{
					current_node->is_marked = 1;
					current_node->origin = (origin_node->id * -1);

					node_mark_neighbors(g, current_node);
				}

				current_edge = current_edge->next;
			}
		}
		
		current_node = current_node->next;
	}
}

void	test_1()
{
	printf("-- FIRST TEST -- \n");

	graph*	g;
	node*	s;
	node*	a;
	node*	b;
	node*	t;

	g = graph_create();

	printf("Created nodes : s = 1, a = 2, b = 3, t = 4;\n");

	s = node_create(1);
	a = node_create(2);
	b = node_create(3);
	t = node_create(4);

	graph_add_node(g, s); 
	graph_add_node(g, a);	
	graph_add_node(g, b);	
	graph_add_node(g, t);	

	graph_log_nodes(g);

	printf("Created edges;\n");

	node_add_edge(s, edge_set_stream(edge_create(3,a),3) ); 
	node_add_edge(s, edge_set_stream(edge_create(6,b),2) ); 
	node_add_edge(b, edge_set_stream(edge_create(4,a),1) ); 
	node_add_edge(a, edge_set_stream(edge_create(6,t),4) ); 
	node_add_edge(b, edge_set_stream(edge_create(1,t),1) ); 

	graph_log_edges(g);

	printf("~ Marking graph ~\n");

	node_mark_neighbors(
		g, 
		node_mark_source(s)
	);

	graph_log_nodes(g);

	printf("-- END OF FIRST TEST -- \n");
	putchar('\n');
}

void	test_2()
{
	printf("-- SECOND TEST -- \n");

	graph*	g;
	node*	s;
	node*	a;
	node*	b;
	node*	t;

	g = graph_create();

	printf("Created nodes : s = 1, a = 2, b = 3, t = 4;\n");

	s = node_create(1);
	a = node_create(2);
	b = node_create(3);
	t = node_create(4);

	graph_add_node(g, s); 
	graph_add_node(g, a);	
	graph_add_node(g, b);	
	graph_add_node(g, t);	

	graph_log_nodes(g);

	printf("Created edges;\n");

	node_add_edge(s, edge_set_stream(edge_create(5,a),5) ); 
	node_add_edge(s, edge_set_stream(edge_create(5,b),0) ); 
	node_add_edge(a, edge_set_stream(edge_create(5,b),5) ); 
	node_add_edge(a, edge_set_stream(edge_create(5,t),0) ); 
	node_add_edge(b, edge_set_stream(edge_create(5,t),5) ); 

	graph_log_edges(g);

	printf("~ Marking graph ~\n");

	node_mark_neighbors(
		g, 
		node_mark_source(s)
	);

	graph_log_nodes(g);

	printf("-- END OF SECOND TEST -- \n");
	putchar('\n');
}

void	test_3()
{
	printf("-- THIRD TEST -- \n");

	graph*	g;
	node*	s;
	node*	a;
	node*	b;
	node*	t;

	g = graph_create();

	printf("Created nodes : s = 1, a = 2, b = 3, t = 4;\n");

	s = node_create(1);
	a = node_create(2);
	b = node_create(3);
	t = node_create(4);

	graph_add_node(g, s); 
	graph_add_node(g, a);	
	graph_add_node(g, b);	
	graph_add_node(g, t);	

	graph_log_nodes(g);

	printf("Created edges;\n");

	node_add_edge(s, edge_set_stream(edge_create(4,a),4) ); 
	node_add_edge(s, edge_set_stream(edge_create(3,b),2) ); 
	node_add_edge(a, edge_set_stream(edge_create(2,b),1) ); 
	node_add_edge(b, edge_set_stream(edge_create(2,a),1) );
	node_add_edge(a, edge_set_stream(edge_create(5,t),4) ); 
	node_add_edge(b, edge_set_stream(edge_create(2,t),2) ); 

	graph_log_edges(g);

	printf("~ Marking graph ~\n");

	node_mark_neighbors(
		g, 
		node_mark_source(s)
	);

	graph_log_nodes(g);

	printf("-- END OF THIRD TEST -- \n");
	putchar('\n');
}

int 	main(int argc, char** argv)
{
	test_1();
	test_2();
	test_3();
	
	exit(EXIT_SUCCESS);
}