/*
 * Author: Sean W Jasin (sjasi3)
 * UIN: 672969187
 * Date: 3AM 9 NOV '23
 * Description:
 *  A demonstration of the skills learned in ECE220. This MP's purpose is to
 * parse a file with matrix values and store them efficiently within the format
 * that matlab apparently uses. Each function has a high level description above
 * it as well as line by line descriptions of what each thing does
 */
#include "sparsemat.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * Calculate the index of a 2d matrix as a 1d position
 * mat_t holds matrix
 * row is row
 * col is column
 */
int get_pos(sp_tuples *mat_t, int row, int col) {
    return row * mat_t->n + col;
}

/*
 * Deletes a tuples from matrix
 * mat_t holds matrix
 * iter is the address which we will use to iterate thru all of matrix
 * row and col is the respective positions which we will try to delete
 */
void del_tuple(sp_tuples *mat_t, sp_tuples_node **iter, int row, int col) {
    if(*iter == NULL) {                                         // If the position val was NULL there is nothing to delete
        return;                                                 // Return
    }
    int iter_idx = get_pos(mat_t, (*iter)->row, (*iter)->col);  // Iterator index
    sp_tuples_node *rem;                                        // Ptr to remove

    if(iter_idx > get_pos(mat_t, row, col)) {                   // We have passed the objective row and col, we dont have to look further
        return;                                                 // Return
    }

    if(iter_idx == get_pos(mat_t, row, col)) {                  // If the position is the same as the iterator index
        rem = *iter;                                            // Store the node to remove (iter)
        (*iter) = (*iter)->next;                                // Set the value of *iter to the next value
        free(rem);                                              // Free rem which points to the old *iter
        mat_t->nz--;                                            // Update non zeros
        return;                                                 // Return
    }

    del_tuple(mat_t, &((*iter)->next), row, col);               // Recursively check
    return;
}

/*
 * Inserts a tuple into a matrix
 * mat_t holds matrix
 * iter is the address which we will use to iterate thru all of matrix
 * node is the node we want to insert
 */
void ins_tuple(sp_tuples *mat_t, sp_tuples_node **iter, sp_tuples_node *node) {
    if(*iter == NULL) {                                         // If the ptr at the ptr is NULL pos is empty
        *iter = node;                                           // Update pos
        (*iter)->next = NULL;                                   // Update next pos
        mat_t->nz++;                                            // Update non zeros
        return;                                                 // Return, we found the insertion point
    }

    int iter_idx = get_pos(mat_t, (*iter)->row, (*iter)->col);  // iterator index
    int node_idx = get_pos(mat_t, node->row, node->col);        // item index of node

    if(iter_idx > node_idx) {                                   // If iter index greater than node index
        sp_tuples_node *next = *iter;                           // Store next pos
        *iter = node;                                           // Update pos
        (*iter)->next = next;                                   // Update next pos
        mat_t->nz++;                                            // Update non zeros
        return;                                                 // Return, we found the insertion point
    }
    if(iter_idx == node_idx) {
        (*iter)->value = node->value;                           // Update pos
        free(node);                                             // Free the memory
        return;                                                 // Return, we found the insertion point
    }
    ins_tuple(mat_t, &((*iter)->next), node);
    return;
}

/*
 * Loads the tuples from a file
 */
sp_tuples * load_tuples(char* input_file)
{
    FILE *f = fopen(input_file, "r");                           // Load the file

    sp_tuples *mat_t = (sp_tuples*)malloc(sizeof(sp_tuples));   // Allocate memory for sp_tuples
    fscanf(f, "%d %d", &(mat_t->m), &(mat_t->n));               // Get the respective size

    mat_t->nz = 0;                                              // Non zeros is zero from the start
    int row, col;                                               // Will be used to store row and col from file
    double value;                                               // Will be used to store value from file

    while(!feof(f)) {                                           // While not EOF
        fscanf(f, "%d %d %lf",                                  // Store data in a buffer
                         &row,
                         &col,
                         &value);
        set_tuples(mat_t, row, col, value);                     // Set tuples
    }
    fclose(f);                                                  // Close files
    return mat_t;                                               // Return the matrix
}

/*
 * Returns the value of the given index (row, col)
 */
double gv_tuples(sp_tuples * mat_t,int row,int col)
{
    sp_tuples_node *cur_node = mat_t->tuples_head;              // Set current node location
    while(cur_node != NULL) {                                   // While the current node is not NULL
        if(cur_node->row == row && cur_node->col == col) {      // If row and col match
            return cur_node->value;                             // Return value of current node
        }
        cur_node = cur_node->next;                              // Go to next node
    }
    return 0;                                                   // 0 val
}

/*
 * Sets the tuple value at the respective row and col
 */
void set_tuples(sp_tuples * mat_t, int row, int col, double value)
{
    sp_tuples_node *tuple_buffer =                              // Allocate memory for buffer
            (sp_tuples_node*)malloc(sizeof(sp_tuples_node));

    tuple_buffer->row = row;                                    // Put into buffer
    tuple_buffer->col = col;
    tuple_buffer->value = value;

    if(tuple_buffer->value == 0) {                              // If the value is zero
        del_tuple(mat_t, &(mat_t->tuples_head),                 // Delete if exists
                  tuple_buffer->row, tuple_buffer->col);
        free(tuple_buffer);                                     // Free buffer memory
        return;
    }
    ins_tuple(mat_t, &(mat_t->tuples_head), tuple_buffer);      // Insert value into matrix
    return;
}

/*
 * Store the matrix inside a file
 */
void save_tuples(char * file_name, sp_tuples * mat_t)
{
    if(mat_t == NULL) {                                         // If mat_t is NULL
        return;                                                 // Return early, nothing to write
    }
    FILE *f = fopen(file_name, "w");                            // Open file in "write" mode
    sp_tuples_node *cur_node = mat_t->tuples_head;              // Load the first node
    fprintf(f, "%d %d\n", mat_t->m, mat_t->n);                  // Put row col info in file
    while(cur_node != NULL) {                                   // Until node is NULL
        fprintf(f, "%d %d %lf\n",                               // Write the values to the file
                cur_node->row,
                cur_node->col,
                cur_node->value);
        cur_node = cur_node->next;                              // Go to next node
    }
	return;
}

/*
 * Add two matrices together, matA and matB
 */
sp_tuples * add_tuples(sp_tuples * matA, sp_tuples * matB){
    if(matA->m != matB->m || matA->n != matB->m) {              // If they are not the same dimen
        return NULL;                                            // Cannot add, return
    }

    sp_tuples *retmat = (sp_tuples*)malloc(sizeof(sp_tuples));  // return matrix
    retmat->m = matA->m;                                        // set same dimen
    retmat->n = matA->n;                                        // set same dimen
    retmat->nz = matA->nz;                                      // set same amnt of non zeros
    
    sp_tuples_node *node = matA->tuples_head;                   // set node to tuples head of A
    while(node != NULL) {                                       // While node is not NULL
        set_tuples(retmat, node->row, node->col, node->value);  // Add to return matrix
        node = node->next;                                      // keep iterating
    }

    node = matB->tuples_head;                                   // set node to tuples head of B
    while(node != NULL) {                                       // While node is not NULL
        set_tuples(retmat, node->row, node->col, node->value + 
                   gv_tuples(retmat, node->row, node->col));    // Add to return matrix
        node = node->next;                                      // keep iterating
    }

	return retmat;
}

/*
 * Did not implement
 */
sp_tuples * mult_tuples(sp_tuples * matA, sp_tuples * matB){ 
    return NULL;
}

/*
 * Frees the memory of matrices
 */
void destroy_tuples(sp_tuples * mat_t){
    if(mat_t != NULL) {                                     // Check if NULL first
        sp_tuples_node * cur_node = mat_t->tuples_head;     // Stores current node
        sp_tuples_node * pre_node = NULL;                   // Stores previous node

        while(cur_node != NULL) {                           // Go to next node
            pre_node = cur_node;                            // Store previous node so we can free
            cur_node = cur_node->next;                      // Go to next node
            free(pre_node);                                 // Free the last node
        }
        free(mat_t);                                        // Me entirety
    }
    return;
}  

