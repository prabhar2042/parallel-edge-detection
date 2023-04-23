
import sys
import os
import subprocess

dataset="dataset/"
output="output/"
read_list = [f for f in os.listdir("dataset") if f.endswith(".ppm")]

def run_sequential(image_name) :
    read_file=f'{dataset}{image_name}'
    write_file=f'out_{image_name}'
    bin_seq="sequential/seq"
    #print (f'For Sequential')
    ret=subprocess.run([f'./{bin_seq}', '-r', read_file, '-w', f'{output}{write_file}'], capture_output=True, text=True)
    return ret.stdout
    

def run_openMP(image_name) :
    read_file=f'{dataset}{image_name}'
    write_file=f'out_{image_name}'
    bin_mp="parallel_openMP/parallel_mp"
    #print (f'For openMP')
    ret=subprocess.run([f'./{bin_mp}', '-r', read_file, '-w', f'{output}mp_{write_file}'], capture_output=True, text=True)
    return ret.stdout

def parser_result(ret) :
    times = {}
    for line in ret.split('\n'):
        if(':' in line ) :
            key, value = line.split(':')
            key = key.strip()
            value = float(value.strip())
            times[key] = value
    return times
    
def get_speed_up(seq_times,mp_times) :
    for k in seq_times.keys() :
        print("speed up for ",k," : ",seq_times[k]/mp_times[k])
        

def get_image_stats(image_id) :
        image_name=read_list[image_id]  
        print(f'*********** Image {image_name} *********')
        seq_ret=run_sequential(image_name)
        seq_times=parser_result(seq_ret)
        mp_ret=run_openMP(image_name)
        mp_times=parser_result(mp_ret)
        get_speed_up(seq_times,mp_times)
      


if len(sys.argv) != 3:
    print("Usage: python3 benchmakr.py image_id seq/mp/cmp")
    exit(-1)
 
image_id=int(sys.argv[1])
test=str(sys.argv[2])
if(test=="seq") :
    image_name=read_list[image_id]  
    print(run_sequential(image_name))
elif(test=="mp") :
    image_name=read_list[image_id]  
    print(run_openMP(image_name))
elif(test=="cmp") :
    get_image_stats(image_id)











    
    



