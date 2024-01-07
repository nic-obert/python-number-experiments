with open('large_number.py', 'w') as f:
    hex_n = hex(10**1000000) # 4 million digits
    f.write(f'n = {hex_n}\nprint(n)\nn += 89\nprint(n)\n')