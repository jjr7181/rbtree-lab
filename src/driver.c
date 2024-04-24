#include "rbtree.h"
#include "rbtree.c"
#include <stdio.h>
#include <stdlib.h>




int main(int argc, char *argv[])
{
    int c, i;
	c = 1;

    rbtree *t = new_rbtree();
    node_t *yes = NULL;

	printf("1: Insert an integer into the Red-Black tree\n");
	printf("2: Find an integer from the Red-Black tree\n");
	printf("3: Find a minimum value from the Red-Black tree\n");
	printf("4: Find a maximum value from the Red-Black tree\n");
	printf("5: Delete an integer from the Red-Black tree\n");
	printf("0: Quit\n");


	while (c != 0)
	{
		printf("Please input your choice(1/2/3/4/5/0): ");
		scanf("%d", &c);

		switch (c)
		{
		case 1:
			printf("Input an integer that you want to insert into the Red-Black Tree: ");
			scanf("%d", &i);
			rbtree_insert(t, i);
            printf("Resulting tree is: ");
            printTree(t,t->root);
            printf("\n");

            printGraph(t, t->root, 0, 0);
            printf("\n"); 
			break;
		case 2:
			printf("Input an integer that you want to find from the Red-Black Tree: ");
            scanf("%d", &i);
            yes = rbtree_find(t, i);
            if (yes != NULL)
            {
    			printf("Found it! key: %d", yes->key);
                if (yes->color == RBTREE_BLACK){
                    printf("(Black)\n");
                }
                else{
                    printf("(Red)\n");
                }
            }
            else{
                printf("Value not found. Try another integer.\n");
            }
			break;
        case 3:
            yes = rbtree_min(t);
            if (t != NULL){
    		    printf("The minimum value is: %d", yes->key);
            }
            else{
                printf("Value not found");
            }

            if (yes->color == RBTREE_BLACK){
                printf("(Black)\n");
            }
            else{
                printf("(Red)\n");
            }
            
			break;
         case 4:
            yes = rbtree_max(t);
            if (t != NULL){
    		    printf("The maximum value is: %d", yes->key);
            }
            else{
                printf("Value not found");
            }

            if (yes->color == RBTREE_BLACK){
                printf("(Black)\n");
            }
            else{
                printf("(Red)\n");
            }
            
			break;
        case 5:
            printf("Input an integer that you want to delete from the Red-Black Tree: ");
            scanf("%d", &i);
            yes = rbtree_find(t, i);
            if (yes == NULL)
            {
                printf("Value not found. Try another integer.\n");
                break;
            }
            
            printf("Deleted \"%d\"", yes->key);
            if (yes->color == RBTREE_BLACK){
                printf("(Black)\n");
            }
            else{
                printf("(Red)\n");
            }

            rbtree_erase(t, yes);
            printf("Resulting tree is: ");
            printTree(t,t->root);
            printf("\n");
            printGraph(t, t->root, 0, 0);
            printf("\n");  
			break;            
		case 0:
			delete_rbtree(t);
    		printf("Bye ^^\n");
			break;
		default:
			printf("Choice unknown;\n");
			break;
		}
	}

	return 0;
}