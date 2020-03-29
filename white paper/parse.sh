#Using 64 thread(s) with 685231 nodes, 1000 steps per walk, dampingVal[0.300000]: 22.817952
#  1    2   3         4    5     6      7    8     9  10      11                    12

cat $1 |
awk ' BEGIN{
	printf "p,n,K,D,time\n"
	OFS=","
}
{ print $2, $5, $7, substr($11, 12, 3 ), $NF }
'

