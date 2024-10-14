class Moded_Array():
    def __init__(self, max_size: int) -> None:
        self.max_size = max_size
        self.array = list([])
        print(f"Initializing modified array with max size: {self.max_size}")
        
    def append(self, element: int) -> None:
        # self.current_size = self.current_size + 1
        self.array.append(element)
        
        # If the size of array is bigger than max allowed size, remove the first (oldest) element in list 
        if (len(self.array) > self.max_size):
            self.array.pop(0) # Removes the first element in the list, so that size x is always contrained between 0 <= x <= max_size
            
    def __len__(self) -> int: # the len() function calls this method  
        return len(self.array)



