import os
import subprocess

class COLORS:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    ORANGE = '\033[38;5;214m'

def compile_cpp(cpp_file):
    ''''
    Compila o arquivo C++
    '''

    if cpp_file == "exaustiva_paralela.cpp":
        compile_command = ["g++", "-Wall", "-fopenmp", "-g", cpp_file, "-o", cpp_file[:-4], "-O3"]
    
    elif cpp_file == "exaustiva_gpu.cu":
        compile_command = ["nvcc", "-arch=sm_70", "-std=c++14", cpp_file, "-o", cpp_file[:-3]]

    else:
        compile_command = ["g++", "-g", cpp_file, "-o", cpp_file[:-4]]
    
    subprocess.run(compile_command, capture_output=True, text=True)

def run_cpp(compiled, input_file):
    '''
    Executa o arquivo C++ compilado
    '''
    with open(input_file, "r") as file:
        
        if compiled[-3:] == ".cu":
            run_command = ["./" + compiled[:-3]]

        if compiled[-4:] == ".cpp":
            run_command = ["./" + compiled[:-4]]

        result = subprocess.run(run_command, stdin=file, capture_output=True, text=True)
    return result.stdout

def main():
    cpp_files = ["exaustiva_paralela.cpp", "exaustiva_sequencial.cpp", "exaustiva_gpu.cu"]
    input_folder = "new_inputs"

    input_files = [file for file in os.listdir(input_folder) if file.endswith(".txt")]

    for cpp_file in cpp_files:
        compile_cpp(cpp_file)
        for input in input_files:
            
            input_path = os.path.join(input_folder, input)

            # Compila o programa
            print(f"{COLORS.BOLD}{COLORS.HEADER}-----------------------------------------------------------------------------------{COLORS.ENDC}")
            print(f"{COLORS.BOLD}{COLORS.HEADER}Processing {cpp_file} with input: {COLORS.OKCYAN}{input}{COLORS.ENDC}\n")

            # Output do programa
            output = run_cpp(cpp_file, input_path)

            print(f"{COLORS.BOLD}{COLORS.ORANGE}Output: {COLORS.OKGREEN}{output}{COLORS.ENDC}")

if __name__ == "__main__":
    main()