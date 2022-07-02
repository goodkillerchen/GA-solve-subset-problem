# GA-solve-subset-sum-problem
use Genetic Algorithm to solve subset problem

# subset sum problem:
The SUBSET-SUM problem involves determining whether or not a subset from a list of integers can sum to a target value. For example, consider the list of nums = [1, 2, 3, 4] . 
If the target = 7 , there are two subsets that achieve this sum: {3, 4} and {1, 2, 4} . If target = 11 , there are no solutions.

#GA(Genetic Algorithm):
An abstraction from the theory of biological evolution that is used to create optimization procedures or methodologies, usually implemented on computers, that 
are used to solve problems.

The Gene here we represent it by 01 series.

Ga can be divided into three parts, which are select, crossover and mutation:

## Select:
In Select process, we should create a fitness function, in order to find how well-adapted an individual is to their environment on genes we created before. In this process, I design 
a fitness function and the fitness fuction will return a score to each gene. The higher the score which means the gene can be better adapted in the environment.

## Cross cover:
In this procedure, it's just like the chromosome crossing-over. The example illustrated below:<br>
origin series:<br>
A: 000|1100|10 <br>
B: 011|0011|00 <br>
Here we can exchange the mid part of series A and B. <br>
new series:<br>
new_A: 000|0011|10<br>
new_B: 011|1100|00<br>
If we find that one of the new series is can be better-adapted in environment than the origin's, we can use them to replace the origin series. Or we weill abandon the new series.

## Mutation:
Mutation is just change one of the bit of 01 series from 0 to 1 or form 1 to 0.

# GA to subset-sum probelm:
The <strong>set size = n.</strong> and
<strong>target value = m</strong>
We can use a 01 series which length (n) is equal to the size of set. It represents whether we seize the number at that position in the set.
We generate 20 01 serieses as the primary genes. There you can generate the number you like as the number of primary genes.
The fitniess function shows below:<br>
<br>
![alt text](https://github.com/goodkillerchen/GA-solve-subset-problem/blob/main/fitness_fun.png)<br>
And then, we can calculate the probability of each seris:
$$ P_s = f_i/\sum_{i=1}^n f_i$$
We can claculate the expection of each series which can be represented:
$$ e_i=P_s * n $$
Assign a copy number to each chromosome string based on the integer part of the $e_i$ value. The copy number represents the number of this chromosome string appear in next generation. The next genration of the genes are generated.
The process of the corss over and mutation is just the swap option to each chromosome string.
