def most_frequent(nums):
    counts = {}
    
    for num in nums:
        if num in counts:
            counts[num] += 1
        else:
            counts[num] = 1
    best_num = None
    best_count = 0
    for num, count in counts.items():
        if count > best_count:
            best_count = count
            best_num = num

            
    return best_num

a1 = (1,1,3,4,88,9,5,88,88,44,2,88)

print(most_frequent(a1))

