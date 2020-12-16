# GameOfLife
Conway's Game of life in C (Beginner project)

Needed:
  1. Draw grid to screen with raylib.
    -Implement zooming and panning
        -https://www.youtube.com/watch?v=ZQ8qtAizis4&ab_channel=javidx9
  
  2. Fix 2D arrays to grig.
    -Implement indexing for arays
      -For example 50*50 array is indexed at 0,0, neighbor is at 1,0
    -when indexing is done, fix arrays to grid
  
  3. Implement rendering based on arrays.
    -If there has been no alive cells near array, no need to check cell states inside array
    -If there are zero alive cells inside rendered array, and no nearby cells, no need to check alive states inside array
    -Only way an unrendered array becomes rendered is if neighbor array has alive cells on colliding edge
      -Can be explored further and only colliding row/column is checked for alive cells, if 0 alive, array doesn't become active.
      -Cells in corners of array have neihbors in three different arrays!
      -Each cell could have an array of indices where recides each neighbor cell of last round's alive cells.
   
  4. Import Life 1.05 files into the program
      -Also saving these files from user input could be a fun thing
      
       
      
      
   
   Future project idea:
      -Game of life on larger scale, if less than 20 etc cells alive all cells in array die.
