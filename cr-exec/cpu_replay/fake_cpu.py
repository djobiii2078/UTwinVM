from multiprocessing import Process
  
def run_proc():
    x = 0
    while x < 10000000000:
        x += 1



for num in range(8):
        Process(target=run_proc).start()
