import os
base = '/home/hhl19/acm_code/MCLIB'

def write_file(name, content):
    path = os.path.join(base, name)
    with open(path, 'w', newline='\n') as f:
        f.write(content.lstrip('\n'))
    print(f'Wrote {name}')
