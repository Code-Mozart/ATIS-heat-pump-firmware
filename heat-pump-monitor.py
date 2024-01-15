import subprocess
import time
import os
import hashlib
import base64


####################################################################################
#                                                                                  #
#   CONSTANTS                                                                      #
#                                                                                  #
####################################################################################


PROCESS_NAME = "heat-pump-firmware"
MAX_PROCESS_LIST_LEN = 120
FLAG_PATH = "./flag.txt"


####################################################################################
#                                                                                  #
#   PROCESS MONITORING IMPLEMENTATION                                              #
#                                                                                  #
####################################################################################


def match_processes(process_name):
    try:
        # Run the ps command to list processes and check if the process is present
        result = subprocess.run(['ps', 'aux'], stdout=subprocess.PIPE, text=True)
        output = result.stdout
        
        # Split the output into lines
        lines = output.splitlines()

        # Remove the first line (header)
        lines.pop(0)

        # Iterate over the lines and check if the process is present
        matching_processes = [line for line in lines if process_name in line]

        current_process_id = os.getpid()
        def get_process_id(process_entry):
            return int(process_entry.split()[1])

        # Remove the current process from the list of matching processes
        matching_processes = [
            process_entry
            for process_entry in matching_processes
            if get_process_id(process_entry) != current_process_id
        ]

        return matching_processes
    except Exception as e:
        print(f"Error: {e}")
        return False


def wait_for_start(process_name, onProcessStarted):
    while True:
        matching_processes = match_processes(process_name)
        if len(matching_processes) > 0:
            onProcessStarted(matching_processes)
            break
        
        time.sleep(1)


def wait_for_stop(process_name, onProcessesRunning, onProcessStopped):
    while True:
        matching_processes = match_processes(process_name)
        if len(matching_processes) == 0:
            onProcessStopped(process_name)
            break
        
        onProcessesRunning(matching_processes)
        time.sleep(1)


def monitor_process(process_name, onBeforeProcessStarted, onProcessStarted, onProcessesRunning, onProcessStopped):
    matching_processes = match_processes(process_name)
    if len(matching_processes) > 0:
        # Process is already running
        onProcessesRunning(matching_processes)
    else:
        onBeforeProcessStarted()
        wait_for_start(process_name, onProcessStarted)

    wait_for_stop(process_name, onProcessesRunning, onProcessStopped)


####################################################################################
#                                                                                  #
#   FLAG CREATION                                                                  #
#                                                                                  #
####################################################################################


BASE_FLAG_KEY = "OKNVfFJOMCVdLE9LJ1hlc0RiPip9OUZFZm5mVkthZns8N2JZRjd3WV9pM0JkekdE"

def xor_bytes(a, b):
    int_a = int.from_bytes(a, byteorder='big')
    int_b = int.from_bytes(b, byteorder='big')
    xor = int_a ^ int_b
    return xor.to_bytes(len(a), byteorder='big')

def encrypt_block(data, key):
    key_hash = hashlib.sha256(key.encode()).digest()
    data_hash = hashlib.sha256(data.encode()).digest()

    return xor_bytes(key_hash, data_hash)

def create_flag():
    flag = ""
    shifts = []
    for i in range(0, len(BASE_FLAG_KEY)):
        as_int = ord(BASE_FLAG_KEY[i])
        shift_by = as_int & (0b11 << ((i*2) % 8))
        shifts.append(shift_by)

    block_size = 24
    shiftet_variants = []
    for shift, index in enumerate(shifts):
        current_offset = (index * block_size + shift) % len(BASE_FLAG_KEY)
        source = BASE_FLAG_KEY * 2
        shifted = source[current_offset:current_offset+block_size]
        shiftet_variants.append(shifted)

    encrypted_blocks = [
        encrypt_block(BASE_FLAG_KEY, variant) 
        for variant in shiftet_variants
    ]
    flag_raw = hashlib.sha256(BASE_FLAG_KEY.encode()).digest()
    for encrypted_block in encrypted_blocks:
        flag_raw = xor_bytes(flag_raw, encrypted_block)

    flag = base64.b64encode(flag_raw).decode()
    return flag


def write_flag(path):
    flag = create_flag()
    with open(path, "w") as f:
        f.write(flag)


####################################################################################
#                                                                                  #
#   FORMATTING HELPERS                                                             #
#                                                                                  #
####################################################################################


def format_path(path):
    return "/".join(path.split("/")[-2:])


def format_process_name(process_entry):
    process_call, *process_arguments = process_entry.split()[10:]

    process_call = format_path(process_call)
    process_arguments = " ".join([format_path(process_argument) for process_argument in process_arguments])
    
    return f"{process_call} {process_arguments}"


####################################################################################
#                                                                                  #
#   MONITORING LISTENERS                                                           #
#                                                                                  #
####################################################################################


def onBeforeProcessStarted():
    print("Waiting for process to start...")


def onProcessStarted(process_names):
    for process_name in process_names:
        print(f"Process started: {process_name}")

    print("Waiting for process to stop...")


def onProcessesRunning(process_entries):
    process_names = [
        format_process_name(process_entry)
        for process_entry in process_entries
    ]
    processes = ", ".join([f'"${process_name}"' for process_name in process_names])
    if len(processes) > MAX_PROCESS_LIST_LEN:
        processes = f"{processes[:MAX_PROCESS_LIST_LEN - 1]}…"

    print(f"{len(process_names)} processes running: {processes}")


def onProcessStopped(process_name):
    print(f"Process stopped: {process_name}")
    print("Creating flag...")
    write_flag(FLAG_PATH)
    print("Flag created! Bye!")


####################################################################################
#                                                                                  #
#   MAIN                                                                           #
#                                                                                  #
####################################################################################


monitor_process(
    PROCESS_NAME,
    onBeforeProcessStarted,
    onProcessStarted,
    onProcessesRunning,
    onProcessStopped
)
