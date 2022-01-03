# Release 0.1

Implements the following commands:
1. `getmouselocation`
2. `search` 
3. `windowactivate`

The window stack is not yet implemented. Therefore it is NOT possible to perform the following:

    w32dotool search --name ".*Firefox.*" windowactivate
    
But it IS possible to perform the following:
    
    w32dotool windowactivate $(w32dotool search --name ".*Firefox.*")
