offers = 2,3,5,2,4,6
tasks = 5,6
offer_matrix = [(x,y,0 if x < y else 1, w,z) for z,x in enumerate(offers) for w,y in enumerate(tasks)]
offer_permutations = [ ( ( x,y ) if ( x[2] == 1 and y[2] == 1 ) and ( x[3] != y[3] )  else 123  ) for x in offer_matrix for y in offer_matrix]
offer_permutations = list(filter(lambda a: a != 123, offer_permutations))
#offer_binary = [ (x,y,0 if 
print(offers)
print(tasks)
print(offer_matrix)
print(offer_permutations)
