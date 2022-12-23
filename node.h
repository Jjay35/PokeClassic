#include <stdio.h>
#include <stdlib.h>

#include <vector>

typedef struct Nodes node_n;
typedef struct Nodes{
    int cost;
    int dist;
    int currX;
    int currY;
    int previousX;
    int previousY;
    char symbol;
    int direction; 


}node_n;

typedef struct MinHeap MinHeap;
struct MinHeap {
    node_n* arrNodes;
    // Current Size of the Heap
    int size;
    // Maximum capacity of the heap
    int capacity;
};
MinHeap* delete_minimum(MinHeap* heap);
void delete_element(MinHeap* heap, int index);
void min_heap(MinHeap* heap, int index, int distance);


int parent(int i) {
    // Get the index of the parent
    return (i - 1) / 2;
}

int left_child(int i) {
    return (2*i + 1);
}

int right_child(int i) {
    return (2*i + 2);
}

node_n get_min(MinHeap* heap) {
    // Return the root node element,
    // since that's the minimum
    return heap->arrNodes[0];
}

MinHeap* init_minheap(int capacity) {
    MinHeap* minheap = (MinHeap*) calloc (1, sizeof(MinHeap));
    minheap->arrNodes = (node_n*) calloc (capacity, sizeof(node_n));
    minheap->capacity = capacity;
    minheap->size = 0;
    return minheap;
}

MinHeap* insert_minheap(MinHeap* heap, node_n node) {
    // Inserts an element to the min heap
    // We first add it to the bottom (last level)
    // of the tree, and keep  with it's parent
    // if it is lesser than it. We keep doing that until
    // we reach the root node. So, we will have inserted the
    // element in it's proper position to preserve the min heap property
    if (heap->size == heap->capacity) {
        fprintf(stderr, "Cannot insert %d. Heap is already full!\n", node);
        return heap;
    }
    // We can add it. Increase the size and add it to the end
    heap->size++;
    heap->arrNodes[heap->size - 1] = node;

    // Keep swapping until we reach the root
    int curr = heap->size - 1;
    // As long as you aren't in the root node, and while the 
    // parent of the last element is greater than it
    while (curr > 0 && heap->arrNodes[parent(curr)].dist  > heap->arrNodes[curr].dist) {
        // Swap
        node_n tempNode = heap->arrNodes[parent(curr)];
        heap->arrNodes[parent(curr)] = heap->arrNodes[curr];
        heap->arrNodes[curr] = tempNode;
        // Update the current index of element
        curr = parent(curr);
    }
    return heap; 
}

void delete_element(MinHeap* heap, int index){
    min_heap(heap, index, 0);
    delete_minimum(heap);
}

void min_heap(MinHeap* heap, int index, int distance){
    node_n tempNode;
    heap->arrNodes[index].dist = distance;
    while(index != 0 && heap->arrNodes[parent(index)].dist> heap->arrNodes[index].dist){
        tempNode = heap->arrNodes[index];
        heap->arrNodes[index] = heap->arrNodes[parent(index)];
        heap->arrNodes[parent(index)] = tempNode;
        index = parent(index);
    }

}

MinHeap* heapify(MinHeap* heap, int index) {
    // Rearranges the heap as to maintain
    // the min-heap property
    if (heap->size <= 1)
        return heap;
    
    int left = left_child(index); 
    int right = right_child(index); 

    // Variable to get the smallest element of the subtree
    // of an element an index
    int smallest = index; 
    
    // If the left child is smaller than this element, it is
    // the smallest
    if (left < heap->size && heap->arrNodes[left].dist  < heap->arrNodes[index].dist) 
        smallest = left; 
    
    // Similarly for the right, but we are updating the smallest element
    // so that it will definitely give the least element of the subtree
    if (right < heap->size && heap->arrNodes[right].dist  < heap->arrNodes[smallest].dist) 
        smallest = right; 

    // Now if the current element is not the smallest,
    // swap with the current element. The min heap property
    // is now satisfied for this subtree. We now need to
    // recursively keep doing this until we reach the root node,
    // the point at which there will be no change!
    if (smallest != index) 
    { 
        node_n tempNode = heap->arrNodes[index];
        heap->arrNodes[index] = heap->arrNodes[smallest];
        heap->arrNodes[smallest] = tempNode;
        heap = heapify(heap, smallest); 
    }

    return heap;
}

MinHeap* delete_minimum(MinHeap* heap) {
    // Deletes the minimum element, at the root
    if (!heap || heap->size == 0)
        return heap;

    int size = heap->size;
    node_n last_element = heap->arrNodes[size-1];
    
    // Update root value with the last element
    heap->arrNodes[0] = last_element;

    // Now remove the last element, by decreasing the size
    heap->size--;
    size--;

    // We need to call heapify(), to maintain the min-heap
    // property
    heap = heapify(heap, 0);
    return heap;
}

void decreaseKey(MinHeap* heap, int coordx, int coordy, int distance){
  int i;
  int location;
  for(i = 0; i < heap->size; i++){
    if(heap->arrNodes[i].currX == coordx && heap->arrNodes[i].currY == coordy){
        location = i;
    }
  }
  heap->arrNodes[location].dist = distance;
  
  node_n tempNode;
  while(location != 0 && heap->arrNodes[parent(location)].dist > heap->arrNodes[location].dist){
   
        tempNode = heap->arrNodes[location];
        heap->arrNodes[location] = heap->arrNodes[parent(location)];
        heap->arrNodes[parent(location)] = tempNode;
        location = parent(location);
    
  }


//   printf("Neighbor coordinates(%d, %d) %d\n", heap->arrNodes[location].currX,heap->arrNodes[location].currY, heap->arrNodes[location].dist);
}


void print_heap(MinHeap* heap) {
    // Simply print the array. This is an
    // inorder traversal of the tree
    printf("Min Heap:\n");
    for (int i=0; i< heap->size; i++) {
        printf("%d -> ", heap->arrNodes[i]);
    }
    printf("\n");
}

void free_minheap(MinHeap* heap) {
    if (!heap)
        return;
    free(heap->arrNodes);
    free(heap);
}

// int main() {
//     // Capacity of 10 elements
//     MinHeap* heap = init_minheap(10);

//     // insert_minheap(heap, 40);
//     // insert_minheap(heap, 50);
//     // insert_minheap(heap, 5);
//     // print_heap(heap);ß
    
//     // // Delete the heap->arr[1] (50)
//     // delete_element(heap, 1);

//     // print_heap(heap);
//     // free_minheap(heap);
//     return 0;
// }
