#include <stdbool.h>
#include <stdlib.h>
typedef struct node_t {
	int x;
	struct node_t *next;
} * Node;

typedef enum {
	SUCCESS = 0,
	MEMORY_ERROR,
	EMPTY_LIST,
	UNSORTED_LIST,
	NULL_ARGUMENT,
} ErrorCode;


int getListLength(Node);

Node createNode()
{
	Node node = malloc(sizeof(*node));
	if (node == NULL) {
		return NULL;
	}
	node->next = NULL;
	return node;
}

void destroyList(Node list)
{
	Node current_node = list, next_node;
	while (current_node != NULL) {
		next_node = current_node->next;
		free(current_node);
		current_node = next_node;
	}
}


ErrorCode new_mergeSortedLists(Node list1, Node list2, Node *merged_out)
{
	if (list1 == NULL || list2 == NULL) {
		return NULL_ARGUMENT;
	}

	*merged_out = createNode();
	if (merged_out == NULL) {
		return MEMORY_ERROR;
	}

	int list1_len = getListLength(list1), list2_len = getListLength(list2);
	if (list1_len < 1 && list2_len < 1) {
		return EMPTY_LIST;
	}


	int i = 0, j = 0;
	Node temp1 = list1, temp2 = list2;
	Node new_list = *merged_out;
	while (i < list1_len && j < list2_len) {
		if (temp1->x < temp2->x) {
			new_list->x = temp1->x;
			i++;
		} else {
			new_list->x = temp2->x;
			j++;
		}
		new_list->next = createNode();
		new_list = new_list->next;
	}
	while (i < list1_len) {
		new_list->x = temp1->x;
		i++;
		new_list->next = createNode();
		new_list = new_list->next;
	}
	while (j < list2_len) {
		new_list->x = temp2->x;
		j++;
		new_list->next = createNode();
		new_list = new_list->next;
	}

	destroyList(new_list); // remove last empty node
	return SUCCESS;
}