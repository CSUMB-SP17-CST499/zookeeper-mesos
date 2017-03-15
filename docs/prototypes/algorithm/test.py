offers = [2,3,5,2,4,6,7]
offers.sort(reverse=True)
tasks = [5,6]
tasks.sort(reverse=True)
offer_matrix = [(1 if y >=x else 0) for z,x in enumerate(tasks) for w,y in enumerate(offers)]
#offer_matrix = [(x,y,0 if x < y else 1, w,z) for z,x in enumerate(offers) for w,y in enumerate(tasks)]
#offer_permutations = [ ( ( x,y ) if ( x[2] == 1 and y[2] == 1 ) and ( x[3] != y[3] ) and ( x[4] != y[4] )  else 123  ) for x in offer_matrix for y in offer_matrix]
#offer_permutations = list(filter(lambda a: a != 123, offer_permutations))
#offer_binary = [ (x,y,0 if 
print(offers)
print(tasks)
print(offer_matrix)

skip = 0
pairs = []
for x in range(0,len(tasks)):
	for y in range(skip,len(offers)):
		if(len(pairs) == len(tasks)):
			continue
		if(tasks[x] <= offers[y]):
			skip += 1
			pairs.append([tasks[x],offers[y]])
			continue
print(pairs)
# for x in range(0,len(offers)):
	# for y in range(x,len(offers)-1):
		# if( offer_matrix[x]==1 and offer_matrix[len(offers)+1+y]==1):
			# print(offers[x],offers[y+1])
#print(offer_permutations)
