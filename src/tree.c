#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

TreeNode* init_node(GameState* gs){
    TreeNode* ret = (TreeNode*) malloc(sizeof(TreeNode));

    ret->num_children = -1;
    ret->game_state = gs;
    ret->children = NULL;

    return ret;
}

TreeNode* init_tree(GameState* gs, int depth){
    TreeNode* root = init_node(gs);

    if(get_game_status(root->game_state) != IN_PROGRESS){
        return root;
    }

    if(depth == 1){
        return root;
    }
    
    bool moves[gs->width];
    for(int i = 0; i < root->game_state->width; i++){
        moves[i] = false;
    }
    root->num_children = available_moves(gs, moves);
    root->children = (TreeNode**) malloc(sizeof(TreeNode*) * root->num_children);

    int count = 0;
    for(int i = 0; i < gs->width; i++){
        if(!moves[i]){
            continue;
        }
        root->children[count++] = init_tree(make_move(root->game_state, i), depth - 1);
    }

    return root;
}

void free_tree(TreeNode* root){
    for(int i = 0; i < root->num_children; i++){ 
        free_tree(root->children[i]);
    }
    free(root->children);
    root->children = NULL;
    free_game_state(root->game_state);
    root->game_state = NULL;
    free(root);
    root = NULL;
}

void expand_tree(TreeNode* root){
    if(get_game_status(root->game_state) != IN_PROGRESS){
        return;
    }

    if (root->num_children <= 0){
        
        bool moves[root->game_state->width];
        for(int i = 0; i < root->game_state->width; i++){
            moves[i] = false;
        }
        root->num_children = available_moves(root->game_state, moves);
        root->children = (TreeNode**) malloc(sizeof(TreeNode*) * root->num_children);

        int count = 0;
        for (int i = 0; i < root->game_state->width; i++){
            if(moves[i]){
                root->children[count++] = init_node(make_move(root->game_state, i));
            }  
        }
        return;
    }

    for(int i = 0; i < root->num_children; i++){
        expand_tree(root->children[i]);
    }
}

int node_count(TreeNode* root){
    int count = 1;

    for (int i = 0; i < root->num_children; i++){
        count += node_count(root->children[i]);
    }
    
    return count;
}

void print_tree(TreeNode* root, int level){
    printf("%d ", root->game_state->evaluation);

    for (int i = 0; i < root->num_children; i++){
        print_tree(root->children[i], 0);
    }
    printf("\n");  
}