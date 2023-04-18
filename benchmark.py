
import sys
import os


dataset="dataset/"
output="output/"

read_list = [f for f in os.listdir("dataset") if f.endswith(".ppm")]



for r in read_list :
    read_file=f'{dataset}{r}'
    write_file=f'out_{r}'
    bin_seq="sequential/seq"
    bin_mp="parallel_openMP/parallel_mp"
    print(f'*********** Image {r} *********')
    print (f'For Sequential')
    cmd = f'./{bin_seq} -r {read_file} -w {output}{write_file}'
    ret = os.system(cmd)
    print(ret)
    print (f'For openMP')
    cmd = f'./{bin_mp} -r {read_file} -w {output}mp_{write_file}'
    ret = os.system(cmd)
    print(ret)
    
    
    



