from sys import argv
from json import dumps
o = {
    'tag': '',
    'note': ''
}
ver = argv[1]
if ver.startswith('refs/tags/'):
    ver = ver.split('/')[-1]
o['tag'] = ver
with open('src/docs/changelog.md') as f:
    for lines in f.read().split('\n## '):
        lines = lines.split('\n')
        if lines[0] == ver:
            o['note'] += '\n'.join(lines[1:])
print(dumps(o))