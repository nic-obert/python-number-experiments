with open('large_number.py', 'w') as f:
    n = '5232'*1000000 # 4 million digits
    f.write(f'n = {n}\nprint(n)\nn += 89\nprint(n)')