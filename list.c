#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INTS 1000

/* A node in the linked list */
struct node {
	int data;
	struct node *next;
};

struct node* create_list(int intarray[], int len);
struct node* add_item_at_start(struct node *head, int data);
int search_array(int integers[], int numints, int element);
int search_list(struct node *head, int element);
struct node* create_sorted_list(struct node *head);
struct node* add_item_sorted(struct node *head, int data);
int copy_list_to_array(struct node *head, int *array);
void print_list(struct node *head);
void print_array(int integers[], int len);


int main(int argc, char *argv[])
{
	FILE *int_file;
	int num[MAX_INTS];
	int i;

	/* TODO: Complete the main method according to the steps outlined */
	/* Open a file for reading */
	int_file = fopen("numbers.txt", "r");

	/* Read the numbers from the file, into an array */
	/* Print the array */

	printf("ARRAY: |");
	while(!feof(int_file)) {
		fscanf(int_file, "%d", &(num[i]));
		printf(" %d |", num[i]);
		++i; 
	}
	printf("\n"); 

	fclose(int_file);

	// i is the length of num


	/* Create a linked list with the integers from the array */
	/* Print the linked list */
	struct node *ans;
	ans = create_list(num, i);
	printf("LINKED LIST:");
	print_list(ans);



	/* Repeatedly prompt the user for a number to be searched.
	*  Search the array for the number and print out the result as shown in the specs.
	*  Search the linked list for the number and print out the result as shown in the specs.
	*  Stop prompting when the user enters 'q' (just the character q without the single quotes).
	*/

	int element1 = 58;
	int element2 = 1;

	int ans1_array = search_array(num, i, element1);
	int ans2_array = search_array(num, i, element2);

	int ans1_list = search_list(ans, element1);
	int ans2_list = search_list(ans, element2);
	
	printf("array1 list1 array2 list2 : %d %d %d %d\n", ans1_array, ans1_list, ans2_array, ans2_list);	

	/* Create a sorted list(in ascending order) 
	from the unsorted list */
	struct node *ans_sorted;
	ans_sorted =  create_sorted_list(ans);
	print_list(ans_sorted);

	/* Print the sorted list */

	/* Copy the sorted list to an array 
	with the same sorted order */

	/* Print out the sorted array */    

	/* Print the original linked list again */

	/* Print the original array again */


	/* Open a file for writing */

	/* Write the sorted array to a file named "sorted_numbers.txt" */

	/* Print out the number of integers written to the file */



	return 0;

}

struct node* create_list(int intarray[], int len)
{
	/* TODO: Complete this function */ 
	// like creating head.
	struct node *head;
	struct node *res;
	int i;

	head = (struct node *)malloc(sizeof(struct node));
	head->data = intarray[len-1];
	head->next = NULL;

	for( i = len-2; i > -1; --i ) {
		res = add_item_at_start(head, intarray[i]);
	}

	return res;
	
}


struct node* add_item_at_start(struct node *head, int data)
{
	/* TODO: Complete this function */
		struct node *addnode, *index;
		addnode = (struct node *)malloc(sizeof(struct node));
		index = (struct node *)malloc(sizeof(struct node));
		index = head;

		while(index->next != NULL)
			index = index->next;	// indexを動かして、最後を探りあてて、最後にデータを入れる
	 
		index->next = addnode;
		addnode->data = data;
	 	addnode->next = NULL;
		return head;	
}

int search_list(struct node *head, int element)
{
	/* TODO: Complete this function */
	// struct node *x;
	// x = (struct node *)malloc(sizeof(struct node));
	int position = 0;

	while(head->next != NULL) {
		if(head->data == element)
			return position;
		++position;
		head = head->next;
	}
	return -1;
}

int search_array(int integers[], int numints, int element)
{
	/* TODO: Complete this function */
	int i;
	for(i = 0; i < numints; i++ )
		if ( integers[i] == element)
			return i;
	
	return -1;
}

int copy_list_to_array(struct node *head, int *array)
{
	/* TODO: Complete this function */
}

struct node* create_sorted_list(struct node *head)
{
	/* TODO: Complete this function */
	struct node *ans, *pNow, *pNew;

	ans = (struct node *)malloc(sizeof(struct node));

	ans -> data = head->data; // ansは回答側のHeadを指すアドレス
	ans -> next = NULL;
	head = head->next; // initiation

	while(head->next != NULL) {
		pNow = (struct node *)malloc(sizeof(struct node));
		pNew = (struct node *)malloc(sizeof(struct node));
		pNew -> data = head->data; // pNewが入れるべきデータ。next =NULL

		if(ans->data > pNew->data) { //入れるデータが先頭よりも小さい
			struct node *tmp;
			tmp = ans;
			ans = pNew;
			ans->next = tmp;
		} else { // どこにデータを挿入すべきか探索する。ないならTail
			pNow = ans; // indexを初期化（Ans側で）
			while(pNow->next != NULL) {
				if (pNow->data <= pNew->data && pNew->data < pNow->next->data) {
					pNew->next = pNow->next;
					pNow->next = pNew;
					break;
				}
				pNow = pNow->next;	
			}
 		}
	head = head->next;	
	}
	return ans;
}


struct node* add_item_sorted(struct node *sorted_head, int data)
{
	/* TODO: Complete this function */
}

void print_list(struct node *head)
{
	if (head == NULL) {
		printf("Linked List is Empty.\n");
	} else {
		struct node *temp = head;
		printf("head->");
		while (temp != NULL) {
			printf("|%d|->", temp->data);
			temp = temp->next;
		}
		printf("NULL\n");
	}
}

void print_array(int integers[], int len)
{
	int i;
	for (i = 0; i < len; i++) {
		printf("| %d ", integers[i]);
	}
	printf("|\n");
}
