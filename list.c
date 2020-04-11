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
bool isListSorted(Node);
// create new Node with a given value
Node createNode(int);
// get the next Node in list
Node getNextNode(Node);
// set the next node in list
void setNextNode(Node, Node);
// get value of node
int getVal(Node);
// given an array of integers, create a list of nodes containing each integer
ErrorCode createListByNumsArr(Node *, int *, int);
// free every node of the list
void destroyList(Node);
// validate given list in order to exercise requirements: check they are sorted, not empty, and not null
ErrorCode validateLists(Node, Node, Node *);
// given two sorted lists, create an array containing the combined numbers sirted
ErrorCode createMergedArray(Node, Node, int **, int *);
// read a list and create an array containing its integers
ErrorCode getNumsArrayFromList(Node, int *, int **);
// insert node to list and increment pointer to node to the new node
ErrorCode insertNodeToList(int, Node *);
// given 2 sorted arrays, merge them into a single sorted array
void mergeArrays(int *, int *, int *, int, int);
ErrorCode mergeSortedLists(Node list1, Node list2, Node *merged_out);

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

ErrorCode getNumsArrayFromList(Node list, int *array_len, int **ptr_array)
{
	*array_len = getListLength(list);
	*ptr_array = malloc(sizeof(int) * (*array_len));
	if (*ptr_array == NULL) {
		return MEMORY_ERROR;
	}
	for (int i = 0; i < *array_len; i++) {
		if (list == NULL) {
			free(*ptr_array);
			return MEMORY_ERROR;
		}
		(*ptr_array)[i] = getVal(list);
		list = getNextNode(list);
	}
	return SUCCESS;
}

Node createNode(int val)
{
	Node node = malloc(sizeof(*node));
	if (node == NULL) {
		return NULL;
	}
	node->x = val;
	node->next = NULL;
	return node;
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

void destroyList(Node list)
{
	Node current_node = list, next_node;
	while (current_node != NULL) {
		next_node = getNextNode(current_node);
		free(current_node);
		current_node = next_node;
	}
}

ErrorCode insertNodeToList(int val, Node *list)
{
	Node new_node = createNode(val);
	if (new_node == NULL) {
		return MEMORY_ERROR;
	}
	if (*list == NULL) {
		*list = new_node;
	} else {
		setNextNode(*list, new_node);
	}
	*list = new_node;
	return SUCCESS;
}

void mergeArrays(int *array1, int *array2, int *res_arr, int len1, int len2)
{
	int index1 = 0, index2 = 0, index3 = 0;
	while (index1 < len1 && index2 < len2) {
		if (array1[index1] < array2[index2]) {
			res_arr[index3] = array1[index1];
			index1++;
		} else {
			res_arr[index3] = array2[index2];
			index2++;
		}
		index3++;
	}
	while (index1 < len1) {
		res_arr[index3] = array1[index1];
		index1++;
		index3++;
	}
	while (index2 < len2) {
		res_arr[index3] = array2[index2];
		index2++;
		index3++;
	}
}

ErrorCode createListByNumsArr(Node *list, int *nums, int nums_len)
{
	Node first_node = createNode(nums[0]);
	*list = first_node;
	for (int i = 1; i < nums_len; i++) {
		if (insertNodeToList(nums[i], list) != SUCCESS) {
			destroyList(*list);
			*list = NULL;
			return MEMORY_ERROR;
		}
	}
	*list = first_node;
	return SUCCESS;
}

ErrorCode createMergedArray(Node list1, Node list2, int **ptr_res_err, int *result_array_len)
{
	int len1, len2, *array1, *array2;

	if (getNumsArrayFromList(list1, &len1, &array1) != SUCCESS) {
		return MEMORY_ERROR;
	}
	if (getNumsArrayFromList(list2, &len2, &array2) != SUCCESS) {
		free(array1);
		return MEMORY_ERROR;
	}
	*result_array_len = len1 + len2;
	*ptr_res_err = malloc(sizeof(int) * (*result_array_len));
	if (*ptr_res_err == NULL) {
		free(array1);
		free(array2);
		return MEMORY_ERROR;
	}
	mergeArrays(array1, array2, *ptr_res_err, len1, len2);
	free(array1);
	free(array2);
	return SUCCESS;
}

ErrorCode mergeSortedLists(Node list1, Node list2, Node *merged_out)
{
	ErrorCode status = validateLists(list1, list2, merged_out);
	if (status != SUCCESS) {
		return status;
	}
	*merged_out = NULL;
	int *merged_num_array, final_len;

	if (createMergedArray(list1, list2, &merged_num_array, &final_len) != SUCCESS) {
		return MEMORY_ERROR;
	}
	status = createListByNumsArr(merged_out, merged_num_array, final_len);
	free(merged_num_array);
	return status;
}

