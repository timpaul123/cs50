/*
* Practice using linked lists
*/

#include <cs50.h>
#include <stdio.h>
#include <stdbool.h>

// create a data structure containing an int and a pointer to the next structure
typedef struct node
{
    int number;
    struct node* next;
}node;

// function prototypes
bool insert_node(int vale);
void print_nodes(node* list);
void delete_node(node* list);

// global variable for the head of the list
node* first_node = NULL;

int main(void)
{
    while(true)
    {
        printf("Enter an option, (0,1,2)\n");
        printf("0 = quit\n1 = insert a new number into the list\n2 = print the current list\n");
        int choice = GetInt();
        
        switch(choice)
        {
            case 0:
                delete_node(first_node);
                printf("Deletion complete!\n");
                return 0;
            case 1:
                printf("Please enter an integer: ");
                int new_value = GetInt();
                char* success = insert_node(new_value) ? "was" : "was not";
                printf("insertion %s successful\n", success);
                break;
            case 2:
                print_nodes(first_node);
                break;
            default:
                printf("not a valid option\n");
                break;
        }
    }
}
/*
*
* inserts a new node in the list
*
*/
bool insert_node(int value)
{
    node* new_node = malloc(sizeof(node));
    
    if(new_node == NULL)
    {
        return false;
    }
    
    new_node -> number = value;
    
    // first insertion if the list does not exist yet
    if(first_node == NULL)
    {
        new_node -> next = first_node;
        first_node = new_node;
        return true;
    }
    
    node* current = first_node;
    node* previous = NULL;
    
    // traverse the list and find the location to insert
    while(current != NULL)
    {
        // ensure duplicates are not added
        if(current -> number == value)
        {
            free(new_node);
            return false;
        }
        
        else if(current -> number > value)
        {
            previous = current;
            current = current->next;
            
            if(current == NULL)
            {
                break;
            }
        }
        
        else if(current -> number < value)
        {
            break;
        }
    }
    
    new_node -> next = current;
    
    if(previous == NULL) 
    {
        first_node = new_node;
    }
    else
    {
        previous -> next = new_node; // set the pointer of the previous node to point to the new node.
    }
    
    return true;
}

/*
*
* prints all nodes in a list
*
*/
void print_nodes(node* list)
{
    int counter = 1;
    
    node* current = first_node;
    
    while(current != NULL)
    {
        printf("Node %i: %i\n", counter, current -> number);
        counter++;
        current = current -> next;
    }
}

/*
*
* Deletes all nodes in a list
*
*/
void delete_node(node* list)
{
    node* current = first_node;
    node* previous = NULL;
    
    while(current != NULL)
    {
        previous = current; // set the "pointer" to the previous node
        current = current -> next; // move one node forward in the list
        free(previous); // delete the previous node
    }
}
