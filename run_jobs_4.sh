#!/bin/bash

mkdir -p output_scenes_6

reportfile="./output_scenes_6/report.md"

if [ -e "$reportfile" ]; then
    > "$reportfile"
	echo "Previous report file cleared"
else
    touch "$reportfile"
    echo "Report file: '$reportfile' created."
fi

echo "# Report File">> $reportfile

echo "## Question 3" | tee -a $reportfile

runQ3(){
	mkdir -p ./output_scenes_6/Question_3/$3

	echo "### Method: \`$3\` | SPP: \`$1\` | Anti-Aliasing: \`On\`" >> $reportfile

	echo "Small:"
	./build/render ./scenes/Assignment\ 3/Question\ 3/small.json ./output_scenes_6/Question_3/$3/small_$1_spp.png $1 $2 | tee temp_output_file.txt
	echo "- Small: " \`$(cat temp_output_file.txt | grep "Render" | awk '{print $((NF - 1)), "ms"}')\` >> $reportfile

	echo "Med:"
	./build/render ./scenes/Assignment\ 3/Question\ 3/med.json ./output_scenes_6/Question_3/$3/med_$1_spp.png $1 $2  | tee temp_output_file.txt
	echo "- Med: " \`$(cat temp_output_file.txt | grep "Render" | awk '{print $((NF - 1)), "ms"}')\` >> $reportfile
	
	echo "Big:"
	./build/render ./scenes/Assignment\ 3/Question\ 3/big.json ./output_scenes_6/Question_3/$3/big_$1_spp.png $1 $2 | tee temp_output_file.txt
	echo "- Big: " \`$(cat temp_output_file.txt | grep "Render" | awk '{print $((NF - 1)), "ms"}')\` >> $reportfile
	
	echo "Many:"
	./build/render ./scenes/Assignment\ 3/Question\ 3/many.json ./output_scenes_6/Question_3/$3/many_$1_spp.png $1 $2 | tee temp_output_file.txt
	echo "- Many: " \`$(cat temp_output_file.txt | grep "Render" | awk '{print $((NF - 1)), "ms"}')\` >> $reportfile

	echo
}

for spp in 30;
do
# echo "Starting uniform Sampling"
# 	echo "Starting spp = $spp, Uniform Sampling"
# 	num_samples=$spp
# 	sampling_method=0
# 	sampling_method_name="uniform_hemisphere_sampling"
# 	runQ3 $num_samples $sampling_method $sampling_method_name
# 	echo "spp = $spp, Uniform Sampling Completed!"

echo "Cosine Weighted Sampling"
	echo "Starting spp = $spp, Cosine Weighted Sampling"
	num_samples=$spp
	sampling_method=1
	sampling_method_name="cosine_weighted_sampling"
	runQ3 $num_samples $sampling_method $sampling_method_name
	echo "spp = $spp, Cosine Weighted Sampling Completed!"

# echo "Light Sampling"
# 	echo "Starting spp = $spp, Light Sampling"
# 	num_samples=$spp
# 	sampling_method=2
# 	sampling_method_name="light_sampling"
# 	runQ3 $num_samples $sampling_method $sampling_method_name
# 	echo "spp = $spp, Light Sampling Completed!"
done