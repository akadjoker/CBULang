import time

def fibonacci(n):
    if n <= 1:
        return n
    else:
        return fibonacci(n-1) + fibonacci(n-2)


n = 30 
start_time = time.time()
print(f"Start: {start_time}")

for i in range(n):
    resultado = fibonacci(i)
    print(f"Fibonacci({i}) = {resultado}")

end_time = time.time()
print(f"End: {end_time}")

print(f"Time passed: {end_time - start_time} seconds")
