./mapper < $1 > mapper_output.txt
./reducer < mapper_output.txt > reducer_output.txt
./combiner < $1 > combiner_output.txt
diff reducer_output.txt combiner_output.txt
diff combiner_output.txt $2