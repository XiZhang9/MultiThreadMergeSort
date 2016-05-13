#pragma once


/** @brief Merge the right and left halves together to become a sorted array.
 *  @param Address to the first element in left half.
 *  @param The size of left half array.
 *  @param Address to the first element in right half.
 *  @param The size of right half array.
 *  @return void.
 */
void Merge(int  *left, int sizeLeft,int  *right, int sizeRight);

/** @brief MergeSort function to recursively split the number array to lift half and right half.
*   Call fork functions to create children processes and call merge function to merge them together.
 *  @param The size of number array.
 *  @param The addres of the first element in the number array.
 *  @return void.
 */
void MergeSort(int *ToBeSort, int size);
