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

int getListLength(Node list);
bool isListSorted(Node list);
ErrorCode mergeSortedLists(Node list1, Node list2, Node *merged_out);
// create new empty node
Node createNode();
// free every node in list
void destroyList(Node list);
// validate given lists according to exercise requirements
ErrorCode validateLists(Node list1, Node list2, Node *list3);
// get next node in list
Node getNextNode(Node list);
// set next node in list
void setNextNode(Node node, Node next_node);
// get value of node
int getVal(Node node);
// set value of node
void setVal(Node node, int val);
// copy a number of node (len argument) from source list to destination list starting from a given node
ErrorCode copyList(Node dest, Node source, int len);

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

ErrorCode validateLists(Node list1, Node list2, Node *list3)
{
	if (list3 == NULL) {
		return NULL_ARGUMENT;
	}
	if (list1 == NULL || list2 == NULL || getListLength(list1) < 1 || getListLength(list2) < 1) {
		*list3 = NULL;
		return EMPTY_LIST;
	}
	if (!isListSorted(list1) || !isListSorted(list2)) {
		*list3 = NULL;
		return UNSORTED_LIST;
	}
	return SUCCESS;
}

Node getNextNode(Node list)
{
	return list->next;
}

void setNextNode(Node node, Node next_node)
{
	node->next = next_node;
}

int getVal(Node node)
{
	return node->x;
}

void setVal(Node node, int val)
{
	node->x = val;
}

ErrorCode copyList(Node dest, Node source, int len)
{
	int i = 0;
	while (i < len) {
		setVal(dest, getVal(source));
		i++;
		if (i != len) {
			setNextNode(dest, createNode());
			if (getNextNode(dest) == NULL) {
				return MEMORY_ERROR;
			}
			source = getNextNode(source);
			dest = getNextNode(dest);
		}
	}
	return SUCCESS;
}

ErrorCode mergeSortedLists(Node list1, Node list2, Node *merged_out)
{
	ErrorCode status = validateLists(list1, list2, merged_out);
	if (status != SUCCESS) {
		return status;
	}
	int list1_len = getListLength(list1), list2_len = getListLength(list2), i = 0, j = 0;
	Node current_node = createNode();
	if (current_node == NULL) {
		return MEMORY_ERROR;
	}
	*merged_out = current_node;
	while (i < list1_len && j < list2_len) {
		if (getVal(list1) < getVal(list2)) {
			setVal(current_node, getVal(list1));
			list1 = getNextNode(list1);
			i++;
		} else {
			setVal(current_node, getVal(list2));
			list2 = getNextNode(list2);
			j++;
		}
		setNextNode(current_node, createNode());
		if (getNextNode(current_node) == NULL) {
			destroyList(*merged_out);
			return MEMORY_ERROR;
		}
		current_node = getNextNode(current_node);
	}
	if (copyList(current_node, list1, list1_len - i) != SUCCESS ||
		copyList(current_node, list2, list2_len - j) != SUCCESS) {
		destroyList(*merged_out);
		return MEMORY_ERROR;
	}
	return SUCCESS;
}